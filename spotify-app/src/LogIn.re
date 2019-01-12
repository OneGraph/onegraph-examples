open Utils;
open Emotion;
open BsReactstrap;

type action =
  | HandleClick
  | SetLoggedIn(bool);

type state = {
  isLoggedIn: bool,
  auth: OneGraphAuth.auth,
};

let component = ReasonReact.reducerComponent("Login");

let make = _children => {
  ...component,
  initialState: () => {
    isLoggedIn: false,
    auth: OneGraphAuth.newAuth(OneGraphAuth.config),
  },
  didMount: self =>
    Js.Promise.(
      OneGraphAuth.(
        OneGraphAuth.isLoggedIn(self.state.auth, "gmail")
        |> then_(loginStatus => {
             Js.log(loginStatus);
             self.send(SetLoggedIn(loginStatus));
             resolve();
           })
        |> catch(err => resolve(Js.log(err)))
        |> ignore
      )
    ),
  reducer: (action, state) =>
    Js.Promise.(
      OneGraphAuth.(
        switch (action) {
        | HandleClick =>
          Js.log("Clicked Login!!");
          ReasonReact.SideEffects(
            (
              ({state: {auth}, send}) =>
                auth
                |> login(_, "gmail")
                |> then_(() => isLoggedIn(auth, "gmail"))
                |> then_(loginStatus => {
                     Js.log(loginStatus);
                     send(SetLoggedIn(loginStatus));
                     resolve();
                   })
                |> catch(err => resolve(Js.log(err)))
                |> ignore
            ),
          );
        | SetLoggedIn(isLoggedIn) =>
          ReasonReact.Update({...state, isLoggedIn})
        }
      )
    ),
  render: self =>
    ReasonReact.(
      <div>
        {string(self.state.isLoggedIn ? "Loged In!" : "Not in...")}
        <Button onClick={() => self.send(HandleClick)}>
          {string("Log In")}
        </Button>
      </div>
    ),
};
