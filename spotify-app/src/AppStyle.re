open Emotion;

let app = [%css [textAlign(`center)]];

let userIcon = [%css [width(`px(25))]];

let flexWrapper = (~justify, ~align) => [%css
  [display(`flex), justifyContent(justify), alignItems(align)]
];

let pageTitle = [%css [fontSize(`px(48)), marginBottom(`px(64))]];

let pageSubTitle = [%css
  [fontSize(`px(24)), marginBottom(`px(32)), fontWeight(300)]
];
