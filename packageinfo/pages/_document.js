import Document, { Html, Head, Main, NextScript } from 'next/document'
import { renderToNodeList } from 'react-fela'

import FelaProvider from '../styling/FelaProvider'
import getFelaRenderer from '../styling/getFelaRenderer'

import PreloadStyleSheet from '../components/PreloadStyleSheet'

export default class MyDocument extends Document {
  static async getInitialProps(ctx) {
    const renderer = getFelaRenderer()
    const originalRenderPage = ctx.renderPage

    ctx.renderPage = () =>
      originalRenderPage({
        enhanceApp: App => props => <App {...props} renderer={renderer} />,
      })

    const initialProps = await Document.getInitialProps(ctx)
    const styles = renderToNodeList(renderer)

    return {
      ...initialProps,
      styles: [...initialProps.styles, ...styles],
    }
  }

  render() {
    return (
      <Html lang="en">
        <Head>
          <meta httpEquiv="content-type" content="text/html; charset=utf-8" />

          <meta
            name="title"
            content="PackageInfo | npm package information using OneGraph"
          />
          <PreloadStyleSheet href="/fonts/inter/inter.css" />
          <link
            rel="stylesheet"
            href="https://unpkg.com/react-vis/dist/style.css"
          />
          <link
            rel="stylesheet"
            href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.13.1/styles/github.min.css"
          />
        </Head>
        <body>
          <Main />
          <NextScript />
        </body>
      </Html>
    )
  }
}
