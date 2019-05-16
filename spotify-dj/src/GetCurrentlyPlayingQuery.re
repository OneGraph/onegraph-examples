open Belt.Option;

module GetCurrentlyPlaying = [%graphql
  {|
  query findUsername {
    spotify {
      me {
        id
        displayName
        images {
          url
        }
        product
        player {
          isPlaying
          currentlyPlayingType
          progressMs
          item {
            id
            name
            artists {
              name
            }
            durationMs
            href
            album {
              images {
                url
              }
            }
          }
        }
      }
    }
  }
|}
];

type formattedData = {
  albumImageUrl: string,
  artistName: string,
  isPlaying: bool,
  positionMs: int,
  progressPct: float,
  songName: string,
  trackId: string,
  userIconUrl: string,
  userName: string,
  isPremiumSpotify: bool,
};

module GetCurrentlyPlayingQuery =
  ReasonApollo.CreateQuery(GetCurrentlyPlaying);

let component = ReasonReact.statelessComponent("GetCurrentlyPlaying");

let make = (~updateTrackHistoryList, children) => {
  ...component,
  render: _self =>
    <GetCurrentlyPlayingQuery pollInterval=1000>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => ReasonReact.null
             | Error(error) =>
               <div> {ReasonReact.string(error##message)} </div>
             | Data(response) =>
               let userName =
                 response##spotify##me
                 ->flatMap(me => me##displayName)
                 ->getWithDefault("");

               let userImages =
                 response##spotify##me
                 ->flatMap(me => me##images)
                 ->getWithDefault([||]);

               let userDefaultIcon = Utils.requireAssetURI("./img/user.png");
               let userIconUrl =
                 Utils.getImageUrl(
                   ~images=userImages,
                   ~defaultImage=userDefaultIcon,
                 );

               let spotifyIsLaunched =
                 response##spotify##me
                 ->flatMap(me => me##player)
                 ->flatMap(player => player##item);

               switch (spotifyIsLaunched) {
               | None =>
                 ReasonReact.string(
                   "No music is playing on this Spotify account right now.
                    To get started, open the Spotify app on the web, your desktop, or even your phone, and play some tunes for us to listen to!",
                 )
               | Some(_item) =>
                 let spotifyStatus =
                   response##spotify##me->flatMap(me => me##product);

                 let isPlaying =
                   response##spotify##me
                   ->flatMap(me => me##player)
                   ->flatMap(player => player##isPlaying)
                   ->getWithDefault(false);

                 let durationMs =
                   response##spotify##me
                   ->flatMap(me => me##player)
                   ->flatMap(player => player##item)
                   ->flatMap(item => item##durationMs)
                   ->getWithDefault(0);

                 let progressMs =
                   response##spotify##me
                   ->flatMap(me => me##player)
                   ->flatMap(player => player##progressMs)
                   ->getWithDefault(0);

                 let progressPct =
                   float_of_int(progressMs)
                   /. float_of_int(durationMs)
                   *. 100.;

                 let albumImages =
                   response##spotify##me
                   ->flatMap(me => me##player)
                   ->flatMap(player => player##item)
                   ->flatMap(item => item##album)
                   ->flatMap(album => album##images)
                   ->getWithDefault([||]);

                 let defaultAlbumImage =
                   Utils.requireAssetURI("./img/now-playing-ex.png");

                 let albumImageUrl =
                   Utils.getImageUrl(
                     ~images=albumImages,
                     ~defaultImage=defaultAlbumImage,
                   );

                 let songName =
                   response##spotify##me
                   ->flatMap(me => me##player)
                   ->flatMap(player => player##item)
                   ->flatMap(item => item##name)
                   ->getWithDefault("");

                 let artistArray =
                   response##spotify##me
                   ->flatMap(me => me##player)
                   ->flatMap(player => player##item)
                   ->flatMap(item => item##artists)
                   ->getWithDefault([||]);

                 let trackId =
                   response##spotify##me
                   ->flatMap(me => me##player)
                   ->flatMap(player => player##item)
                   ->flatMap(item => item##id)
                   ->getWithDefault("");

                 let artistNameArray =
                   Js.Array.map(
                     artist =>
                       switch (artist##name) {
                       | Some(name) => name
                       | None => ""
                       },
                     artistArray,
                   )
                   |> Js.Array.filter(name => name !== "");

                 let artistName = Js.Array.joinWith(",", artistNameArray);

                 updateTrackHistoryList(trackId);

                 let isPremiumSpotify =
                   switch (spotifyStatus) {
                   | Some("premium") => true
                   | _ => false
                   };

                 children({
                   albumImageUrl,
                   artistName,
                   isPlaying,
                   positionMs: progressMs,
                   progressPct,
                   songName,
                   trackId,
                   userName,
                   userIconUrl,
                   isPremiumSpotify,
                 });
               };
             }
         }
    </GetCurrentlyPlayingQuery>,
};

module Test = {
  module UserQueryConfig = [%graphql
    {|
  query findUsername {
    spotify {
      me {
        id
        displayName
        images {
          url
        }
        product
        player {
          isPlaying
          currentlyPlayingType
          progressMs
          item {
            id
            name
            artists {
              name
            }
            durationMs
            href
            album {
              images {
                url
              }
            }
          }
        }
      }
    }
  }
|}
  ];

  module UserQuery = ReasonApolloHooks.Query.Make(UserQueryConfig);

  [@react.component]
  let make = () => {
    /* Both variant and records available */
    let (simple, _full) = UserQuery.use();

    <div>
      {
        switch (simple) {
        | Loading => <p> {React.string("Loading...")} </p>
        | Data(data) =>
          <p>
            {
              React.string(
                [%get_in data##spotify##me#??displayName]
                ->Belt.Option.getWithDefault("No name"),
              )
            }
          </p>
        | NoData
        | Error(_) => <p> {React.string("Get off my lawn!")} </p>
        }
      }
    </div>;
  };
};
