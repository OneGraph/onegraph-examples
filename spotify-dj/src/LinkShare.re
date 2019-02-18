open Utils;
open BsReactstrap;
open Emotion;

let facebookIcon = requireAssetURI("./img/facebook-icon.png");
let twitterIcon = requireAssetURI("./img/twitter-icon.png");
let linkIcon = requireAssetURI("./img/link-icon.png");

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

let drowpdownMenuStyle = [%css
  [backgroundColor(`hex("047F30")), color(`hex("ffffff"))]
];

let drowpdownItemStyle = [%css
  [
    backgroundColor(`hex("047F30")),
    color(`hex("ffffff")),
    select(
      ":hover",
      [backgroundColor(`hex("00521D")), color(`hex("ffffff"))],
    ),
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

let copyUrlToClipboard = url =>
  try (
    Utils.writeText(url)
    |> Js.Promise.then_(_e => Js.Promise.resolve())
    |> ignore
  ) {
  | _ =>
    selectElement(getElementById("shareLinkURL"), ());
    let success = execCommand("copy");
    Js.log2("Success copying to clipboard: ", success);
  };

let shareSocialMedia = (url, name, properties) =>
  Utils.windowOpen(url, name, properties);

let component = ReasonReact.reducerComponent("User");

let sharingLink = djId => Utils.Window.({j|$protocol//$host?dj=$djId|j});

let localhostRegex = [%re "/https?:\/\/localhost/ig"];

let isLocalhost = target => localhostRegex |> Js.Re.test(target);

let make = (~peerId, _children) => {
  ...component,
  initialState: () => {isDropdownOpen: false},
  reducer: (action, state) =>
    switch (action) {
    | Toggle => ReasonReact.Update({isDropdownOpen: !state.isDropdownOpen})
    },
  render: self => {
    open ReasonReact;
    let sharingLink = sharingLink(peerId);

    <div className=linkSharing>
      <p className=shareLinkTitle>
        {string("Share the following link to invite people to your music")}
      </p>
      <div
        className={
          Cn.make([
            SharedCss.flexWrapper(~justify=`center, ~align=`flexEnd),
            shareWrapper,
          ])
        }>
        <input
          className=shareLinkURL
          value=sharingLink
          readOnly=true
          id="shareLinkURL"
        />
        <hr className=inputLine />
        <Dropdown
          isOpen={self.state.isDropdownOpen}
          toggle={() => self.send(Toggle)}
          size="sm">
          <DropdownToggle className=SharedCss.button caret=true>
            {string("Share")}
          </DropdownToggle>
          <DropdownMenu className=drowpdownMenuStyle>
            <DropdownItem
              className=drowpdownItemStyle
              onClick={
                _e =>
                  shareSocialMedia(
                    "https://www.facebook.com/sharer/sharer.php?u="
                    ++ (
                      /* Detect localhost urls because facebook only allows sharing public links */
                      isLocalhost(sharingLink) ?
                        "https://spotdj.onegraphapp.com/" ++ peerId :
                        sharingLink
                    ),
                    "Share SpotDJ Channel",
                    "menubar=1,resizable=1,width=560,height=450",
                  )
              }>
              <img
                className=SharedCss.icon
                src=facebookIcon
                alt="Facebook Icon"
              />
              {string("Facebook")}
            </DropdownItem>
            <DropdownItem
              className=drowpdownItemStyle
              onClick={
                _e =>
                  shareSocialMedia(
                    "https://twitter.com/intent/tweet?text=Join%20me%20on%20SpotDj%20Here:%20"
                    ++ sharingLink,
                    "Share SpotDJ Channel",
                    "menubar=1,resizable=1,width=350,height=250",
                  )
              }>
              <img
                className=SharedCss.icon
                src=twitterIcon
                alt="Twitter Icon"
              />
              {string("Twitter")}
            </DropdownItem>
            <DropdownItem
              className=drowpdownItemStyle
              onClick={_e => copyUrlToClipboard(sharingLink)}>
              <img className=SharedCss.icon src=linkIcon alt="Link Icon" />
              {string("Copy URL")}
            </DropdownItem>
          </DropdownMenu>
        </Dropdown>
      </div>
    </div>;
  },
};

/*
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
