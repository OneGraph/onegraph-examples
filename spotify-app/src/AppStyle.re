open Emotion;

let app = [%css
  [
    textAlign(`center),
    backgroundColor(`hex("000000")),
    minHeight(`vh(100.)),
    minWidth(`vw(100.)),
    overflow(`auto),
    color(`hex("ffffff")),
  ]
];

let userIcon = [%css [width(`px(25))]];

let flexWrapper = (~justify, ~align) => [%css
  [display(`flex), justifyContent(justify), alignItems(align)]
];

let pageSubTitle = [%css
  [fontSize(`px(24)), marginBottom(`px(32)), fontWeight(300)]
];
