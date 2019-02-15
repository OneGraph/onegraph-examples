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

module Uuid = {
  include BsUuid.Uuid.V4;

  external ofString: string => t = "%identity";
};

type switchboard;

module Impl = {
  type dataConnection;
  type data;
  type id = string;

  [@bs.send]
  external connectToPeer: (switchboard, id) => dataConnection = "connect";

  [@bs.send]
  external switchboardOn:
    (
      switchboard,
      [@bs.string] [
        | [@bs.as "open"] `Open
        | [@bs.as "close"] `Close
        | [@bs.as "error"] `Error
        | [@bs.as "connection"] `Connection
        | [@bs.as "call"] `Call
        | [@bs.as "disconnected"] `Disconnected
      ],
      dataConnection => unit
    ) =>
    unit =
    "on";

  [@bs.send] external peerDisconnect: switchboard => unit = "disconnect";

  [@bs.send]
  external connOn:
    (
      dataConnection,
      [@bs.string] [
        | [@bs.as "data"] `Data
        | [@bs.as "open"] `Open
        | [@bs.as "close"] `Close
        | [@bs.as "error"] `Error
      ],
      string => unit
    ) =>
    unit =
    "on";

  /* [@bs.send] */
  /* external connSendId: (dataConnection, BsUuid.Uuid.V4.t) => unit = "send"; */

  /* [@bs.send] */
  /* external connSendTrack: (dataConnection, playerStatus) => unit = "send"; */

  [@bs.send] external connSend: (dataConnection, string) => unit = "send";

  [@bs.get] external localId: switchboard => option(string) = "id";

  [@bs.get] external connIsOpen: dataConnection => bool = "open";

  [@bs.get] external disconnected: switchboard => bool = "";

  type connectionsMap = Js.Dict.t(array(dataConnection));

  [@bs.get] external connections: switchboard => connectionsMap = "";
};

[@bs.new] [@bs.scope "window"]
external newSwitchBoard: Uuid.t => switchboard = "Peer";

[@bs.new] [@bs.scope "window"]
external newSwitchBoardWithOptions: (Uuid.t, options) => switchboard = "Peer";

open Impl;

let connect =
    (
      ~onOpen: option(string => unit)=?,
      ~onClose: option(string => unit)=?,
      ~onError: option(string => unit)=?,
      ~onConnecting: option(unit => unit)=?,
      ~me,
      ~toPeerId,
      ~onData,
      (),
    ) => {
  switch (onConnecting) {
  | None => ()
  | Some(onConnecting) => onConnecting()
  };

  let _localId = me |> localId |> Belt.Option.map(_, Uuid.ofString);

  let conn = connectToPeer(me, toPeerId);
  switch (onOpen) {
  | None => ()
  | Some(onOpen) => connOn(conn, `Open, onOpen)
  };

  connOn(conn, `Data, onData);
  connOn(
    conn,
    `Error,
    Belt.Option.getWithDefault(onError, err => Js.log2("PeerJS error", err)),
  );
  connOn(
    conn,
    `Close,
    Belt.Option.getWithDefault(onClose, data =>
      Js.log2("Connection to peer Lost", data)
    ),
  );

  conn;
};

let getConnectedPeerCount = peer => {
  let conns = connections(peer);
  let activeConn =
    conns
    |> Js.Dict.keys
    |> Array.map(key => {
         let connection = Js.Dict.unsafeGet(conns, key);
         let latestConnInfo = connection[Array.length(connection) - 1];
         /*Js.Dict.unsafeGet(latestConnInfo, "open");*/
         connIsOpen(latestConnInfo) ? Some(latestConnInfo) : None;
       })
    |> Array.to_list
    |> List.filter(conn =>
         switch (conn) {
         | Some(_conn) => true
         | None => false
         }
       )
    |> Array.of_list;

  let connectedCount = activeConn |> Array.length;
  connectedCount;
};

let broadcast = (peer, data) => {
  let conns = connections(peer);

  conns
  |> Js.Dict.keys
  |> Array.map(key => Js.Dict.unsafeGet(conns, key))
  |> Array.iter(peerConns =>
       peerConns |> Array.iter(peerConn => connSend(peerConn, data))
     );
  Js.log2("Broadcast conns", conns);
};
