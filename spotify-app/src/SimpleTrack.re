open Emotion;

let prePlayerWrapper = [%css
  [
    width(`px(200)),
    position(`relative),
    margin3(`px(0), `px(0), `px(64)),
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

let albumImage = [%css [width(`pct(100.)), height(`px(300))]];

let component = ReasonReact.statelessComponent("PreviouslyPlayed");

let make = (~songName, ~artistName, ~albumImageUrl, ~newestHistory, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div
        key=songName
        className={
          Cn.make([
            SharedCss.flipToLeftAnimation(
              ~startScale=newestHistory ? 1. : 0.7,
              ~endScale=0.7,
            ),
            prePlayerWrapper,
          ])
        }>
        <img className=albumImage src=albumImageUrl alt="Album Image" />
        <div
          className={
            SharedCss.flexWrapper(~justify=`spaceBetween, ~align=`center)
          }>
          <div>
            <h3 className=songNameStyle> {string(songName)} </h3>
            <p className=artistNameStyle> {string(artistName)} </p>
          </div>
        </div>
      </div>
    ),
};
