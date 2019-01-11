import React from 'react'
import RouterLink from 'next/link'
import { useFela } from 'react-fela'

export default function Link({ href, noUnderline, children }) {
  const { css, theme } = useFela()

  return (
    <RouterLink href={href}>
      <a
        className={css({
          color: theme.colors.primary,
          textDecoration: noUnderline ? 'none' : 'underline',
        })}>
        {children}
      </a>
    </RouterLink>
  )
}
