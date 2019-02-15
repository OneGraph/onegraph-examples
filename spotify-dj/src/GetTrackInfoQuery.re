open Belt.Option;

module GetTrackInfo = [%graphql
  {|
     query getTrackInfo($trackId: String!) {
     spotify {
     track(id: $trackId) {
     album {
     images {
     url
     }
     }
     artists {
     name
     }
     name
     }
     }
     }
|}
];

type simpleTrackData = {
  albumImageUrl: string,
  artistName: string,
  songName: string,
};

module GetTrackInfoQuery = ReasonApollo.CreateQuery(GetTrackInfo);

let component = ReasonReact.statelessComponent("GetTrackInfoQuery");

let make = (~trackId, children) => {
  ...component,
  render: _self => {
    let getTrackInfoVariable = GetTrackInfo.make(~trackId, ());

    <GetTrackInfoQuery variables=getTrackInfoVariable##variables>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => ReasonReact.null
             | Error(error) =>
               <div> {ReasonReact.string(error##message)} </div>
             | Data(response) =>
               let albumImages =
                 response##spotify##track
                 ->flatMap(track => track##album)
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
                 response##spotify##track
                 ->flatMap(track => track##name)
                 ->getWithDefault("");

               let artistArray =
                 response##spotify##track
                 ->flatMap(track => track##artists)
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

               children({songName, artistName, albumImageUrl});
             }
         }
    </GetTrackInfoQuery>;
  },
};
