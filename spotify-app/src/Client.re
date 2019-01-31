/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(
    ~uri=Config.oneGraphUri,
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
