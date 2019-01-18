open Utils;
open Emotion;

module Css = AppStyle;
let pageTitle = [%css [fontSize(`px(56)), marginBottom(`px(16))]];
let pageSubTitle = [%css
  [fontSize(`px(32)), marginBottom(`px(64)), fontWeight(200)]
];

type state = {
  isLoggedIn: bool,
  auth: OneGraphAuth.auth,
  isPublic: bool,
};
type action =
  | SetLogInStatus(bool)
  | ToggleShareStatus;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {isLoggedIn: false, auth: Client.auth, isPublic: true},
  didMount: self =>
    Js.Promise.(
      OneGraphAuth.(
        OneGraphAuth.isLoggedIn(self.state.auth, "spotify")
        |> then_(loginStatus => {
             Js.log(loginStatus);
             self.send(SetLogInStatus(loginStatus));
             resolve();
           })
        |> catch(err => resolve(Js.log(err)))
        |> ignore
      )
    ),
  reducer: (action, state) =>
    switch (action) {
    | SetLogInStatus(isLoggedIn) =>
      ReasonReact.Update({...state, isLoggedIn})
    | ToggleShareStatus =>
      ReasonReact.Update({...state, isPublic: !state.isPublic})
    },
  render: self =>
    ReasonReact.(
      <div className=Css.app>
        {
          self.state.isLoggedIn ?
            <div>
              <GetUsername
                auth={self.state.auth}
                setLogInStatus={status => self.send(SetLogInStatus(status))}
              />
              <h1 className=pageTitle> {string("SpotDJ")} </h1>
              <h2 className=pageSubTitle>
                {string("Share your Spotify music live")}
              </h2>
              <LinkShare
                isPublic={self.state.isPublic}
                toggleShareStatus={() => self.send(ToggleShareStatus)}
              />
              <GetCurrentlyPlayingQuery />
            </div> :
            <LogIn
              auth={self.state.auth}
              setLogInStatus={status => self.send(SetLogInStatus(status))}
            />
        }
      </div>
    ),
};
