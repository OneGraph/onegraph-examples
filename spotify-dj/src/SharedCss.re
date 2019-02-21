open Emotion;

let flexWrapper = (~justify, ~align) => [%css
  [display(`flex), justifyContent(justify), alignItems(align)]
];

let button = [%css
  [
    backgroundColor(`hex("19A34A")),
    borderStyle(`none),
    margin(`px(16)),
    marginBottom(`px(32)),
    padding2(`px(8), `px(16)),
    borderRadius(`px(3)),
    color(`hex("fff")),
    fontSize(`px(18)),
    fontWeight(600),
    boxShadow(~x=`px(0), ~y=`px(2), ~blur=`px(4), `hex("00000040")),
    transitionDuration(`ms(350)),
    select(
      ":hover",
      [
        backgroundColor(`hex("20c159")),
        cursor(`pointer),
        transform(`scale(1.2)),
      ],
    ),
    select(":active", [important(backgroundColor(`hex("20c159")))]),
    select(":focus", [important(backgroundColor(`hex("20c159")))]),
  ]
];

let icon = [%css [width(`px(25)), borderRadius(`pct(50.))]];

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

let slideToFront =
  keyframes([
    (
      0,
      [
        transforms([
          `scale(0.3),
          `rotateY(`deg(50.)),
          `translate((`px(400), `px(0))),
        ]),
      ],
    ),
    (
      30,
      [
        transforms([
          `scale(0.3),
          `rotateY(`deg(50.)),
          `translate((`px(400), `px(0))),
        ]),
      ],
    ),
    (
      100,
      [
        transforms([
          `scale(1.),
          `rotateY(`deg(0.)),
          `translate((`px(0), `px(0))),
        ]),
      ],
    ),
  ]);

let toFrontAnimation = (~animationKeyframes) => [%css
  [
    animationName(animationKeyframes),
    animationDuration(`ms(700)),
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
