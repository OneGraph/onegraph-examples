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

let appearScale =
  keyframes([
    (
      0,
      [transforms([`scaleX(0.4), `translate((`pct(-80.), `pct(-50.)))])],
    ),
    (
      100,
      [transforms([`scaleX(1.), `translate((`pct(-50.), `pct(-50.)))])],
    ),
  ]);

let scaleAnimation = [%css
  [
    animationName(appearScale),
    animationDuration(`ms(400)),
    animationFillMode(`forwards),
    animationTimingFunction(`easeOut),
  ]
];

let appearBackToFront =
  keyframes([
    (0, [transforms([`scale(0.6)])]),
    (100, [transforms([`scale(1.)])]),
  ]);

let toFrontAnimation = [%css
  [
    animationName(appearBackToFront),
    animationDuration(`ms(600)),
    animationFillMode(`forwards),
    animationTimingFunction(`easeOut),
  ]
];

let flipToLeft = (~startScale, ~endScale) =>
  keyframes([
    (
      0,
      [
        transforms([
          `scale(startScale),
          `rotateY(`deg(0.)),
          `translate((`px(200), `px(0))),
        ]),
      ],
    ),
    (
      100,
      [
        transforms([
          `scale(endScale),
          `rotateY(`deg(-40.)),
          `translate((`px(0), `px(0))),
        ]),
      ],
    ),
  ]);

let flipToLeftAnimation = (~startScale, ~endScale) => [%css
  [
    animationName(flipToLeft(~startScale, ~endScale)),
    animationDuration(`ms(500)),
    animationFillMode(`forwards),
    animationTimingFunction(`easeOut),
  ]
];
