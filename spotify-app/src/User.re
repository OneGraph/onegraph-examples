open Utils;
open BsReactstrap;

module Css = AppStyle;
let userIcon = requireAssetURI("./img/user.png");

type action =
  | Toggle;

type state = {isDropdownOpen: bool};

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
      <header className=Css.flexWrapperRightAlign>
        <div style={ReactDOMRe.Style.make(~width="100px", ())}>
          <Dropdown
            isOpen={self.state.isDropdownOpen}
            toggle={() => self.send(Toggle)}>
            <DropdownToggle
              caret=true
              tag="div"
              className={
                Cn.make([Css.flexWrapperRightAlign, Css.userAccountWrapper])
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
