type peer;
type dataConnection;
type data;
type trackData = {
  trackId: string,
  positionMs: option(int),
};
type id = string;

[@bs.new] [@bs.module "peerjs"] external newPeer: unit => peer = "default";

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

let openConnection = (peer, requesterId, djId) => {
  let conn = peerConnect(peer, djId);
  connOn(conn, "open", _data => connSendId(conn, requesterId));
  connOn(conn, "error", err => Js.log(err));
  connOn(conn, "close", _data => Js.log("Connection Lost"));
};

let onRecieveConnRequest = (peer, trackId, positionMs) =>
  peerOn(
    peer,
    "connection",
    conn => {
      connOn(conn, "data", data => Js.log(data));
      connSendTrack(conn, {trackId, positionMs});
    },
  );
