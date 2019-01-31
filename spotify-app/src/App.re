open Emotion;
open ReasonReact;

let header = (~centered) => [%css
  [
    transform(translateY(centered ? `vh(20.) : `zero)),
    transitionProperty("transform"),
    transitionDuration(`ms(600)),
    transitionTimingFunction(`easeOut),
  ]
];

let pageTitle = [%css [fontSize(`px(56)), marginBottom(`px(16))]];
let pageSubTitle = [%css
  [fontSize(`px(32)), marginBottom(`px(64)), fontWeight(200)]
];

let main = [%css [position(`relative)]];
let welcome = [%css
  [
    position(`absolute),
    marginLeft(`auto),
    marginRight(`auto),
    left(`zero),
    right(`zero),
  ]
];

type logInStatus =
  | HandshakingToken
  | LoggedIn
  | LoggedOut;

type state = {
  logInStatus,
  auth: OneGraphAuth.auth,
};

type action =
  | LogIn
  | LogOut;

let component = reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {logInStatus: HandshakingToken, auth: Client.auth},
  didMount: self =>
    Js.Promise.(
      OneGraphAuth.isLoggedIn(self.state.auth, "spotify")
      |> then_((isLoggedIn: bool) =>
           (isLoggedIn ? LogIn : LogOut) |> self.send |> resolve
         )
      |> catch(err =>
           err
           |> Js.Console.error2(
                "Error checking logged-in status with OneGraph",
              )
           |> resolve
         )
      |> ignore
    ),
  reducer: (action, state) =>
    switch (action) {
    | LogIn => Update({...state, logInStatus: LoggedIn})
    | LogOut => Update({...state, logInStatus: LoggedOut})
    },
  render: self =>
    <div>
      <header
        className={
          Cn.make([
            SharedCss.flexWrapper(~justify=`center, ~align=`center),
            SharedCss.appearAnimation(~direction=`normal, ~delayMs=0),
            header(~centered=self.state.logInStatus == HandshakingToken),
          ])
        }>
        <h1 className=pageTitle> {string("SpotDJ")} </h1>
      </header>
      <main className=main>
        {
          switch (self.state.logInStatus) {
          | HandshakingToken => <div> {string("Warming up a bit...")} </div>
          | LoggedOut =>
            <div
              className={
                Cn.make([
                  SharedCss.appearAnimation(~direction=`normal, ~delayMs=400),
                  welcome,
                ])
              }>
              <h2 className=pageSubTitle>
                {string("Share your Spotify music live")}
              </h2>
              <LogIn
                auth={self.state.auth}
                onLogIn=(() => self.send(LogIn))
              />
            </div>
          | LoggedIn =>
            <GetCurrentlyPlayingQuery>
              ...(
                   (
                     {
                       userName,
                       userIconUrl,
                       songName,
                       artistName,
                       isPlaying,
                       progressPct,
                       albumImageUrl,
                       trackId,
                       positionMs,
                     },
                   ) =>
                     <CurrentlyPlayingContainer
                       auth={self.state.auth}
                       artistName
                       isPlaying
                       progressPct
                       songName
                       userName
                       userIconUrl
                       albumImageUrl
                       positionMs
                       trackId
                       onLogOut={() => self.send(LogOut)}
                     />
                 )
            </GetCurrentlyPlayingQuery>
          }
        }
      </main>
    </div>,
};
