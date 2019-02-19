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

let footer = [%css
  [
    position(`fixed),
    bottom(`px(0)),
    backgroundColor(`hex("000")),
    width(`vw(100.)),
    height(`px(48)),
  ]
];

let footerLink = [%css
  [
    margin2(`px(0), `px(4)),
    color(`hex("fff")),
    textDecoration(`underline),
    transitionDuration(`ms(200)),
    select(":hover", [color(`hex("cfcfcf"))]),
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
  trackHistoryList: array(string),
};

type action =
  | NotePlayerStatus(playerStatus)
  | SetLogInStatus(logInStatus)
  | UpdateTrackHistoryList(string);

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
    trackHistoryList: [||],
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
    | UpdateTrackHistoryList(id) =>
      let listLength = Array.length(state.trackHistoryList);
      listLength <= 0 ?
        {
          let newTrackIds =
            Array.copy(state.trackHistoryList) |> Js.Array.concat([|id|]);

          Update({...state, trackHistoryList: newTrackIds});
        } :
        {
          let lastTrackId = state.trackHistoryList[listLength - 1];
          let isCurrentTrack = lastTrackId === id;

          isCurrentTrack ?
            NoUpdate :
            {
              let newTrackIds =
                Array.copy(state.trackHistoryList) |> Js.Array.concat([|id|]);

              Update({...state, trackHistoryList: newTrackIds});
            };
        };
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
              <GetCurrentlyPlayingQuery
                updateTrackHistoryList=(
                  id => self.send(UpdateTrackHistoryList(id))
                )>
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
                         isPremiunSpotify,
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
                         setLogOut={
                           () => self.send(SetLogInStatus(LoggedOut))
                         }
                         trackHistoryList={self.state.trackHistoryList}
                         isPremiunSpotify
                       />
                   )
              </GetCurrentlyPlayingQuery>
            }
          }
        </main>
        <div
          className={
            Cn.make([
              SharedCss.flexWrapper(~justify=`center, ~align=`center),
              footer,
            ])
          }>
          {string("Built by ")}
          <a
            className=footerLink
            href="https://twitter.com/yukims19"
            target="_blank">
            {string(" @yukims19")}
          </a>
          {string(" on top of ")}
          <a
            className=footerLink
            href="https://www.onegraph.com/"
            target="_blank">
            {string(" OneGraph ")}
          </a>
          {string(" view the source on ")}
          <a
            className=footerLink
            href="https://github.com/OneGraph/onegraph-examples/tree/master/spotify-dj"
            target="_blank">
            {string("GitHub")}
          </a>
        </div>
      </div>
    ),
};
