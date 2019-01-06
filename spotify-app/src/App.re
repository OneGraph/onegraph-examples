module Css = AppStyle;

let component = ReasonReact.statelessComponent("Component1");

let make = _children => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div className=Css.app>
        <User />
        <h1> {string("Welcome to SpotDJ")} </h1>
        <div className="link-sharing">
          <p>
            {
              string("Share the following link to invite people to your music")
            }
          </p>
          <div>
            <input value="www.example.com/?userId" />
            <button> {string("share")} </button>
          </div>
          <div>
            <p> {string("240 Listners")} </p>
            <a> {string("Stop Sharing")} </a>
          </div>
        </div>
        <div className="current-playing">
          <p> {string("Played by userABC")} </p>
        </div>
      </div>
    ),
};
