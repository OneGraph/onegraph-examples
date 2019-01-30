type peer;
type dataConnection;
type data;
type playerStatus = {
  isPlaying: bool,
  trackId: string,
  positionMs: int,
};

type id = string;

ignore("peerjs/dist/peer.js");

[@bs.deriving abstract]
type urlObj = {url: string};

[@bs.deriving abstract]
type config = {iceServers: option(array(urlObj))};

[@bs.deriving abstract]
type options = {
  [@bs.optional]
  key: string,
  [@bs.optional]
  host: string,
  [@bs.optional]
  port: int,
  [@bs.optional]
  path: string,
  [@bs.optional]
  secure: bool,
  [@bs.optional]
  config,
  [@bs.optional]
  debug: int,
};

[@bs.new] [@bs.scope "window"]
external newPeer: (BsUuid.Uuid.V4.t, options) => peer = "Peer";

[@bs.send] external peerConnect: (peer, id) => dataConnection = "connect";
[@bs.send]
external peerOn: (peer, string, dataConnection => unit) => unit = "on";
[@bs.send] external peerDisconnect: peer => unit = "disconnect";

[@bs.send]
external connOn: (dataConnection, string, playerStatus => unit) => unit = "on";

[@bs.send]
external connSendId: (dataConnection, BsUuid.Uuid.V4.t) => unit = "send";

[@bs.send]
external connSendTrack: (dataConnection, playerStatus) => unit = "send";

[@bs.send] external connSend: (dataConnection, string) => unit = "send";

let controlPlayer = (token, playerStatus) => {
  let startPlayQuery =
    "{\"query\":\"mutation startPlay(\\n  $trackId: String!\\n  $positionMs: Int!\\n) {\\n  spotify {\\n    playTrack(\\n      input: { trackIds: $trackId, positionMs: $positionMs }\\n    ) {\\n      player {\\n        isPlaying\\n      }\\n    }\\n  }\\n}\\n\",\"variables\":{\"trackId\":\""
    ++ playerStatus.trackId
    ++ "\",\"positionMs\":"
    ++ string_of_int(playerStatus.positionMs)
    ++ "},\"operationName\":\"startPlay\"}";

  let pauseQuery = "{\"query\":\"mutation pause {\\n  spotify {\\n    pausePlayer {\\n      player {\\n        isPlaying\\n      }\\n    }\\n  }\\n}\\n\",\"variables\":null,\"operationName\":\"pause\"}";

  Js.Promise.(
    Fetch.fetchWithInit(
      "https://serve.onegraph.com/dynamic?app_id="
      ++ OneGraphAuth.appId
      ++ "&show_metrics=true",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(
            playerStatus.isPlaying ? startPlayQuery : pauseQuery,
          ),
        ~headers=
          Fetch.HeadersInit.make({
            "accept": "application/json",
            "accept-language": "en-US,en;q=0.9,ja;q=0.8,zh-CN;q=0.7,zh;q=0.6",
            "authentication": "Bearer " ++ token,
            "content-type": "application/json",
            "show_beta_schema": "false",
          }),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
    |> ignore
  );
};

let openConnection = (peer, requesterId, djId, auth) => {
  let conn = peerConnect(peer, djId);
  connOn(conn, "open", _data => connSendId(conn, requesterId));
  connOn(
    conn,
    "data",
    data => {
      Js.log2("Got some data from a peer:", data);
      let token =
        switch (OneGraphAuth.authToken(auth)) {
        | Some(token) => token
        | None => ""
        };
      controlPlayer(token, data);
    },
  );
  connOn(conn, "error", err => Js.log2("PeerJS error", err));
  connOn(conn, "close", _data => Js.log("Connection Lost"));
  conn;
};

type connectionsMap = Js.Dict.t(array(dataConnection));

[@bs.get] external connections: peer => connectionsMap = "";

let broadcast = (peer, playerStatus) => {
  let conns = connections(peer);
  conns
  |> Js.Dict.keys
  |> Array.map(key => Js.Dict.unsafeGet(conns, key))
  |> Array.iter(peerConns =>
       peerConns
       |> Array.iter(peerConn => connSendTrack(peerConn, playerStatus))
     );
  Js.log2("Broadcast conns", conns);
};
