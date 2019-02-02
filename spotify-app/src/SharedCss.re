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

let appear =
  keyframes([
    (0, [opacity(0.), visibility(`hidden)]),
    (1, [opacity(0.), visibility(`visible)]),
    (100, [opacity(1.)]),
  ]);

let appearAnimation = (~delayMs: int=0, ~direction) => [%css
  [
    opacity(direction == `normal ? 0. : 1.),
    animationName(appear),
    animationDuration(`ms(400)),
    animationDelay(`ms(delayMs)),
    animationFillMode(`forwards),
    animationTimingFunction(`easeOut),
    animationDirection(direction),
  ]
];

let marginZero = [%css [margin(`px(0))]];

let textBlack = [%css
  [color(`hex("000")), select(":hover", [color(`hex("000"))])]
];

let textBold = [%css [fontWeight(600)]];

let textWeak = [%css [opacity(0.8)]];
