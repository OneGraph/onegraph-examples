import React from 'react'
import { useFela } from 'react-fela'

export default function Tile({ label, sup, children }) {
  const { css } = useFela()

  return (
    <div
      className={css({
        flex: '1 0 100%',
        borderBottom: '1px solid rgb(220, 220, 220)',
        justifyContent: 'center',
        alignItems: 'center',
        textAlign: 'center',
        padding: '5px 0',
        '@media (min-width: 335px)': {
          flex: '1 0 30%',
          borderRight: '1px solid rgb(220, 220, 220)',

          ':nth-child(3n)': {
            borderRight: 0,
          },

          ':nth-last-child(-n+3)': {
            borderBottom: 0,
          },
        },
      })}>
      <h3 className={css({ lineHeight: 1.4, fontWeight: 400, fontSize: 16 })}>
        {children}
      </h3>
      <span
        className={css({
          fontSize: 13,
          paddingLeft: 1,
          fontWeight: 400,
          color: 'rgb(80, 80, 80)',
        })}>
        {sup ? (
          <sup className={css({ fontSize: 12, position: 'relative', top: 3 })}>
            {sup}
          </sup>
        ) : null}
        {label}
      </span>
    </div>
  )
}
