open BsReactstrap;
open Emotion;
module Css = AppStyle;

type action =
  | Toggle;

type state = {isDropdownOpen: bool};

let linkSharing = [%css
  [
    width(`pct(50.)),
    backgroundColor(`hex("e0e0e0")),
    color(`hex("333333")),
    borderRadius(`px(5)),
    margin3(`px(0), `auto, `px(64)),
    boxShadow(
      ~inset=true,
      ~x=`px(2),
      ~y=`px(2),
      ~blur=`px(4),
      `hex("00000040"),
    ),
    padding2(`px(16), `px(0)),
  ]
];

let shareWrapper = [%css [position(`relative)]];

let shareLinkTitle = [%css [fontSize(`px(24)), marginBottom(`px(16))]];

let shareLinkURL = [%css
  [
    border(`px(0), `none, `hex("1DB954")),
    padding2(`px(0), `px(8)),
    width(`px(280)),
    backgroundColor(`hex("ffffff00")),
    outlineStyle(`none),
    color(`hex("7d7d7d")),
    marginBottom(`px(16)),
  ]
];

let shareBtn = [%css
  [
    backgroundColor(`hex("1DB954")),
    borderStyle(`none),
    marginBottom(`px(16)),
    boxShadow(~x=`px(0), ~y=`px(2), ~blur=`px(4), `hex("00000040")),
  ]
];

let inputLine = [%css
  [
    position(`absolute),
    width(`pct(100.)),
    border(`px(1), `solid, `hex("1DB95480")),
    bottom(`px(0)),
    width(`px(280)),
    transform(`translateX(`px(-33))),
    boxShadow(~x=`px(0), ~y=`px(2), ~blur=`px(4), `hex("00000040")),
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
        <p className=shareLinkTitle>
          {string("Share the following link to invite people to your music")}
        </p>
        /*<Button
            color={isPublic ? "danger" : "success"}
            size="sm"
            onClick={_e => toggleShareStatus()}>
            {
              isPublic ?
                ReasonReact.string("Stop Sharing") :
                ReasonReact.string("Share Publicaly")
            }
          </Button>*/
        <div
          className={
            Cn.make([
              Css.flexWrapper(~justify=`center, ~align=`flexEnd),
              shareWrapper,
            ])
          }>
          <input
            className=shareLinkURL
            value="www.example.com/?userId"
            readOnly=true
          />
          <hr className=inputLine />
          <Dropdown
            isOpen={self.state.isDropdownOpen}
            toggle={() => self.send(Toggle)}
            size="sm">
            <DropdownToggle className=shareBtn caret=true>
              {string("Share")}
            </DropdownToggle>
            <DropdownMenu>
              <DropdownItem> {string("Facebook")} </DropdownItem>
              <DropdownItem> {string("Twitter")} </DropdownItem>
              <DropdownItem> {string("Copy URL")} </DropdownItem>
            </DropdownMenu>
          </Dropdown>
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
