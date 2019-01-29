open PeerJsBinding;
open ReasonReact;
open Belt.Option;

type userKind =
  | DJ
  | Listener(string);

type state = {
  peer: option(PeerJsBinding.peer),
  peerId: BsUuid.Uuid.V4.t,
  userKind,
  isPublic: bool,
};

type action =
  | SetPeer(peer)
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
      _children,
    ) => {
  ...component,
  initialState: () => {
    peer: None,
    peerId: BsUuid.Uuid.V4.create(),
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

    let myPeer = newPeer(self.state.peerId, options);
    Js.log2("myPeer", myPeer);

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
      let connection = openConnection(myPeer, self.state.peerId, djId, auth);
      Js.log3("I'm a listener to with connection:", djId, connection);
      self.send(SetPeer(myPeer));
    };
  },
  reducer: (action, state) =>
    switch (action) {
    | SetPeer(peer) => ReasonReact.Update({...state, peer: Some(peer)})
    | ToggleShareStatus =>
      ReasonReact.Update({...state, isPublic: !state.isPublic})
    },
  render: self =>
    <div>
      <div
        className={SharedCss.appearAnimation(~direction=`normal, ~delayMs=0)}>
        <pre> {string(trackId ++ string_of_int(positionMs))} </pre>
        <User auth userName userIconUrl />
        <CurrentlyPlaying
          songName
          artistName
          isPlaying
          progressPct
          albumImageUrl
        />
        <LinkShare
          isPublic={self.state.isPublic}
          toggleShareStatus={() => self.send(ToggleShareStatus)}
          peerId={self.state.peerId}
        />
      </div>
    </div>,
};
