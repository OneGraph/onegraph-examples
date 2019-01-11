import App, { Container } from 'next/app'
import React from 'react'
import { ApolloProvider } from 'react-apollo'
import { AuthProvider } from 'react-onegraph'

import withApolloClient from '../integration/withApolloClient'

import { APP_ID } from '../../env'

class MyApp extends App {
  render() {
    const { Component, pageProps, apolloClient } = this.props
    return (
      <Container>
        <ApolloProvider client={apolloClient}>
          <AuthProvider appId={APP_ID}>
            <Component {...pageProps} />
          </AuthProvider>
        </ApolloProvider>
      </Container>
    )
  }
}

export default withApolloClient(MyApp)
