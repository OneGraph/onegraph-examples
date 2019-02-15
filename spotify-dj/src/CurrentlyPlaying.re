open BsReactstrap;
open Utils;
open Emotion;
open SharedCss;

let audioWave = requireAssetURI("./img/audio-wave.png");
let like = requireAssetURI("./img/like.png");
let share = requireAssetURI("./img/share.png");
let pause = requireAssetURI("./img/pause.png");

type action =
  | Toggle;

type state = {isDropdownOpen: bool};

let playerWrapper = [%css
  [
    width(`px(300)),
    position(`relative),
    margin3(`px(0), `auto, `px(64)),
  ]
];

let songNameStyle = [%css
  [
    fontSize(`px(20)),
    marginBottom(`px(0)),
    textAlign(`left),
    width(`px(200)),
  ]
];

let artistNameStyle = [%css
  [
    fontSize(`px(12)),
    marginBottom(`px(0)),
    textAlign(`left),
    textOverflow(`ellipsis),
    width(`px(200)),
    whiteSpace(`nowrap),
    overflow(`hidden),
  ]
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

let component = ReasonReact.reducerComponent("User");

let make =
    (
      ~songName,
      ~artistName,
      ~isPlaying,
      ~progressPct,
      ~albumImageUrl,
      ~isFirstSong,
      _children,
    ) => {
  ...component,
  initialState: () => {isDropdownOpen: false},
  reducer: (action, state) =>
    switch (action) {
    | Toggle => ReasonReact.Update({isDropdownOpen: !state.isDropdownOpen})
    },
  render: self =>
    ReasonReact.(
      <div className={Cn.make([playerWrapper])}>

          <img
            key=songName
            className={
              Cn.make([
                albumImage,
                toFrontAnimation(
                  ~animationKeyframes=
                    isFirstSong ? appearBackToFront : slideToFront,
                ),
              ])
            }
            src=albumImageUrl
            alt="Album Image"
          />
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
          </div>
        </div>
        /*Implement Share and Like later
          <img className=actionBtnStyle src=share alt="Share Icon" />
          <img className=actionBtnStyle src=like alt="Like Icon" />*/
    ),
};
