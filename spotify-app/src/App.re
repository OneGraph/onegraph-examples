open Emotion;

let pageTitle = [%css [fontSize(`px(56)), marginBottom(`px(16))]];
let pageSubTitle = [%css
  [fontSize(`px(32)), marginBottom(`px(64)), fontWeight(200)]
];

let main = [%css [position(`relative)]];
let welcome = [%css [
  position(`absolute),
  marginLeft(`auto),
  marginRight(`auto),
  left(`zero),
  right(`zero)
]];

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
      OneGraphAuth.isLoggedIn(self.state.auth, "spotify")
      |> then_(loginStatus => {
            Js.log(loginStatus);
            self.send(SetLogInStatus(loginStatus));
            resolve();
          })
      |> catch(err => resolve(Js.log(err)))
      |> ignore
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
      <div>
        <header className=SharedCss.flexWrapper(~justify=`center, ~align=`center)>
          <h1 className=pageTitle>{string("SpotDJ")}</h1>
        </header>
        <main className=main>
          <div className={
            Cn.make([
              SharedCss.appearAnimation(~direction=self.state.isLoggedIn ? `reverse : `normal),
              welcome
            ])
          }>
            <h2 className=pageSubTitle>
              {string("Share your Spotify music live")}
            </h2>
              <LogIn
                auth={self.state.auth}
                setLogInStatus={status => self.send(SetLogInStatus(status))}
              />
          </div>
          {self.state.isLoggedIn ?
            <div className=SharedCss.appearAnimation(~direction=`normal)>
              <GetCurrentlyPlayingQuery>
                ...{({ userName, songName, artistName, isPlaying, progressPct, imageUrl }) =>
                  <div>
                    <User
                      auth={self.state.auth}
                      setLogInStatus={status => self.send(SetLogInStatus(status))}
                      userName
                    />
                    <CurrentlyPlaying
                      songName
                      artistName
                      isPlaying
                      progressPct
                      imageUrl
                    />
                    <LinkShare
                      isPublic={self.state.isPublic}
                      toggleShareStatus={() => self.send(ToggleShareStatus)}
                    />
                  </div>
                }
              </GetCurrentlyPlayingQuery>
            </div>
            : ReasonReact.null
          }
        </main>
      </div>
    ),
};
