import React from 'react'
import { useFela } from 'react-fela'

function FooterLink({ href, children }) {
  const { css, theme } = useFela()

  return (
    <a
      href={href}
      className={css({
        padding: '0 4px',
        color: theme.colors.primary,
      })}>
      {children}
    </a>
  )
}

export default function Footer() {
  const { css } = useFela()

  return (
    <div
      className={css({
        padding: '30px 10px 50px',
        fontSize: 14,
        backgroundColor: 'rgb(235, 235, 235)',
      })}>
      <p className={css({ textAlign: 'center' })}>
        Brought to you with ♥︎ by OneGraph.
      </p>
      <div
        className={css({
          justifyContent: 'center',
          flexDirection: 'row',
          paddingTop: 5,
        })}>
        <FooterLink href="https://github.com/OneGraph/onegraph-examples/tree/master/examples/packageinfo">
          GitHub
        </FooterLink>
        <FooterLink href="https://www.onegraph.com">OneGraph</FooterLink>
        <FooterLink href="https://twitter.com/onegraphio">Twitter</FooterLink>
      </div>
    </div>
  )
}
