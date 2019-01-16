open Utils;
open Emotion;
open BsReactstrap;

let spotifyLogo = requireAssetURI("./img/spotify-logo.png");

let spotifyBtn = [%css
  [
    backgroundColor(`hex("1DB954")),
    borderStyle(`none),
    display(`flex),
    alignItems(`center),
    margin4(`px(48), `auto, `px(32), `auto),
    select("hover", [important(backgroundColor(`hex("1ED760")))]),
  ]
];

let component = ReasonReact.statelessComponent("Login");

let handleLogIn = (auth, setLogInStatus) => {
  open Js.Promise;
  open OneGraphAuth;

  Js.log("Clicked Login!!");
  auth
  |> login(_, "gmail")
  |> then_(() => isLoggedIn(auth, "gmail"))
  |> then_(loginStatus => {
       Js.log(loginStatus);
       setLogInStatus(loginStatus);
       resolve();
     })
  |> catch(err => resolve(Js.log(err)))
  |> ignore;
};

let make = (~auth, ~setLogInStatus, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div>
        <Button
          className=spotifyBtn
          onClick={() => handleLogIn(auth, setLogInStatus)}>
          <img
            style={
              ReactDOMRe.Style.make(~width="24px", ~marginRight="8px", ())
            }
            src=spotifyLogo
            alt="Spotify Logo"
          />
          {string("Log In to Spotify Account")}
        </Button>
      </div>
    ),
};
