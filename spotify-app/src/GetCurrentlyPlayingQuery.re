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

type formattedData = {
  albumImageUrl: string,
  artistName: string,
  isPlaying: bool,
  positionMs: int,
  progressPct: float,
  songName: string,
  trackId: option(string),
  userIconUrl: string,
  userName: string,
};

module GetCurrentlyPlayingQuery =
  ReasonApollo.CreateQuery(GetCurrentlyPlaying);

let component = ReasonReact.statelessComponent("GetCurrentlyPlaying");

let make = (
  children
) => {
  ...component,
  render: _self =>
    <GetCurrentlyPlayingQuery pollInterval=500>
      ...{
        ({result}) =>
          switch (result) {
          | Loading => ReasonReact.null
          | Error(error) =>
            <div> {ReasonReact.string(error##message)} </div>
          | Data(response) => {

            let userName =
              response##spotify##me
              ->flatMap(me => me##displayName)
              ->getWithDefault("")

            let userImages =
              response##spotify##me
              ->flatMap(me => me##images)
              ->getWithDefault([||]);

            let userDefaultIcon = Utils.requireAssetURI("./img/user.png");
            let userIconUrl = Utils.getImageUrl(
              ~images=userImages,
              ~defaultImage=userDefaultIcon
            );

            let spotifyIsLaunched =
              response##spotify##me
              ->flatMap(me => me##player)
              ->flatMap(player => player##item)

            switch (spotifyIsLaunched) {
            | None => ReasonReact.string("Nobody is listening to Spotify on this account right now.")
            | Some(_item) => {
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
                  *. 100.

              let albumImages =
                response##spotify##me
                ->flatMap(me => me##player)
                ->flatMap(player => player##item)
                ->flatMap(item => item##album)
                ->flatMap(album => album##images)
                ->getWithDefault([||]);

              let defaultAlbumImage = Utils.requireAssetURI("./img/now-playing-ex.png");
              let albumImageUrl = Utils.getImageUrl(
                ~images=albumImages,
                ~defaultImage=defaultAlbumImage
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
                  ->flatMap(item => item##id);



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
              });
              }
            }
          }
        }
      }
    </GetCurrentlyPlayingQuery>,
};
