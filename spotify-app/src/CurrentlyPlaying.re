open BsReactstrap;
open Utils;
open Emotion;

let audioWave = requireAssetURI("./img/audio-wave.png");
let like = requireAssetURI("./img/like.png");
let share = requireAssetURI("./img/share.png");
let pause = requireAssetURI("./img/pause.png");

let playerWrapper = [%css
  [
    width(`px(300)),
    position(`relative),
    margin3(`px(0), `auto, `px(64)),
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

let component = ReasonReact.statelessComponent("CurrentlyPlaying");

let make =
    (
      ~songName,
      ~artistName,
      ~isPlaying,
      ~progressPct,
      ~albumImageUrl,
      _children,
    ) => {
  ...component,
  render: _ =>
    ReasonReact.(
      <div className="current-playing">
        <div className=playerWrapper>
          <img className=albumImage src=albumImageUrl alt="Album Image" />
          <div
            className={
              SharedCss.flexWrapper(~justify=`spaceBetween, ~align=`center)
            }>
            <div>
              <h3 className=songNameStyle> {string(songName)} </h3>
              <p className=artistNameStyle> {string(artistName)} </p>
            </div>
            {
              isPlaying ?
                null :
                <img className=pauseBtnStyle src=pause alt="Pause Icon" />
            }
            <SoundWave isPlaying />
          </div>
          <div
            className={
              Cn.make([
                SharedCss.flexWrapper(~justify=`spaceBetween, ~align=`center),
              ])
            }>
            <Progress
              color="success"
              className=progressBarStyle
              value=progressPct
            />
            <img className=actionBtnStyle src=share alt="Share Icon" />
            <img className=actionBtnStyle src=like alt="Like Icon" />
          </div>
        </div>
      </div>
    ),
};
