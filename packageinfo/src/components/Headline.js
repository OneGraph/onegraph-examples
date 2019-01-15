import React from 'react'
import { useFela } from 'react-fela'

export default function Headline({ children }) {
  const { css } = useFela()

  return (
    <h2 className={css({ borderBottom: '1px solid rgb(200, 200, 200)' })}>
      {children}
    </h2>
  )
}
