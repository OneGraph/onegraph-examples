open PeerJsBinding;
open ReasonReact;
open Utils;
open Emotion;
open SharedCss;

let recordPlayer = requireAssetURI("./img/record-player.png");
let headphone = requireAssetURI("./img/headphone.png");

let userKindWrapper = [%css
  [position(`fixed), top(`px(16)), left(`px(16))]
];

let userKindIcon = [%css [width(`px(24)), margin2(`px(0), `px(4))]];

let userKindSwitchBtn = [%css [fontSize(`px(12)), fontWeight(600)]];

let unactiveStyle = [%css [opacity(0.5)]];

let statusRibon = [%css
  [
    fontSize(`px(32)),
    position(`absolute),
    left(`pct(50.)),
    transform(`translateX(`pct(-50.))),
    zIndex(2),
    top(`px(200)),
    backgroundColor(`hex("b50303")),
    width(`px(400)),
    color(`hex("ffffffc9")),
  ]
];

type isConnected =
  | Connected
  | Connecting
  | Error
  | DjAway;

type followerNum = int;

type userKind =
  | DJ(followerNum)
  | Listener(string);

type state = {
  switchboard: option(PeerJsBinding.switchboard),
  peerId: option(BsUuid.Uuid.V4.t),
  userKind,
  isConnectedToDj: isConnected,
};

type action =
  | NotifyPeers
  | UpdateDjFollowerNum(followerNum)
  | ExamineDJState(SpotifyControls.playerStatus)
  | PausePlayer
  | SyncPlayer(SpotifyControls.playerStatus)
  | SetSwitchboard(switchboard, BsUuid.Uuid.V4.t)
  | UpdateUserKind(userKind)
  | UpdateIsConnectedToDj(isConnected);

let component = ReasonReact.reducerComponent("App");

let msDiffThreshold = 10000;

