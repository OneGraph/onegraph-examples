import React from 'react'
import { useFela } from 'react-fela'

export default function Placeholder({ updateSearch }) {
  const { css } = useFela()

  return (
    <div
      className={css({
        flex: 1,
        textAlign: 'center',
        paddingTop: '10vh',
        paddingLeft: '10%',
        paddingRight: '10%',
        color: 'rgb(180, 180, 180)',
        fontSize: 22,
      })}>
      Enter a package name to see detailed package information.
      <br />
      <br />
      <span className={css({ fontSize: 18 })}>
        Try{' '}
        <span
          onClick={() => updateSearch('graphql')}
          className={css({ color: 'rgb(150, 150, 150)', cursor: 'pointer' })}>
          <i>graphql</i>
        </span>
        {' '}
        for example.
      </span>
    </div>
  )
}
