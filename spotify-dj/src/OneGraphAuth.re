[@bs.deriving abstract]
type config = {appId: string};

[@bs.deriving abstract]
type authHeader = {
  [@bs.optional] [@bs.as "Authentication"]
  authenticationHeader: string,
};

type auth;

let appId = "bafd4254-c229-48c2-8c53-44a01477a43e";

[@bs.new] [@bs.module "onegraph-auth"]
external newAuth: config => auth = "default";

[@bs.send]
external isLoggedIn: (auth, string) => Js.Promise.t(bool) = "isLoggedIn";
[@bs.send] external login: (auth, string) => Js.Promise.t(unit) = "login";
[@bs.send] external logout: (auth, string) => Js.Promise.t(unit) = "logout";
[@bs.send] external authHeaders: auth => authHeader = "authHeaders";

let authToken = (auth: auth): option(string) =>
  switch (auth |> authHeaders |> authenticationHeaderGet) {
  | None => None
  | Some(authHeader) => Some(Js.String.replace("Bearer ", "", authHeader))
  };

let dangerouslyGetAuthToken = (auth: auth): string =>
  switch (authToken(auth)) {
  | None =>
    raise(
      Failure("OneGraph auth token not present in dangerouslyGetAuthToken"),
    )
  | Some(token) => token
  };

let config = config(~appId);
