type playerStatus = {
  isPlaying: bool,
  trackId: string,
  positionMs: int,
};

let pausePlayerQuery = {|
mutation {
  spotify {
    pausePlayer {
      player {
        isPlaying
      }
    }
  }
   }|};

let startPlayerQuery = {|
mutation startPlayer($trackId: String!, $positionMs: Int!) {
  spotify {
    playTrack(input: { trackIds: [$trackId], positionMs: $positionMs}) {
      player {
        isPlaying
      }
    }
  }
}|};

let makeHeaders = token => {
  "content-type": "application/json",
  "accept": "application/json",
  "authentication": {j|Bearer $token|j},
  "show_beta_schema": "true",
};

let startPlayer = (token, playerStatus) => {
  open Js;
  open OneGraphFetch;
  let vars = Dict.empty();
  Dict.set(vars, "trackId", Json.string(playerStatus.trackId));
  Dict.set(
    vars,
    "positionMs",
    playerStatus.positionMs |> float_of_int |> Json.number,
  );

  let payload =
    makePayload(~query=startPlayerQuery, ~variables=Json.object_(vars), ());

  fetch(token, payload);
};

let pausePlayer = token => {
  open OneGraphFetch;
  let payload = makePayload(~query=pausePlayerQuery, ());

  fetch(token, payload);
};
