import React, { useState } from 'react'

import getTotalDownloads from '../utils/getTotalDownloads'
import clusterDownloads from '../utils/clusterDownloads'

import TabBar from '../components/TabBar'
import Spacer from '../components/Spacer'
import Headline from '../components/Headline'
import DownloadGraph from '../components/DownloadGraph'

export default function Downloads({ downloads }) {
  const [range, setRange] = useState('month')

  return (
    <div>
      <Headline>Downloads</Headline>
      <div>
        <TabBar
          activeTab={range}
          tabs={[
            {
              key: 'week',
              name: 'Week',
              action: () => setRange('week'),
            },
            {
              key: 'month',
              name: 'Month',
              action: () => setRange('month'),
            },
            {
              key: 'year',
              name: 'Year',
              action: () => setRange('year'),
            },
          ]}
        />
        <Spacer size={10} />
        <DownloadGraph
          range={range}
          totalDownloads={getTotalDownloads([...downloads], range)}
          downloads={clusterDownloads([...downloads], range)}
        />
      </div>
    </div>
  )
}
