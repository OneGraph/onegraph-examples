open Utils;
open Emotion;
open BsReactstrap;

module Css = AppStyle;
let userIcon = requireAssetURI("./img/user.png");

type action =
  | HandleLogOut
  | Toggle;

type state = {isDropdownOpen: bool};

/*Style*/
let userAccountWrapper = [%css
  [padding4(`px(16), `px(0), `px(0), `px(24)), marginRight(`px(48))]
];

let component = ReasonReact.reducerComponent("User");

let make = (~auth, ~setLogInStatus, ~userName, _children) => {
  ...component,
  initialState: () => {isDropdownOpen: false},
  reducer: (action, state) =>
    Js.Promise.(
      OneGraphAuth.(
        switch (action) {
        | HandleLogOut =>
          Js.log("Clicked LogOut!!");
          ReasonReact.SideEffects(
            (
              _state =>
                auth
                |> logout(_, "spotify")
                |> then_(() => isLoggedIn(auth, "spotify"))
                |> then_(loginStatus => {
                     Js.log(loginStatus);
                     setLogInStatus(loginStatus);
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
      <header className={Css.flexWrapper(~justify=`flexEnd, ~align=`center)}>
        <div style={ReactDOMRe.Style.make(~width="100px", ())}>
          <Dropdown
            isOpen={self.state.isDropdownOpen}
            toggle={() => self.send(Toggle)}>
            <DropdownToggle
              caret=true
              tag="div"
              className={
                Cn.make([
                  Css.flexWrapper(~justify=`flexEnd, ~align=`center),
                  userAccountWrapper,
                ])
              }>
              <img className=Css.userIcon src=userIcon alt="user icon" />
              <p
                style={
                  ReactDOMRe.Style.make(~margin="0px", ~flex="0 0 auto", ())
                }>
                {ReasonReact.string(userName)}
              </p>
            </DropdownToggle>
            <DropdownMenu right=true>
              <DropdownItem onClick={() => self.send(HandleLogOut)}>
                {string("Sign Out")}
              </DropdownItem>
            </DropdownMenu>
          </Dropdown>
        </div>
      </header>
    ),
};
