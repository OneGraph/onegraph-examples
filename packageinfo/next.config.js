var webpack = require('webpack')
var withOffline = require('next-offline')

var withBundleAnalyzer = require('@next/bundle-analyzer')({
  enabled: process.env.ANALYZE === 'true',
})

const config = {
  target: 'serverless',
  env: {
    APP_ID: process.env.APP_ID,
  },
  workboxOpts: {
    runtimeCaching: [
      {
        urlPattern: /^https:\/\/fonts\.googleapis\.com/,
        handler: 'cacheFirst',
        options: {
          cacheName: 'google-fonts-stylesheets',
          expiration: { maxEntries: 100, maxAgeSeconds: 60 * 60 * 24 * 30 },
          cacheableResponse: {
            statuses: [0, 200],
          },
        },
      },
      { urlPattern: /^https?.*/, handler: 'networkFirst' },
    ],
    // Not sure adding display swap is actually working (i see fetch for plain still after)
    // importScripts: ['static/js/service-worker-extras.js'],
    skipWaiting: true,
    clientsClaim: true,
  },
}

module.exports = withBundleAnalyzer(withOffline(config))
