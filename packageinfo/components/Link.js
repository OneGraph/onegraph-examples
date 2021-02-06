import React from 'react'
import { useFela } from 'react-fela'

export default function Link({ href, noUnderline, children }) {
  const { css, theme } = useFela()

  return (
    <a
      href={href}
      className={css({
        color: theme.colors.primary,
        textDecoration: noUnderline ? 'none' : 'underline',
      })}>
      {children}
    </a>
  )
}
