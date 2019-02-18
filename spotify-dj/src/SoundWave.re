open Emotion;

let soundWaveWrapper = [%css
  [
    width(`px(50)),
    height(`px(50)),
    padding(`px(2)),
    transform(`rotate(`deg(180.))),
    borderTop(`px(1), `solid, `hex("7ED321")),
  ]
];

let soundCol = [%css
  [flexShrink(1.), overflow(`hidden), transitionDuration(`ms(300))]
];

let soundColOnPause = heightPct => [%css
  [height(`pct(heightPct)), opacity(0.3)]
];

/*Top to bottom*/
let swing1 =
  keyframes([
    (0, [height(`pct(100.))]),
    (25, [height(`pct(82.))]),
    (50, [height(`pct(75.))]),
    (75, [height(`pct(53.))]),
    (100, [height(`pct(40.))]),
  ]);

/*Bottom to Top*/
let swing2 =
  keyframes([
    (0, [height(`pct(50.))]),
    (25, [height(`pct(65.))]),
    (50, [height(`pct(75.))]),
    (75, [height(`pct(88.))]),
    (100, [height(`pct(100.))]),
  ]);

/*Top to bottom to top*/
let swing3 =
  keyframes([
    (0, [height(`pct(75.))]),
    (25, [height(`pct(53.))]),
    (50, [height(`pct(36.))]),
    (75, [height(`pct(50.))]),
    (100, [height(`pct(80.))]),
  ]);

let colAnimation = (~name, ~durationMs) => [%css
  [
    animationName(name),
    animationDuration(`ms(durationMs)),
    animationIterationCount(`infinite),
    animationDirection(`alternate),
  ]
];

let soundBlock = [%css
  [
    backgroundColor(`hex("fff")),
    height(`px(3)),
    width(`px(14)),
    margin(`px(2)),
    borderStyle(`none),
  ]
];

let green = [%css [backgroundColor(`hex("7ED321"))]];

let yellow = [%css [backgroundColor(`hex("F5E223"))]];

let red = [%css [backgroundColor(`hex("D0021B"))]];

let grey = [%css [backgroundColor(`hex("cfcfcf"))]];

let component = ReasonReact.statelessComponent("SoundWave");

let make = (~isPlaying, _children) => {
  ...component,
  render: _self =>
    <div>
      <div
        className={
          Cn.make([
            soundWaveWrapper,
            SharedCss.flexWrapper(~justify=`center, ~align=`flexStart),
          ])
        }>
        <div
          className={
            isPlaying ?
              Cn.make([
                soundCol,
                colAnimation(~name=swing1, ~durationMs=500),
              ]) :
              Cn.make([soundCol, soundColOnPause(47.)])
          }>
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([yellow, soundBlock])} />
          <div className={Cn.make([yellow, soundBlock])} />
          <div className={Cn.make([red, soundBlock])} />
          <div className={Cn.make([red, soundBlock])} />
        </div>
        <div
          className={
            isPlaying ?
              Cn.make([
                soundCol,
                colAnimation(~name=swing2, ~durationMs=500),
              ]) :
              Cn.make([soundCol, soundColOnPause(22.)])
          }>
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([yellow, soundBlock])} />
          <div className={Cn.make([yellow, soundBlock])} />
          <div className={Cn.make([red, soundBlock])} />
          <div className={Cn.make([red, soundBlock])} />
        </div>
        <div
          className={
            isPlaying ?
              Cn.make([
                soundCol,
                colAnimation(~name=swing3, ~durationMs=800),
              ]) :
              Cn.make([soundCol, soundColOnPause(80.)])
          }>
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([green, soundBlock])} />
          <div className={Cn.make([yellow, soundBlock])} />
          <div className={Cn.make([yellow, soundBlock])} />
          <div className={Cn.make([red, soundBlock])} />
          <div className={Cn.make([red, soundBlock])} />
        </div>
      </div>
    </div>,
};
