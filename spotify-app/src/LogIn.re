open Utils;
open Emotion;
open BsReactstrap;

let spotifyLogo = requireAssetURI("./img/spotify-logo.png");

let spotifyBtn = [%css
  [
    display(`flex),
    alignItems(`center),
    margin4(`px(48), `auto, `px(32), `auto),
  ]
];

let component = ReasonReact.statelessComponent("Login");

let handleLogIn = (auth, logIn) =>
  Js.Promise.(
    OneGraphAuth.(
      auth
      |> login(_, "spotify")
      |> then_(() => isLoggedIn(auth, "spotify"))
      |> then_(isLoggedIn => {
           switch (isLoggedIn) {
           | false => ()
           | true => logIn()
           };
           resolve();
         })
      |> catch(err =>
           resolve(
             Js.Console.error2(
               "Error logging in to Spotify with OneGraph: ",
               err,
             ),
           )
         )
      |> ignore
    )
  );

let make = (~auth, ~onLogIn, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div>
        {string("Let's start the party.")}
        <Button
          className={Cn.make([SharedCss.button, spotifyBtn])}
          onClick={() => handleLogIn(auth, onLogIn)}>
          <img
            style={
              ReactDOMRe.Style.make(~width="24px", ~marginRight="8px", ())
            }
            src=spotifyLogo
            alt="Spotify Logo"
          />
          {string("Log in to your Spotify Account")}
        </Button>
      </div>
    ),
};
