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

let make = (~isPublic, ~toggleShareStatus, _children) => {
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
        <Button
          color={isPublic ? "danger" : "success"}
          size="sm"
          onClick={_e => toggleShareStatus()}>
          {
            isPublic ?
              ReasonReact.string("Stop Sharing") :
              ReasonReact.string("Share Publicaly")
          }
        </Button>
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
          </div>
        </div>
      </div>
    ),
};

/*
 let switchBtn = [%css
   [
     position(`relative),
     display(`inlineBlock),
     width(`px(85)),
     height(`px(25)),
     select("input", [display(`none)]),
   ]
 ];

 let sliderRound = [%css
   [
     position(`absolute),
     cursor(`pointer),
     top(`px(0)),
     left(`px(0)),
     right(`px(0)),
     bottom(`px(0)),
     backgroundColor(`hex("afafaf")),
     textAlign(`center),
     lineHeight(`px(25)),
     fontSize(`px(14)),
     color(`hex("ffffff")),
     padding2(`px(0), `px(0)),
     transitionDuration(`s(0.4)),
     borderRadius(`px(34)),
     paddingLeft(`px(20)),
     select(
       "before",
       [
         position(`absolute),
         content(""),
         height(`px(17)),
         width(`px(17)),
         left(`px(4)),
         bottom(`px(4)),
         backgroundColor(`hex("ffffff")),
         transitionDuration(`s(0.4)),
         borderRadius(`pct(50.)),
         select("input:checked", [transform(`translateX(`px(60)))]),
       ],
     ),
     select(
       "input:checked",
       [backgroundColor(`hex("2196f3")), paddingRight(`px(30))],
     ),
     select(
       "input:focus",
       [boxShadow(~x=`px(0), ~y=`px(0), ~blur=`px(1), `hex("2196f3"))],
     ),
   ]
 ];*/
