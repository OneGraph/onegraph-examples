import App, { Container } from 'next/app'
import React from 'react'
import { ApolloProvider } from 'react-apollo'
import { ThemeProvider } from 'react-fela'
import { AuthProvider } from 'react-onegraph'

import withApolloClient from '../integration/withApolloClient'
import theme from '../styling/theme'

import { APP_ID } from '../../env'

class MyApp extends App {
  render() {
    const { Component, pageProps, apolloClient } = this.props
    return (
      <Container>
        <ThemeProvider theme={theme}>
          <AuthProvider appId={APP_ID}>
            <ApolloProvider client={apolloClient}>
              <Component {...pageProps} />
            </ApolloProvider>
          </AuthProvider>
        </ThemeProvider>
      </Container>
    )
  }
}

export default withApolloClient(MyApp)
