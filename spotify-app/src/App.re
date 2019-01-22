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

type logInStatus =
  | HandshakingToken
  | LoggedIn
  | LoggedOut;

type state = {
  logInStatus: logInStatus,
  auth: OneGraphAuth.auth,
  isPublic: bool,
};

type action =
  | SetLogInStatus(logInStatus)
  | ToggleShareStatus;

let component = ReasonReact.reducerComponent("App");

let renderIf = (condition: bool, element: ReasonReact.reactElement) =>
  condition ? element : ReasonReact.null

let make = _children => {
  ...component,
  initialState: () => {logInStatus: HandshakingToken, auth: Client.auth, isPublic: true},
  didMount: self =>
    Js.Promise.(
      OneGraphAuth.isLoggedIn(self.state.auth, "spotify")
      |> then_((isLoggedIn:bool) => {
            let logInStatus = isLoggedIn ? LoggedIn : LoggedOut;
            self.send(SetLogInStatus(logInStatus));
            resolve();
          })
      |> catch(err => resolve(Js.log(err)))
      |> ignore
    ),
  reducer: (action, state) =>
    switch (action) {
    | SetLogInStatus(logInStatus) =>
      ReasonReact.Update({...state, logInStatus})
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
          {renderIf(
            self.state.logInStatus == LoggedOut,
            <div className={
              Cn.make([
                SharedCss.appearAnimation(~direction=`normal, ~delayMs=200),
                welcome
              ])
            }>
              <h2 className=pageSubTitle>
                {string("Share your Spotify music live")}
              </h2>
                <LogIn
                  auth={self.state.auth}
                  logIn={() => self.send(SetLogInStatus(LoggedIn))}
                />
            </div>
          )}
          {renderIf(
            self.state.logInStatus == LoggedIn,
            <GetCurrentlyPlayingQuery>
            ...{({ userName, songName, artistName, isPlaying, progressPct, imageUrl }) =>
                <div className=SharedCss.appearAnimation(~direction=`normal, ~delayMs=0)>
                    <User
                      auth={self.state.auth}
                      logOut={() => self.send(SetLogInStatus(LoggedOut))}
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
          )}
        </main>
      </div>
    ),
};
