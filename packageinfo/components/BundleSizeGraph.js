import React from 'react'
import {
  FlexibleWidthXYPlot,
  XAxis,
  YAxis,
  DiscreteColorLegend,
  VerticalBarSeries,
  HorizontalGridLines,
  VerticalGridLines,
} from 'react-vis'
import { useFela } from 'react-fela'

import Spacer from './Spacer'

import calculateVersionScore from '../utils/calculateVersionScore'

export default function BundleSizeGraph({ versions }) {
  const { css, theme } = useFela()

  const sizePoints = versions
    .map(({ version, size, gzip }) => ({
      x: version,
      y: size - gzip,
    }))
    .sort(
      (a, b) => calculateVersionScore(a.x) > calculateVersionScore(b.x) || -1
    )

  const gzipPoints = versions
    .map(({ version, gzip }) => ({
      x: version,
      y: gzip,
    }))
    .sort(
      (a, b) => calculateVersionScore(a.x) > calculateVersionScore(b.x) || -1
    )

  return (
    <div
      className={css({
        flex: 1,
        '@media (min-width: 640px)': { flexDirection: 'row' },
      })}>
      <DiscreteColorLegend
        className={css({
          order: 0,
          '@media (min-width: 640px)': { order: 2 },
        })}
        items={['size (gzip)', 'size']}
        orientation="horizontal"
      />
      <FlexibleWidthXYPlot
        height={220}
        stackBy="y"
        xType="ordinal"
        className={css({
          order: 2,
          '@media (min-width: 640px)': { order: 1 },
        })}>
        <VerticalGridLines />
        <HorizontalGridLines />
        <XAxis tickLabelAngle={-45} />
        <YAxis tickFormat={v => v / 1000 + 'kb'} left={10} />
        <VerticalBarSeries data={gzipPoints} />
        <VerticalBarSeries data={sizePoints} />
      </FlexibleWidthXYPlot>
    </div>
  )
}
