import { createRenderer } from 'fela'
import plugins from 'fela-preset-web'
import sortMediaQueryMobileFirst from 'fela-sort-media-query-mobile-first'

import staticStyle from './staticStyle'

export default function getFelaRenderer() {
  const renderer = createRenderer({
    enhancers: [sortMediaQueryMobileFirst()],
    plugins,
  })

  staticStyle.forEach(rule => renderer.renderStatic(rule.style, rule.selector))

  return renderer
}
