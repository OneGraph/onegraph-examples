open Emotion;

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

type playerStatus = {
  isPlaying: bool,
  trackId: string,
  positionMs: int,
};

type state = {
  logInStatus,
  auth: OneGraphAuth.auth,
  peerId: BsUuid.Uuid.V4.t,
  playerStatus,
};

type action =
  | NotePlayerStatus(playerStatus)
  | SetLogInStatus(logInStatus);

let component = ReasonReact.reducerComponent("App");

let renderIf = (condition: bool, element: ReasonReact.reactElement) =>
  condition ? element : ReasonReact.null;

let make = _children => {
  ...component,
  initialState: () => {
    logInStatus: HandshakingToken,
    auth: Client.auth,
    playerStatus: {
      isPlaying: false,
      trackId: "",
      positionMs: 0,
    },
    peerId: BsUuid.Uuid.V4.create(),
  },
  didMount: self =>
    Js.Promise.(
      OneGraphAuth.isLoggedIn(self.state.auth, "spotify")
      |> then_((isLoggedIn: bool) => {
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

    | NotePlayerStatus(playerStatus) =>
      ReasonReact.Update({...state, playerStatus})
    },
  render: self =>
    ReasonReact.(
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
            | HandshakingToken => <div> {string("Handshaking")} </div>
            | LoggedOut =>
              <div
                className={
                  Cn.make([
                    SharedCss.appearAnimation(
                      ~direction=`normal,
                      ~delayMs=400,
                    ),
                    welcome,
                  ])
                }>
                <h2 className=pageSubTitle>
                  {string("Share your Spotify music live")}
                </h2>
                <LogIn
                  auth={self.state.auth}
                  logIn=(() => self.send(SetLogInStatus(LoggedIn)))
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
                       />
                   )
              </GetCurrentlyPlayingQuery>
            }
          }
        </main>
      </div>
    ),
};
