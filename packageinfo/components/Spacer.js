import React from 'react'
import { useFela } from 'react-fela'

export default function Spacer({ size }) {
  const { css } = useFela()

  return (
    <div
      className={css({
        flexGrow: 0,
        flexShrink: 0,
        flexBasis: size + 'px',
        width: size,
      })}
    />
  )
}
