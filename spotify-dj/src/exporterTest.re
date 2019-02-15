open ReasonReact;

module Test = [%graphql
  {|
query test($email: String!, $limit: Int = 50) {
  clearbit {
    enrich(email: $email) {
      company {
        twitter {
          timeline(first: $limit) {
            tweets {
              id
            }
          }
        }
      }
    }
  }
}
|}
];

let component = ReasonReact.statelessComponent("TestComp");

module TestMutation = ReasonApollo.CreateMutation(Test);

let make = (~email, ~limit=?, _children) => {
  ...component,
  render: _ =>
    <TestMutation>
      ...(
           (mutation /* Mutation to call */, _ /* Result of your mutation */) => {
             let testQuery = Test.make(~email="Bob", ~limit?, ());

             <div>
               <button
                 onClick=(
                   _mouseEvent =>
                     mutation(
                       ~variables=testQuery##variables,
                       /* Queries to refetch that might be stale because of this mutation */
                       ~refetchQueries=[||],
                       (),
                     )
                     |> ignore
                 )>
                 (ReasonReact.string("Trigger `test` Mutation"))
               </button>
             </div>;
           }
         )
    </TestMutation>,
};
