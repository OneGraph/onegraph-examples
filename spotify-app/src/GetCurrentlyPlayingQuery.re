open Belt.Option;

module GetCurrentlyPlaying = [%graphql
  {|query findUsername {
     spotify {
     me {
     id
     player {
     isPlaying
     currentlyPlayingType
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
     progressMs
     }
     }
     }
}|}
];

module GetCurrentlyPlayingQuery =
  ReasonApollo.CreateQuery(GetCurrentlyPlaying);

let component = ReasonReact.statelessComponent("GetCurrentlyPlaying");

let make = _children => {
  ...component,
  render: _self =>
    <GetCurrentlyPlayingQuery>
      ...{
        ({result}) =>
          switch (result) {
          | Loading => <div> {ReasonReact.string("Loading")} </div>
          | Error(error) =>
            <div> {ReasonReact.string(error##message)} </div>
          | Data(response) => {

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

              let imageArray =
                response##spotify##me
                ->flatMap(me => me##player)
                ->flatMap(player => player##item)
                ->flatMap(item => item##album)
                ->flatMap(album => album##images)
                ->getWithDefault([||]);

              let imageUrlArray =
                Js.Array.map(
                  image =>
                    switch (image##url) {
                    | Some(url) => url
                    | None => ""
                    },
                  imageArray,
                )
                |> Js.Array.filter(name => name !== "");

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

              <CurrentlyPlaying
                songName
                artistName
                isPlaying
                progressPct={
                  float_of_int(progressMs)
                  /. float_of_int(durationMs)
                  *. 100.
                }
                imageUrl={
                  Array.length(imageUrlArray) > 1
                  ? imageUrlArray[1]
                  : Array.length(imageUrlArray) == 1 ? imageUrlArray[0] : ""
                }
              />;
              }
            }
          }
        }
      }
    </GetCurrentlyPlayingQuery>,
};