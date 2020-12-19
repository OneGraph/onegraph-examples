import Head from 'next/head'
import React from 'react'
import { AuthProvider } from 'react-onegraph'
import { ThemeProvider } from 'react-fela'
import { Provider as UrqlProvider } from 'urql'

import FelaProvider from '../styling/FelaProvider'
import theme from '../styling/theme'

import client from '../integration/client'

import { APP_ID } from '../../env'

export default function App({ Component, pageProps, renderer }) {
  return (
    <AuthProvider appId={APP_ID}>
      <UrqlProvider value={client}>
        <FelaProvider renderer={renderer}>
          <ThemeProvider theme={theme}>
            <>
              <Head>
                <meta
                  name="viewport"
                  content="width=device-width,height=device-height,initial-scale=1, viewport-fit=cover"
                />
              </Head>
              <Component {...pageProps} />
            </>
          </ThemeProvider>
        </FelaProvider>
      </UrqlProvider>
    </AuthProvider>
  )
}
