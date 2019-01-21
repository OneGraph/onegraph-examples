open Emotion;

let flexWrapper = (~justify, ~align) => [%css
  [display(`flex), justifyContent(justify), alignItems(align)]
];

let button = [%css
  [
    backgroundColor(`hex("1DB954")),
    borderStyle(`none),
    marginBottom(`px(16)),
    boxShadow(~x=`px(0), ~y=`px(2), ~blur=`px(4), `hex("00000040")),
    select(":hover", [backgroundColor(`hex("18a94b"))]),
    select(":active", [important(backgroundColor(`hex("18a94b")))]),
    select(":focus", [important(backgroundColor(`hex("18a94b")))]),
  ]
];
