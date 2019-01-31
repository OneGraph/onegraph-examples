let appId = OneGraphAuth.appId;

let oneGraphUri = {j|https://serve.onegraph.com/dynamic?app_id=$appId&show_metrics=true|j};

let makeHeaders = token => {
  "content-type": "application/json",
  "accept": "application/json",
  "authentication": {j|Bearer $token|j},
  "show_beta_schema": "true",
};

let fetch = (token, body) =>
  Fetch.(
    fetchWithInit(
      oneGraphUri,
      RequestInit.make(
        ~method_=Post,
        ~body=body |> Js.Json.stringify |> BodyInit.make,
        ~headers=HeadersInit.make(makeHeaders(token)),
        (),
      ),
    )
    |> Js.Promise.then_(Response.json)
  );

let makePayload = (~operationName=?, ~variables=?, ~query, ()) => {
  open Js;

  let json = Dict.empty();
  Dict.set(json, "query", Json.string(query));
  Dict.set(
    json,
    "variables",
    variables->Belt.Option.getWithDefault(Json.null),
  );
  Dict.set(
    json,
    "operationName",
    switch (operationName) {
    | None => Js.Json.null
    | Some(operationName) => Json.string(operationName)
    },
  );

  Json.object_(json);
};
