import React from 'react'
import { useFela } from 'react-fela'

export default function LinkTile({ link, title, iconType, icon }) {
  const { css, theme } = useFela()

  return (
    <a
      href={link}
      className={css({
        textDecoration: 'none',
        color: 'black',
        cursor: 'pointer',
        color: theme.colors.primary,
        fontSize: 15,
        paddingTop: 3,
        textAlign: 'center',
        padding: '0 5px',
      })}>
      {title}
    </a>
  )
}
