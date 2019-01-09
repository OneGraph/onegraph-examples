open Utils;
open BsReactstrap;

module Css = AppStyle;

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
      <div className=Css.linkSharing>
        <p className=Css.pageSubTitle>
          {string("Share the following link to invite people to your music")}
        </p>
        <div
          style={
            ReactDOMRe.Style.make(~width="fit-content", ~margin="auto", ())
          }>
          <div className={Css.flexWrapper(~justify=`center)}>
            <input
              className=Css.shareLinkURL
              value="www.example.com/?userId"
              readOnly=true
            />
            <Dropdown
              isOpen={self.state.isDropdownOpen}
              toggle={() => self.send(Toggle)}
              size="sm">
              <DropdownToggle caret=true> {string("Share")} </DropdownToggle>
              <DropdownMenu right=true>
                <DropdownItem> {string("Facebook")} </DropdownItem>
                <DropdownItem> {string("Twitter")} </DropdownItem>
                <DropdownItem> {string("Copy URL")} </DropdownItem>
              </DropdownMenu>
            </Dropdown>
          </div>
          <div
            className={
              Cn.make([Css.flexWrapper(~justify=`flexStart), Css.subInfo])
            }>
            <p> <i> {string("240 Listners")} </i> </p>
            <a id="stop-btn"> {string("Stop Sharing")} </a>
          </div>
        </div>
      </div>
    ),
};
