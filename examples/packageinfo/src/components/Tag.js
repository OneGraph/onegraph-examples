import React from 'react'
import { useFela } from 'react-fela'

export default function Tag({ children, color }) {
  const { css } = useFela()

  return (
    <div
      className={css({
        fontSize: 11,
        padding: '5px 8px',
        margin: '0 3px',
        alignSelf: 'center',
        backgroundColor: color,
        borderRadius: 4,
      })}>
      {children}
    </div>
  )
}
