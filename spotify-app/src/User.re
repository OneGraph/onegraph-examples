let component = ReasonReact.statelessComponent("User");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <img src="./img/user.png" alt="user icon" />
      <p> {ReasonReact.string("userABC")} </p>
    </div>,
};
