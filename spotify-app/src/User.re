open Utils;
open Emotion;

let userDefaultIcon = requireAssetURI("./img/user.png");

type action =
  | HandleLogOut
  | Toggle;

type state = {isDropdownOpen: bool};

/*Style*/
let userIcon = [%css [width(`px(25)), marginRight(`em(0.25))]];
let anchor = [%css [cursor(`pointer)]];
let separator = [%css [
  width(`em(1.)),
  backgroundColor(`hex("fff")),
  height(`px(1)),
  verticalAlign(`middle),
  margin4(`zero, `em(0.5), `zero, `em(0.5)),
  display(`inlineBlock),
]];

let component = ReasonReact.reducerComponent("User");

let make = (~auth, ~logOut, ~userName, _children) => {
  ...component,
  initialState: () => {isDropdownOpen: false},
  reducer: (action, state) =>
    Js.Promise.(
      OneGraphAuth.(
        switch (action) {
        | HandleLogOut =>
          ReasonReact.SideEffects(
            (
              _state =>
                auth
                |> logout(_, "spotify")
                |> then_(() => isLoggedIn(auth, "spotify"))
                |> then_(isLoggedIn => {
                     if(!isLoggedIn) {
                       logOut();
                     }
                     resolve();
                   })
                |> catch(err => resolve(Js.log(err)))
                |> ignore
            ),
          );
        | Toggle =>
          ReasonReact.Update({isDropdownOpen: !state.isDropdownOpen})
        }
      )
    ),
  render: self =>
    ReasonReact.(
          <p>
            {string("Listening as ")}
            <img className=userIcon src=userDefaultIcon alt={"user icon"} />
            {string(userName)}
            <span className=separator />
            <a
              className=anchor
              onClick={(_e) => self.send(HandleLogOut)}
            >
              {string("Sign Out")}
            </a>
          </p>
    ),
};
