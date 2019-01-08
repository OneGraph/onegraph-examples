import fetch from 'node-fetch'
import { ApolloClient, InMemoryCache, HttpLink } from 'apollo-boost'

import { getAuth, loginGithub } from './oneGraph'
import { APP_ID } from '../../env'

let apolloClient = null

// Polyfill fetch() on the server (used by apollo-client)
if (!process.browser) {
  global.fetch = fetch
}

function create(initialState, getHeaders = () => {}) {
  // Check out https://github.com/zeit/next.js/pull/4611 if you want to use the AWSAppSyncClient
  return new ApolloClient({
    connectToDevTools: process.browser,
    ssrMode: !process.browser, // Disables forceFetch on the server (so queries are only run once)
    link: new HttpLink({
      uri: 'https://serve.onegraph.com/dynamic?app_id=' + APP_ID,
      headers: getHeaders(),
      credentials: 'same-origin', // Additional fetch() options like `credentials` or `headers`
    }),
    cache: new InMemoryCache().restore(initialState || {}),
  })
}

export default function initApollo(initialState) {
  // Make sure to create a new client for every server-side request so that data
  // isn't shared between connections (which would be bad)
  if (!process.browser) {
    return create(initialState)
  }

  if (!apolloClient) {
    apolloClient = create(initialState, getAuth().authHeaders)
  }

  return apolloClient
}
