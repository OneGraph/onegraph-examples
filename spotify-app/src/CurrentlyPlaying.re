open Utils;
open Emotion;
module Css = AppStyle;
let songImage = requireAssetURI("./img/now-playing-ex.png");
let audioWave = requireAssetURI("./img/audio-wave.png");
let like = requireAssetURI("./img/like.png");
let share = requireAssetURI("./img/share.png");

type action =
  | Toggle;

type state = {isDropdownOpen: bool};

let songInfo = [%css
  [
    alignItems(`center),
    width(`pct(100.)),
    padding(`px(16)),
    position(`absolute),
    bottom(`px(0)),
    color(`hex("ffffff")),
    select(".audio-wave", [width(`px(48))]),
    select(
      ".action-btn-circle",
      [
        width(`px(32)),
        border(`px(2), `solid, `hex("ffffff")),
        borderRadius(`pct(50.)),
        padding(`px(4)),
        select(
          ":hover",
          [cursor(`pointer), backgroundColor(`hex("525252"))],
        ),
      ],
    ),
    select(".song-name", [fontSize(`px(24)), marginBottom(`px(0))]),
    select(".singer-name", [fontSize(`px(14)), marginBottom(`px(0))]),
  ]
];

let songImageFilter = [%css
  [
    width(`pct(100.)),
    height(`pct(100.)),
    position(`absolute),
    top(`px(0)),
    backgroundImage(
      `linearGradient((
        `deg(-180.),
        [(0, `rgba((255, 255, 255, 0.00))), (85, `hex("000000"))],
      )),
    ),
  ]
];

let component = ReasonReact.reducerComponent("User");

let make = _children => {
  ...component,
  initialState: () => {isDropdownOpen: false},
  reducer: (action, state) =>
    switch (action) {
    | Toggle => ReasonReact.Update({isDropdownOpen: !state.isDropdownOpen})
    },
  render: self =>
    ReasonReact.(
      <div className="current-playing">
        <div
          style={
            ReactDOMRe.Style.make(
              ~width="300px",
              ~position="relative",
              ~margin="auto",
              (),
            )
          }>
          <img
            style={ReactDOMRe.Style.make(~width="100%", ())}
            src=songImage
            alt="Song Image"
          />
          <div className=songImageFilter />
          <div
            className={
              Cn.make([Css.flexWrapper(`spaceAround, `center), songInfo])
            }>
            <img className="audio-wave" src=audioWave alt="Audio Wave" />
            <div>
              <h3 className="song-name"> {string("Song Name")} </h3>
              <p className="singer-name"> {string("Author Name")} </p>
            </div>
            <img className="action-btn-circle" src=like alt="Like Icon" />
            <img className="action-btn-circle" src=share alt="Share Icon" />
          </div>
        </div>
        <p> {string("Played by userABC")} </p>
      </div>
    ),
};
