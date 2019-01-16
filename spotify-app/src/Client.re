/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(
    ~uri=
      "https://serve.onegraph.com/dynamic?app_id=bafd4254-c229-48c2-8c53-44a01477a43e",
    ~credentials="include",
    (),
  );

let auth = OneGraphAuth.newAuth(OneGraphAuth.config);

let authLink =
  ApolloLinks.createContextLink(
    OneGraphAuth.(
      () => {
        "headers": {
          "authorization": auth |> authHeaders |> authenticationHeaderGet,
        },
      }
    ),
  );

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|authLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );
