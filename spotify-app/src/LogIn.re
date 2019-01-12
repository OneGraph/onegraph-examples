open Utils;
open Emotion;
open BsReactstrap;

type action =
  | HandleClick;

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
                     resolve();
                   })
                |> catch(err => resolve(Js.log(err)))
                |> ignore
            ),
          );
        }
      )
    ),
  render: self =>
    ReasonReact.(
      <div>
        <Button onClick={() => self.send(HandleClick)}>
          {string("Log In")}
        </Button>
      </div>
    ),
};
