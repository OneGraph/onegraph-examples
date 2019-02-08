type props = {styles: Js.nullable(string)};
type style = {
  x: float,
  scale: float,
  rotate: float,
};
type springConfig('a) = Js.t('a);

[@bs.deriving abstract]
type jsProps = {
  /* some example fields */
  className: string,
  /* `type` is reserved in Reason. use `type_` and make it still compile to the
     JS key `type` */
  styles: unit => array(style),
  defaultStyles: array(style),
};

[@bs.module]
external starggeredMotion: ReasonReact.reactClass =
  "../node_modules/react-motion/lib/StaggeredMotion.js";

[@bs.module "react-motion"] [@bs.scope "presets"]
external noWobble: springConfig('a) = "";

[@bs.module "react-motion"] [@bs.scope "presets"]
external gentle: springConfig('a) = "";

[@bs.module "react-motion"] [@bs.scope "presets"]
external wobbly: springConfig('a) = "";

[@bs.module "react-motion"] [@bs.scope "presets"]
external stiff: springConfig('a) = "";

[@bs.module "react-motion"] [@bs.val]
external spring: (float, springConfig('a)) => float = "spring";

let component = ReasonReact.statelessComponent("ReactMotionBinding");

let make = (~className, ~getAimationStyleValue, ~defaultStyles, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=starggeredMotion,
    ~props=jsProps(~className, ~styles=getAimationStyleValue, ~defaultStyles),
    children,
  );

let a = spring(0., gentle);
