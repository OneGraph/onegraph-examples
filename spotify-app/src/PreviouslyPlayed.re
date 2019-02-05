open BsReactstrap;
open Utils;
open Emotion;

let previouslyPlayed = [%css
  [position(`absolute), perspective(`px(1000)), width(`pct(40.))]
];

let prePlayerWrapper = [%css
  [
    width(`px(200)),
    position(`relative),
    margin3(`px(0), `auto, `px(64)),
    transformStyle(`preserve3d),
    transform(`scale(0.7)),
  ]
];

let songNameStyle = [%css
  [fontSize(`px(24)), marginBottom(`px(0)), textAlign(`left)]
];

let artistNameStyle = [%css
  [fontSize(`px(14)), marginBottom(`px(0)), textAlign(`left)]
];

let audioWaveStyle = [%css [height(`px(48))]];

let actionBtnStyle = [%css
  [
    width(`px(32)),
    border(`px(2), `solid, `hex("ffffff")),
    borderRadius(`pct(50.)),
    padding(`px(4)),
    margin(`px(4)),
    select(":hover", [cursor(`pointer), backgroundColor(`hex("525252"))]),
  ]
];

let pauseBtnStyle = [%css
  [width(`px(40)), position(`absolute), right(`px(6)), opacity(0.85)]
];

let progressBarStyle = [%css [width(`px(200)), height(`px(4))]];

let albumImage = [%css [width(`pct(100.)), height(`px(300))]];

let component = ReasonReact.statelessComponent("PreviouslyPlayed");

let make = (~trackList, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div
        className={
          Cn.make([
            previouslyPlayed,
            SharedCss.flexWrapper(~justify=`flexEnd, ~align=`center),
          ])
        }>
        {
          trackList
          |> Array.mapi((idx, trackId) =>
               <GetTrackInfoQuery trackId>
                 ...{
                      ({songName, artistName, albumImageUrl}) =>
                        <SimpleTrack
                          songName
                          artistName
                          albumImageUrl
                          newestHistory={
                            idx === Array.length(trackList) - 2 ? true : false
                          }
                          isCurrentTrack={
                            idx === Array.length(trackList) - 1 ? true : false
                          }
                          idx
                        />
                    }
               </GetTrackInfoQuery>
             )
          |> ReasonReact.array
        }
      </div>
    ),
};
