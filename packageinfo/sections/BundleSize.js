import React from 'react'

import Spacer from '../components/Spacer'
import Headline from '../components/Headline'
import BundleSizeGraph from '../components/BundleSizeGraph'

export default function BundleSize({ bundlephobiaHistory, versions }) {
  const filteredVersions = bundlephobiaHistory.filter(Boolean).slice(0, 10)

  return (
    <div>
      <Headline>Bundle</Headline>
      {filteredVersions.length > 0 ? (
        <BundleSizeGraph versions={filteredVersions} />
      ) : (
        <div>There's no bundle size information for previous versions.</div>
      )}
    </div>
  )
}
