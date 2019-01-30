open PeerJsBinding;
open ReasonReact;
open Belt.Option;

type userKind =
  | DJ
  | Listener(string);

type state = {
  peer: option(PeerJsBinding.peer),
  peerId: option(BsUuid.Uuid.V4.t),
  userKind,
  isPublic: bool,
};

type action =
  | SetPeer(peer, BsUuid.Uuid.V4.t)
  | ToggleShareStatus;

let component = ReasonReact.reducerComponent("App");

let renderIf = (condition: bool, element: ReasonReact.reactElement) =>
  condition ? element : ReasonReact.null;

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
    peer: None,
    peerId: None,
    userKind: DJ,
    isPublic: true,
  },
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

    let myPeer = newPeer(peerId, options);
    self.send(SetPeer(myPeer, peerId));

    switch (userKind) {
    | DJ =>
      Js.log("I'm a DJ");
      let intervalId =
        Js.Global.setInterval(
          () =>
            PeerJsBinding.broadcast(
              myPeer,
              {isPlaying, trackId, positionMs},
            ),
          1000,
        );
      self.onUnmount(() => Js.Global.clearInterval(intervalId));
    | Listener(djId) =>
      let connection = openConnection(myPeer, peerId, djId, auth);
      Js.log3("I'm a listener to with connection:", djId, connection);
    };
  },
  reducer: (action, state) =>
    switch (action) {
    | SetPeer(peer, peerId) =>
      ReasonReact.Update({...state, peer: Some(peer), peerId: Some(peerId)})
    | ToggleShareStatus =>
      ReasonReact.Update({...state, isPublic: !state.isPublic})
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
