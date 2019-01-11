open Utils;
open Emotion;
open BsReactstrap;

module Css = AppStyle;
let userIcon = requireAssetURI("./img/user.png");

type action =
  | Toggle;

type state = {isDropdownOpen: bool};

/*Style*/
let userAccountWrapper = [%css
  [padding4(`px(16), `px(24), `px(0), `px(24))]
];

let component = ReasonReact.reducerComponent("User");

let make = _children => {
  ...component,
  initialState: () => {isDropdownOpen: false},
  reducer: (action, state) =>
    switch (action) {
    | Toggle => ReasonReact.Update({isDropdownOpen: !state.isDropdownOpen})
    },
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
              <p style={ReactDOMRe.Style.make(~margin="0px", ())}>
                {ReasonReact.string("userABC")}
              </p>
            </DropdownToggle>
            <DropdownMenu right=true>
              <DropdownItem> {string("Sign Out")} </DropdownItem>
            </DropdownMenu>
          </Dropdown>
        </div>
      </header>
    ),
};
