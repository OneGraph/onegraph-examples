import fetch from 'node-fetch'
import { ApolloClient, InMemoryCache, HttpLink, ApolloLink } from 'apollo-boost'

import { APP_ID } from '../../env'

const link = new HttpLink({
  uri: 'https://serve.onegraph.com/dynamic?show_metrics=true&app_id=' + APP_ID,
})

const metricsWatchers = {}

let id = 0

export function addMetricsWatcher(f) {
  const watcherId = (id++).toString(36)
  metricsWatchers[watcherId] = f
  return () => {
    delete metricsWatchers[watcherId]
  }
}

function runWatchers(requestMetrics) {
  for (const watcherId of Object.keys(metricsWatchers)) {
    try {
      metricsWatchers[watcherId](requestMetrics)
    } catch (e) {
      console.error('error running metrics watcher', e)
    }
  }
}

const trackMetrics = new ApolloLink((operation, forward) => {
  return forward(operation).map(response => {
    runWatchers(
      response
        ? response.extensions
          ? response.extensions.metrics
          : null
        : null
    )
    return response
  })
})

let apolloClient = null

// Polyfill fetch() on the server (used by apollo-client)
if (!process.browser) {
  global.fetch = fetch
}

function create(initialState) {
  // Check out https://github.com/zeit/next.js/pull/4611 if you want to use the AWSAppSyncClient
  return new ApolloClient({
    connectToDevTools: process.browser,
    ssrMode: !process.browser, // Disables forceFetch on the server (so queries are only run once)
    link: trackMetrics.concat(link),
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
    apolloClient = create(initialState)
  }

  return apolloClient
}
