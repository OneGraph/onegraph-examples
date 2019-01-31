open ReasonReact;

type userRole =
  | DJ
  | Listener(string);

type rtc = {
  switchboard: option(PeerJs.switchboard),
  id: option(BsUuid.Uuid.V4.t),
};

type state = {
  userRole,
  rtc,
};

type action =
  | NotifyListeners
  | ExamineDJState(SpotifyControls.playerStatus)
  | PausePlayer
  | SyncPlayer(SpotifyControls.playerStatus)
  | InitiateSwitchboard(PeerJs.switchboard, BsUuid.Uuid.V4.t);

let component = ReasonReact.reducerComponent("CurrentlyPlayingContainer");

let msDiffThreshold = 10000;

let semiDurableId = () => {
  let maybePeerId = Utils.sessionStorageGetItem("spotDjPeerId");
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
};

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
      ~onLogOut,
      _children,
    ) => {
  ...component,
  initialState: () => {
    userRole: DJ,
    rtc: {
      switchboard: None,
      id: None,
    },
  },
  didMount: self => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    let query = QueryString.parseQueryString(url.search);

    let userRole =
      switch (Js.Dict.get(query, "dj")) {
      | Some(Single(djId)) => Listener(djId)
      | _ => DJ
      };

    let rtcId = semiDurableId();

    let me = PeerJs.newSwitchBoard(rtcId);

    self.send(InitiateSwitchboard(me, rtcId));

    let onData = data => {
      Js.log2("Received data as listener: ", data);
      switch (Serialize.messageOfString(data)) {
      | DjPlayerState(playerStatus) =>
        self.send(ExamineDJState(playerStatus))
      | Unrecognized(raw) => Js.log2("Unrecognized message from peer: ", raw)
      | Malformed(raw) => Js.log2("Malformed message from peer: ", raw)
      };
    };

    switch (userRole) {
    | DJ =>
      Js.log("Starting in DJ mode");
      let intervalId =
        Js.Global.setInterval(() => self.send(NotifyListeners), 1000);
      self.onUnmount(() => Js.Global.clearInterval(intervalId));
    | Listener(djId) =>
      let connection = PeerJs.connect(~me, ~toPeerId=djId, ~onData, ());
      Js.log(
        {j|Starting in Listener mode. My switchboard $me has a connection $connection to dj $djId|j},
      );
    };
  },
  reducer: (action, state) =>
    switch (action) {
    /* DJ actions */
    | NotifyListeners =>
      SideEffects(
        (
          ({state}) =>
            switch (state.rtc.switchboard) {
            | None => ()
            | Some(switchboard) =>
              let serializedMessage =
                Serialize.stringOfMessage(
                  DjPlayerState({isPlaying, trackId, positionMs}),
                );
              PeerJs.broadcast(switchboard, serializedMessage);
            }
        ),
      )

    /* Listener actions */
    | InitiateSwitchboard(switchboard, id) =>
      let rtc = {switchboard: Some(switchboard), id: Some(id)};
      Update({...state, rtc});
    | ExamineDJState(dj) =>
      /* Determine if we should run a OneGraph Spotify mutation to sync players,
         and if so, which mutation */
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
    },
  render: self =>
    <div>
      <div
        className={SharedCss.appearAnimation(~direction=`normal, ~delayMs=0)}>
        <pre> {string({j|Track: `$trackId` @ $positionMs ms|j})} </pre>
        <User auth userName userIconUrl onLogOut />
        <CurrentlyPlaying
          songName
          artistName
          isPlaying
          progressPct
          albumImageUrl
        />
        <LinkShare
          rtcId={
            switch (self.state.rtc.id) {
            | Some(rtcId) => BsUuid.Uuid.V4.toString(rtcId)
            | None => ""
            }
          }
        />
      </div>
    </div>,
};