let make =
    (
      ~auth,
      ~songName,
      ~artistName,
      ~isPlaying,
      ~progressPct,
      ~albumImageUrl,
      ~positionMs,
      ~trackId,
      ~userName,
      ~userIconUrl,
      ~setLogOut,
      _children,
    ) => {
  ...component,
  initialState: () => {
    switchboard: None,
    peerId: None,
    userKind: DJ(0),
    isConnectedToDj: Connecting,
  },
  didMount: self => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    let query = QueryString.parseQueryString(url.search);
    let userKind =
      switch (Js.Dict.get(query, "dj")) {
      | Some(Single(djId)) => Listener(djId)
      | _ => DJ(0)
      };
    self.send(UpdateUserKind(userKind));
    let maybePeerId = Utils.localStorageGetItem("spotDjPeerId");
    let peerId =
      switch (Js.nullToOption(maybePeerId)) {
      | None =>
        let newId = BsUuid.Uuid.V4.create();
        Utils.localStorageSetItem(
          "spotDjPeerId",
          BsUuid.Uuid.V4.toString(newId),
        );
        newId;
      | Some(id) => id
      };
    let me = newSwitchBoard(peerId);
    self.send(SetSwitchboard(me, peerId));

    let onConnecting = () => {
      Js.log("No Connection");
      self.send(UpdateIsConnectedToDj(Connecting));
    };

    switch (userKind) {
    | DJ(_) =>
      Js.log("I'm a DJ");
      let intervalId =
        Js.Global.setInterval(() => self.send(NotifyPeers), 1000);
      self.onUnmount(() => Js.Global.clearInterval(intervalId));
    | Listener(djId) =>
      initiateConnection(
        ~switchboard=me,
        ~updateIsConnectedToDjConnected=
          () => self.send(UpdateIsConnectedToDj(Connected)),
        ~updateIsConnectedToDjError=
          () => self.send(UpdateIsConnectedToDj(Error)),
        ~updateIsConnectedToDjDjAway=
          () => self.send(UpdateIsConnectedToDj(DjAway)),
        ~examineDJState=
          playerStatus => self.send(ExamineDJState(playerStatus)),
        ~onConnecting,
        ~djId,
      );
      Js.log({j|My switchboard $me has a connection to dj $djId|j});
    };
  },
  willUpdate: ({oldSelf, newSelf}) =>
    switch (newSelf.state.userKind, newSelf.state.isConnectedToDj) {
    | (Listener(_), DjAway) =>
      Js.log("Dj awaying....");
      switch (newSelf.state.switchboard) {
      | None => ()
      | Some(switchboard) =>
        let url = ReasonReact.Router.dangerouslyGetInitialUrl();
        let query = QueryString.parseQueryString(url.search);
        let djId =
          switch (Js.Dict.get(query, "dj")) {
          | Some(Single(djId)) => djId
          | _ => ""
          };
        initiateConnection(
          ~switchboard,
          ~updateIsConnectedToDjConnected=
            () => newSelf.send(UpdateIsConnectedToDj(Connected)),
          ~updateIsConnectedToDjError=
            () => newSelf.send(UpdateIsConnectedToDj(Error)),
          ~updateIsConnectedToDjDjAway=
            () => newSelf.send(UpdateIsConnectedToDj(DjAway)),
          ~examineDJState=
            playerStatus => newSelf.send(ExamineDJState(playerStatus)),
          ~djId,
          ~onConnecting=() => (),
        );
      };
    | (Listener(_), Connecting) =>
      Js.log("connecting.....");
      let timeoutId =
        Js.Global.setTimeout(
          () => newSelf.send(UpdateIsConnectedToDj(DjAway)),
          3000,
        );
      newSelf.onUnmount(() => Js.Global.clearTimeout(timeoutId));
      ();
    | (_, _) => Js.log("Else///")
    },
  reducer: (action, state) =>
    switch (action) {
    | NotifyPeers =>
      SideEffects(
        (
          self =>
            switch (self.state.switchboard) {
            | None => ()
            | Some(switchboard) =>
              let rawMessage =
                Serialize.stringOfMessage(
                  DjPlayerState({isPlaying, trackId, positionMs}),
                );
              let getDjFollowerNum = followerNum =>
                self.send(UpdateDjFollowerNum(followerNum));
              broadcast(switchboard, rawMessage, getDjFollowerNum);
            }
        ),
      )
    | UpdateDjFollowerNum(followerNum) =>
      Update({...state, userKind: DJ(followerNum)})
    | SetSwitchboard(switchboard, peerId) =>
      Update({
        ...state,
        switchboard: Some(switchboard),
        peerId: Some(peerId),
      })
    | ExamineDJState(dj) =>
      /* Determine if we should run a OneGraph Spotify mutation to sync players, and if so, which mutation */
      let djAction =
        switch (
          dj.isPlaying,
          isPlaying,
          dj.trackId == trackId,
          abs(dj.positionMs - positionMs) > msDiffThreshold,
        ) {
        | (false, true, _, _) => Some(PausePlayer)
        | (true, false, _, _)
        | (true, true, false, _)
        | (true, true, true, true) => Some(SyncPlayer(dj))
        | (true, true, true, false)
        | (false, false, _, _) => None
        };

      switch (djAction) {
      | None => NoUpdate
      | Some(message) => SideEffects((self => self.send(message)))
      };

    /* Listener actions */
    | PausePlayer =>
      SideEffects(
        (
          _ =>
            SpotifyControls.pausePlayer(OneGraphAuth.authToken(auth))
            |> ignore
        ),
      )
    | SyncPlayer(playerStatus) =>
      SideEffects(
        (
          _ =>
            SpotifyControls.startPlayer(
              OneGraphAuth.authToken(auth),
              playerStatus,
            )
            |> ignore
        ),
      )
    | UpdateUserKind(userKind) => Update({...state, userKind})
    | UpdateIsConnectedToDj(isConnectedToDj) =>
      Update({...state, isConnectedToDj})
    },
  render: self =>
    <div>
      <div className={appearAnimation(~direction=`normal, ~delayMs=0)}>
        {
          switch (self.state.userKind) {
          | DJ(num) =>
            <div className=userKindWrapper>
              <div
                className={flexWrapper(~justify=`flexStart, ~align=`center)}>
                <img src=recordPlayer alt="DJ Icon" className=userKindIcon />
                <p className=marginZero>
                  <strong className=textBold> {string("DJ ")} </strong>
                  <span className=textWeak>
                    {string(" - " ++ string_of_int(num) ++ " followers")}
                  </span>
                </p>
              </div>
            </div>
          | Listener(_) =>
            <div className=userKindWrapper>
              <div
                className={flexWrapper(~justify=`flexStart, ~align=`center)}>
                <img
                  src=headphone
                  alt="Listener Icon"
                  className=userKindIcon
                />
                <p className=marginZero>
                  <strong className=textBold> {string("Audience ")} </strong>
                  <span className=textWeak>
                    {string(" - listening with ")}
                    <i> {string("ABC")} </i>
                  </span>
                </p>
              </div>
              <button
                className={Cn.make([button, userKindSwitchBtn, marginZero])}>
                <a
                  className=textBlack
                  href={"http://" ++ Utils.Window.host ++ "/#"}>
                  {string("I wanna be DJ")}
                </a>
              </button>
            </div>
          }
        }
        <pre> {string(trackId ++ string_of_int(positionMs))} </pre>
        <User auth userName userIconUrl setLogOut />
        {
          switch (self.state.userKind, self.state.isConnectedToDj) {
          | (Listener(_), Connected) => null
          | (Listener(_), Connecting) =>
            <div className=statusRibon> {string("Connecting ...")} </div>
          | (Listener(_), DjAway) =>
            <div className=statusRibon> {string("DJ went away")} </div>
          | (Listener(_), Error) =>
            <div className=statusRibon>
              {string("Whops.. Something is wrong. Try Refresh")}
            </div>
          | (DJ(_), _) => null
          }
        }
        <div
          className={
            switch (self.state.userKind, self.state.isConnectedToDj) {
            | (DJ(_), _)
            | (Listener(_), Connected) => ""
            | (Listener(_), _) => unactiveStyle
            }
          }>
          <CurrentlyPlaying
            songName
            artistName
            isPlaying
            progressPct
            albumImageUrl
          />
        </div>
        <LinkShare
          peerId={
            switch (self.state.peerId) {
            | Some(peerId) => BsUuid.Uuid.V4.toString(peerId)
            | None => ""
            }
          }
        />
      </div>
    </div>,
};
