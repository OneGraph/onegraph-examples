open Utils;

module Css = AppStyle;

let userIcon = requireAssetURI("./img/user.png");

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
        OneGraphAuth.isLoggedIn(self.state.auth, "gmail")
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
              <h1 className=Css.pageTitle> {string("Welcome to SpotDJ")} </h1>
              <LinkShare
                isPublic={self.state.isPublic}
                toggleShareStatus={() => self.send(ToggleShareStatus)}
              />
              <CurrentlyPlaying />
            </div> :
            <LogIn
              auth={self.state.auth}
              setLogInStatus={status => self.send(SetLogInStatus(status))}
            />
        }
      </div>
    ),
};
