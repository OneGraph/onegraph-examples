open Emotion;

/*Style*/
let userIcon = [%css [
  width(`px(25)),
  margin4(`zero, `em(0.25), `zero, `em(0.5)),
  borderRadius(`pct(50.))
]];
let anchor = [%css [cursor(`pointer)]];
let separator = [%css [
  width(`em(1.)),
  backgroundColor(`hex("fff")),
  height(`px(1)),
  verticalAlign(`middle),
  margin4(`zero, `em(0.5), `zero, `em(0.5)),
  display(`inlineBlock),
]];

let handleLogOut = (auth, logOut) =>
  Js.Promise.(
    OneGraphAuth.(
      auth
        |> logout(_, "spotify")
        |> then_(() => isLoggedIn(auth, "spotify"))
        |> then_(isLoggedIn => {
            if(!isLoggedIn) {
              logOut();
            }
            resolve();
          })
        |> catch(err => resolve(Js.log(err)))
        |> ignore
    )
  )

let component = ReasonReact.statelessComponent("User");

let make = (~auth, ~logOut, ~userName, ~userIconUrl, _children) => {
  ...component,
  render: self =>
    ReasonReact.(
          <p>
            {string("Listening as")}
            <img className=userIcon src=userIconUrl alt={"user icon"} />
            {string(userName)}
            <span className=separator />
            <a
              className=anchor
              onClick={(_e) => handleLogOut(auth, logOut)}
            >
              {string("Sign Out")}
            </a>
          </p>
    ),
};
