open Utils;
open Emotion;
open BsReactstrap;

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
        <Button onClick={() => handleLogIn(auth, setLogInStatus)}>
          {string("Log In")}
        </Button>
      </div>
    ),
};
