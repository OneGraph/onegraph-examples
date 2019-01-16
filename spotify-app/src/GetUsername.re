module GetUsername = [%graphql
  {|query findUsername {
  me {
    gmail {
      sub
      name
      givenName
    }
  }
}|}
];

module GetUserNameQuery = ReasonApollo.CreateQuery(GetUsername);

let component = ReasonReact.statelessComponent("GetUsername");

let make = (~auth, ~setLogInStatus, _children) => {
  ...component,
  render: self =>
    <GetUserNameQuery>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => <div> {ReasonReact.string("Loading")} </div>
             | Error(error) =>
               <div> {ReasonReact.string(error##message)} </div>
             | Data(response) =>
               switch (response##me##gmail) {
               | Some(gmail) =>
                 switch (gmail##name) {
                 | Some(name) =>
                   <User key=name auth setLogInStatus userName=name />

                 | None => <User key="" auth setLogInStatus userName="" />
                 }
               | None => <User key="" auth setLogInStatus userName="" />
               }
             }
         }
    </GetUserNameQuery>,
};
