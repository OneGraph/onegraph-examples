open Emotion;

let userIcon = [%css [margin4(`zero, `em(0.25), `zero, `em(0.5))]];

let anchor = [%css [cursor(`pointer)]];

let separator = [%css
  [
    width(`em(1.)),
    backgroundColor(`hex("fff")),
    height(`px(1)),
    verticalAlign(`middle),
    margin4(`zero, `em(0.5), `zero, `em(0.5)),
    display(`inlineBlock),
  ]
];

let wrapper = [%css
  [backgroundColor(`hex("000")), paddingBottom(`px(4))]
];

let handleLogOut = (auth, setLogOut) =>
  Js.Promise.(
    OneGraphAuth.(
      auth
      |> logout(_, "spotify")
      |> then_(() => isLoggedIn(auth, "spotify"))
      |> then_(isLoggedIn => {
           if (!isLoggedIn) {
             setLogOut();
           };
           resolve();
         })
      |> catch(err => resolve(Js.log(err)))
      |> ignore
    )
  );

let component = ReasonReact.statelessComponent("User");

let make = (~auth, ~userName, ~userIconUrl, ~setLogOut, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <p className=wrapper>
        {string("Listening as")}
        <img
          className={Cn.make([SharedCss.icon, userIcon])}
          src=userIconUrl
          alt="user icon"
        />
        {string(userName)}
        <span className=separator />
        <a className=anchor onClick={_e => handleLogOut(auth, setLogOut)}>
          {string("Sign Out")}
        </a>
      </p>
    ),
};
