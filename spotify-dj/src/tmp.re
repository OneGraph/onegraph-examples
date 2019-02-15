/* let inMemoryCache = ApolloInMemoryCache.createInMemoryCache(); */
/* let serverUrl = "https://serve.onegraph.com/dynamic?app_id=0b33e830-7cde-4b90-ad7e-2a39c57c0e11"; */
/* let httpLink = */
/*   ApolloLinks.createHttpLink(~uri=serverUrl, ~credentials="include", ()); */
/* let auth = OneGraphAuth.newAuth(OneGraphAuth.config); */
/* let authLink = */
/*   ApolloLinks.createContextLink( */
/*     OneGraphAuth.( */
/*       () => { */
/*         "headers": { */
/*           "authorization": auth |> authHeaders |> authenticationHeaderGet, */
/*         }, */
/*       } */
/*     ), */
/*   ); */
/* let instance = */
/*   ReasonApollo.createApolloClient( */
/*     ~link=ApolloLinks.from([|authLink, httpLink|]), */
/*     ~cache=inMemoryCache, */
/*     (), */
/*   ); */
/* module Test = [%graphql */
/*   {| */
     /* query test($email: String!, $limit: Int = 50) { */
     /*   clearbit { */
     /*     enrich(email: $email) { */
     /*       company { */
     /*         twitter { */
     /*           timeline(first: $limit) { */
     /*             tweets { */
     /*               id */
     /*             } */
     /*           } */
     /*         } */
     /*       } */
     /*     } */
     /*   } */
     /* } */
     /* |} */
/* ]; */
/* module TestQuery = ReasonApollo.CreateQuery(Test); */
/* let component = ReasonReact.statelessComponent("TestComp"); */
/* let make = (~id, _children) => { */
/*   ...component, */
/*   render: _ => { */
/*     let testQuery = Test.make(~id, ()); */
/*     <TestQuery variables=testQuery##variables> */
/*       ...( */
/*            ({result}) => */
/*              switch (result) { */
/*              | Loading => <div> (ReasonReact.string("Loading")) </div> */
/*              | Error(error) => */
/*                <div> (ReasonReact.string(error##message)) </div> */
/*              | Data(response) => */
/*                <div> */
/*                  <pre> */
/*                    ( */
/*                      response */
/*                      |> Js.Json.stringifyAny */
/*                      |> Belt.Option.getExn */
/*                      |> ReasonReact.string */
/*                    ) */
/*                  </pre> */
/*                </div> */
/*              } */
/*          ) */
/*     </TestQuery>; */
/*   }, */
/* }; */
