open Utils;

module Css = AppStyle;

let userIcon = requireAssetURI("./img/user.png");

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div className=Css.app>
        <User />
        <h1 className=Css.pageTitle> {string("Welcome to SpotDJ")} </h1>
        <LinkShare />
        <div className="current-playing">
          <p> {string("Played by userABC")} </p>
        </div>
      </div>
    ),
};
