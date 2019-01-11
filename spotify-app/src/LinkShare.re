open BsReactstrap;
open Emotion;
module Css = AppStyle;

type action =
  | Toggle;

type state = {isDropdownOpen: bool};

let linkSharing = [%css [marginBottom(`px(64))]];

let shareLinkURL = [%css
  [
    border(`px(0), `none, `hex("ffffff")),
    borderBottom(`px(1), `solid, `hex("cfcfcf")),
    padding2(`px(0), `px(8)),
    width(`px(280)),
  ]
];

let subInfo = [%css
  [
    fontSize(`px(14)),
    color(`hex("a2a2a2")),
    padding2(`px(0), `px(8)),
    select(
      "#stop-btn",
      [
        color(`hex("ff4e4e")),
        margin2(`px(0), `px(16)),
        textDecoration(`underline),
        select(":hover", [cursor(`pointer), color(Emotion.Css.Color.red)]),
      ],
    ),
  ]
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
      <div className=linkSharing>
        <p className=Css.pageSubTitle>
          {string("Share the following link to invite people to your music")}
        </p>
        <div
          style={
            ReactDOMRe.Style.make(~width="fit-content", ~margin="auto", ())
          }>
          <div className={Css.flexWrapper(~justify=`center, ~align=`flexEnd)}>
            <input
              className=shareLinkURL
              value="www.example.com/?userId"
              readOnly=true
            />
            <Dropdown
              isOpen={self.state.isDropdownOpen}
              toggle={() => self.send(Toggle)}
              size="sm">
              <DropdownToggle caret=true> {string("Share")} </DropdownToggle>
              <DropdownMenu>
                <DropdownItem> {string("Facebook")} </DropdownItem>
                <DropdownItem> {string("Twitter")} </DropdownItem>
                <DropdownItem> {string("Copy URL")} </DropdownItem>
              </DropdownMenu>
            </Dropdown>
          </div>
          <div
            className={
              Cn.make([
                Css.flexWrapper(~justify=`flexStart, ~align=`flexStart),
                subInfo,
              ])
            }>
            <p> <i> {string("240 Listners")} </i> </p>
            <a id="stop-btn"> {string("Stop Sharing")} </a>
          </div>
        </div>
      </div>
    ),
};
