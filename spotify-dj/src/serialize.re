let version = 1.0;

type message =
  | DjPlayerState(SpotifyControls.playerStatus)
  | Unrecognized(string)
  | Malformed(string);

let stringOfMessage: message => string =
  message => {
    open Js;
    let envelope = Dict.empty();
    let json = Dict.empty();

    Dict.set(envelope, "version", Json.number(version));

    switch (message) {
    | DjPlayerState(playerStatus) =>
      Dict.set(envelope, "type", Json.string("djPlayerState"));

      Dict.set(json, "trackId", Json.string(playerStatus.trackId));
      Dict.set(
        json,
        "positionMs",
        Json.number(playerStatus.positionMs |> float_of_int),
      );

      Dict.set(json, "isPlaying", Json.boolean(playerStatus.isPlaying));

    | Unrecognized(string) =>
      Dict.set(envelope, "type", Json.string("unrecognized"));

      Dict.set(json, "raw", Json.string(string));

    | Malformed(string) =>
      Dict.set(envelope, "type", Json.string("malformed"));

      Dict.set(json, "raw", Json.string(string));
    };

    Dict.set(envelope, "payload", Json.object_(json));

    envelope |> Json.object_ |> Json.stringify;
  };

let messageOfString: string => message =
  jsonString =>
    try (
      Js.(
        Js.Json.(
          Belt.Option.(
            switch (parseExn(jsonString) |> decodeObject) {
            | None => raise(Failure("Not an object"))
            | Some(json) =>
              let messageVersion =
                Dict.get(json, "version")->getExn->decodeNumber->getExn;
              let typ = Dict.get(json, "type")->getExn->decodeString->getExn;

              switch (messageVersion > version, typ) {
              | (true, _) => raise(Failure("Newer message version"))
              | (_, "djPlayerState") =>
                let payload =
                  json->Dict.get("payload")->getExn->decodeObject->getExn;

                let trackId =
                  Dict.get(payload, "trackId")->getExn->decodeString->getExn;

                let isPlaying =
                  Dict.get(payload, "isPlaying")
                  ->getExn
                  ->decodeBoolean
                  ->getExn;

                let positionMs =
                  Dict.get(payload, "positionMs")
                  ->getExn
                  ->decodeNumber
                  ->getExn
                  ->int_of_float;

                DjPlayerState({isPlaying, trackId, positionMs});
              | (_, _other) => Unrecognized(jsonString)
              };
            }
          )
        )
      )
    ) {
    | Failure(msg) =>
      Js.log2("Malformed exception: ", msg);
      Malformed(jsonString);
    | exn_ =>
      Js.log2("Really, really malformed exception: ", exn_);
      Malformed(jsonString);
    };
