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

let defaultUserIcon = Utils.requireAssetURI("./img/user.png");
let defaultAlbumImage = Utils.requireAssetURI("./img/now-playing-ex.png");

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
};

module GetCurrentlyPlayingQuery =
  ReasonApollo.CreateQuery(GetCurrentlyPlaying);

let component = ReasonReact.statelessComponent("GetCurrentlyPlaying");

let make = children => {
  ...component,
  render: _self =>
    <GetCurrentlyPlayingQuery pollInterval=500>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => ReasonReact.null
             | Error(error) =>
               <div> {ReasonReact.string(error##message)} </div>
             | Data(response) =>
               let userName =
                 [%get_in response##spotify##me#??displayName]
                 ->getWithDefault("");

               let userImages =
                 [%get_in response##spotify##me#??images]
                 ->getWithDefault([||]);

               let userIconUrl =
                 Utils.getImageUrl(
                   ~images=userImages,
                   ~defaultImage=defaultUserIcon,
                 );

               let spotifyIsLaunched = [%get_in
                 response##spotify##me#??player#??item
               ];

               switch (spotifyIsLaunched) {
               | None =>
                 ReasonReact.string(
                   "It looks like all of your Spotify players are closed. Try opening the Spotify app on your computer or phone!",
                 )
               | Some(item) =>
                 let isPlaying =
                   [%get_in response##spotify##me#??player#??isPlaying]
                   ->getWithDefault(false);

                 let durationMs = item##durationMs->getWithDefault(0);

                 let positionMs =
                   [%get_in response##spotify##me#??player#??progressMs]
                   ->getWithDefault(0);

                 let progressPct =
                   float_of_int(positionMs)
                   /. float_of_int(durationMs)
                   *. 100.;

                 let albumImages =
                   [%get_in item##album#??images]->getWithDefault([||]);

                 let albumImageUrl =
                   Utils.getImageUrl(
                     ~images=albumImages,
                     ~defaultImage=defaultAlbumImage,
                   );

                 let songName = item##name->getWithDefault("");

                 let artists = item##artists->getWithDefault([||]);

                 let trackId = item##id->getWithDefault("");

                 let artistName =
                   artists
                   |> Js.Array.map(artist =>
                        getWithDefault(artist##name, "")
                      )
                   |> Js.Array.filter(name => name !== "")
                   |> Js.Array.joinWith(", ");

                 children({
                   albumImageUrl,
                   artistName,
                   isPlaying,
                   positionMs,
                   progressPct,
                   songName,
                   trackId,
                   userName,
                   userIconUrl,
                 });
               };
             }
         }
    </GetCurrentlyPlayingQuery>,
};
