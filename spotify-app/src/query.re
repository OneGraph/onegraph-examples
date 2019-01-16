module GetUsername = [%graphql
  {|query findMe {
  me {
    gmail {
      email
    }
  }
}|}
];

module GetUserNameQuery = ReasonApollo.CreateQuery(GetUsername);

let component = ReasonReact.statelessComponent("Query");

let make = (~token, _children) => {
  ...component,
  didMount: self => Js.log(OneGraphAuth.authHeaders),
  render: _ =>
    <GetUserNameQuery>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => <div> {ReasonReact.string("Loading")} </div>
             | Error(error) =>
               <div> {ReasonReact.string(error##message)} </div>
             | Data(response) =>
               <div>
                 {
                   ReasonReact.string(
                     switch (response##me##gmail) {
                     | Some(_gmail) => "Found email"
                     | None => "Not Found"
                     },
                   )
                 }
               </div>
             /* Handles a deeply nested optional response */
             }
         }
    </GetUserNameQuery>,
};
