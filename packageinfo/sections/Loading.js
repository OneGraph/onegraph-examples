import React from 'react'
import { useFela } from 'react-fela'

import LoadingSpinner from '../components/Loading'

export default function Loading() {
  const { css } = useFela()

  return (
    <div
      className={css({
        flex: 1,
        alignSelf: 'center',
        paddingTop: '10vh',
        paddingLeft: '10%',
        paddingRight: '10%',
        color: 'rgb(180, 180, 180)',
        fontSize: 22,
      })}>
      <LoadingSpinner />
    </div>
  )
}
