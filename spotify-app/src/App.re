open Utils;

module Css = AppStyle;

let userIcon = requireAssetURI("./img/user.png");

type state = {
  isLoggedIn: bool,
  auth: OneGraphAuth.auth,
  userName: option(string),
};
type action =
  | SetLogInStatus(bool);

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {isLoggedIn: false, auth: Client.auth, userName: None},
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
    },
  render: self =>
    ReasonReact.(
      <div className=Css.app>
        {
          self.state.isLoggedIn ?
            <div>
              <Query
                token={
                  switch (OneGraphAuth.authToken(self.state.auth)) {
                  | Some(token) => token
                  | None => ""
                  }
                }
              />
              <User
                auth={self.state.auth}
                setLogInStatus={status => self.send(SetLogInStatus(status))}
                userName={self.state.userName}
              />
              <h1 className=Css.pageTitle> {string("Welcome to SpotDJ")} </h1>
              <LinkShare />
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
