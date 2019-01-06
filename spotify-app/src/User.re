open Utils;

let userIcon = requireAssetURI("./img/user.png");

let component = ReasonReact.statelessComponent("User");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <img src=userIcon alt="user icon" />
      <p> {ReasonReact.string("userABC")} </p>
    </div>,
};
