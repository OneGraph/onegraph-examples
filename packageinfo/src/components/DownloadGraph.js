import React from 'react'
import {
  FlexibleWidthXYPlot,
  XAxis,
  YAxis,
  HorizontalGridLines,
  VerticalGridLines,
  LineSeries,
} from 'react-vis'
import { useFela } from 'react-fela'

import Spacer from './Spacer'

import padNumber from '../utils/padNumber'

const months = [
  'Jan',
  'Feb',
  'Mar',
  'Apr',
  'May',
  'Jun',
  'Jul',
  'Aug',
  'Sep',
  'Oct',
  'Nov',
  'Dec',
]

const getTickFormatX = range => value => {
  const date = new Date(value)

  if (range === 'year') {
    return months[date.getMonth()]
  }

  return padNumber(date.getDate())
}

const getTickFormatY = highestDownload => value => {
  if (highestDownload > 1000000) {
    return value / 1000000 + 'kk'
  }

  if (highestDownload > 1000) {
    return value / 1000 + 'k'
  }

  return value
}

export default function DownloadGraph({ downloads, totalDownloads, range }) {
  const { css, theme } = useFela()

  const dataPoints = downloads.map(({ day, count }) => ({
    x: new Date(day).getTime(),
    y: count,
  }))

  const highestDownload = downloads.sort((a, b) => a.count < b.count || -1)[0]
    .count

  return (
    <div className={css({ flex: 1 })}>
      <h3 className={css({ fontWeight: 400, textAlign: 'center' })}>
        <b className={css({ color: theme.colors.primary })}>
          {totalDownloads.toLocaleString()}
        </b>{' '}
        last {range}
      </h3>
      <Spacer size={10} />
      <FlexibleWidthXYPlot height={220} xType="time">
        <VerticalGridLines />
        <HorizontalGridLines />
        <XAxis
          tickTotal={range === 'week' ? 7 : undefined}
          tickFormat={getTickFormatX(range)}
          tickLabelAngle={-45}
        />
        <YAxis tickFormat={getTickFormatY(highestDownload)} />
        <LineSeries curve={'curveMonotoneX'} data={dataPoints} />
      </FlexibleWidthXYPlot>
    </div>
  )
}
