open PeerJsBinding;
open ReasonReact;
type userKind =
  | DJ
  | Listener(string);

type state = {
  switchboard: option(PeerJsBinding.switchboard),
  peerId: option(BsUuid.Uuid.V4.t),
  userKind,
};

type action =
  | NotifyPeers
  | ExamineDJState(SpotifyControls.playerStatus)
  | PausePlayer
  | SyncPlayer(SpotifyControls.playerStatus)
  | SetSwitchboard(switchboard, BsUuid.Uuid.V4.t);

let component = ReasonReact.reducerComponent("App");

let msDiffThreshold = 5000;

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
  initialState: () => {switchboard: None, peerId: None, userKind: DJ},
  didMount: self => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    let query = QueryString.parseQueryString(url.search);

    let userKind =
      switch (Js.Dict.get(query, "dj")) {
      | Some(Single(djId)) => Listener(djId)
      | _ => DJ
      };

    let options: PeerJsBinding.options =
      PeerJsBinding.options(~key="abc", ());

    let maybePeerId = Utils.sessionStorageGetItem("spotDjPeerId");
    let peerId =
      switch (Js.nullToOption(maybePeerId)) {
      | None =>
        let newId = BsUuid.Uuid.V4.create();
        Utils.sessionStorageSetItem(
          "spotDjPeerId",
          BsUuid.Uuid.V4.toString(newId),
        );
        newId;
      | Some(id) => id
      };

    let me = newSwitchBoard(peerId, options);
    self.send(SetSwitchboard(me, peerId));

    let onData = data => {
      Js.log2("Received data", data);
      switch (Serialize.messageOfString(data)) {
      | DjPlayerState(playerStatus) =>
        self.send(ExamineDJState(playerStatus))
      | Unrecognized(raw) => Js.log2("Unrecognized message from peer: ", raw)
      | Malformed(raw) => Js.log2("Malformed message from peer: ", raw)
      };
    };

    switch (userKind) {
    | DJ =>
      Js.log("I'm a DJ");
      let intervalId =
        Js.Global.setInterval(() => self.send(NotifyPeers), 1000);
      self.onUnmount(() => Js.Global.clearInterval(intervalId));
    | Listener(djId) =>
      let connection = connect(~me, ~toPeerId=djId, ~onData, ());
      Js.log(
        {j|My switchboard $me has a connection $connection to dj $djId|j},
      );
    };
  },
  reducer: (action, state) =>
    switch (action) {
    | NotifyPeers =>
      SideEffects(
        (
          ({state}) =>
            switch (state.switchboard) {
            | None => ()
            | Some(switchboard) =>
              let rawMessage =
                Serialize.stringOfMessage(
                  DjPlayerState({isPlaying, trackId, positionMs}),
                );
              broadcast(switchboard, rawMessage);
            }
        ),
      )
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
        (_ => SpotifyControls.startPlayer(auth, playerStatus) |> ignore),
      )
    },
  render: self =>
    <div>
      <div
        className={SharedCss.appearAnimation(~direction=`normal, ~delayMs=0)}>
        <pre> {string(trackId ++ string_of_int(positionMs))} </pre>
        <User auth userName userIconUrl setLogOut />
        <CurrentlyPlaying
          songName
          artistName
          isPlaying
          progressPct
          albumImageUrl
        />
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
