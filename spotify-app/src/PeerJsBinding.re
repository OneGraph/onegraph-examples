type peer;
type dataConnection;
type data;
type trackData = {
  trackId: string,
  positionMs: option(int),
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
external newPeer: (Js.Nullable.t(string), options) => peer = "Peer";

[@bs.send] external peerConnect: (peer, id) => dataConnection = "connect";
[@bs.send]
external peerOn: (peer, string, dataConnection => unit) => unit = "on";
[@bs.send] external peerDisconnect: peer => unit = "disconnect";

[@bs.send]
external connOn: (dataConnection, string, data => unit) => unit = "on";

[@bs.send]
external connSendId: (dataConnection, BsUuid.Uuid.V4.t) => unit = "send";

[@bs.send]
external connSendTrack: (dataConnection, trackData) => unit = "send";

[@bs.send] external connSend: (dataConnection, string) => unit = "send";

let openConnection = (peer, requesterId, djId) => {
  let conn = peerConnect(peer, djId);
  connOn(conn, "open", _data => connSendId(conn, requesterId));
  connOn(conn, "data", data => Js.log2("Got some data from a peer:", data));
  connOn(conn, "error", err => Js.log2("PeerJS error", err));
  connOn(conn, "close", _data => Js.log("Connection Lost"));
  conn;
};

let onReceiveConnRequest = (peer, trackId, positionMs) =>
  peerOn(
    peer,
    "connection",
    conn => {
      connOn(conn, "data", data =>
        Js.log2("Got some data from a peer:", data)
      );
      connSendTrack(conn, {trackId, positionMs});
    },
  );

type connectionsMap = Js.Dict.t(array(dataConnection));

[@bs.get] external connections: peer => connectionsMap = "";

let broadcast = (peer, message) => {
  let conns = connections(peer);
  conns
  |> Js.Dict.keys
  |> Array.map(key => Js.Dict.unsafeGet(conns, key))
  |> Array.iter(peerConns =>
       peerConns |> Array.iter(peerConn => connSend(peerConn, message))
     );
  Js.log2("Broadcast conns", conns);
};