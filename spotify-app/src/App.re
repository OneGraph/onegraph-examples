let component = ReasonReact.statelessComponent("Component1");

let make = _children => {
  ...component,
  render: _self =>
    <div> <h1> {ReasonReact.string("Welcome to SpotDJ")} </h1> </div>,
};
