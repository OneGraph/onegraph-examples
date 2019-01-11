import RouterLink from 'next/link'

import Spacer from '../components/Spacer'

function FooterLink({ href, children }) {
  return (
    <RouterLink href={href}>
      <a
        style={{
          padding: '0 4px',
          color: 'red',
        }}>
        {children}
      </a>
    </RouterLink>
  )
}

export default function Footer() {
  return (
    <div
      style={{
        padding: '30px 10px 50px',
        fontSize: 14,
        backgroundColor: 'rgb(235, 235, 235)',
      }}>
      <p style={{ textAlign: 'center' }}>Brought to you with ♥︎ by OneGraph.</p>
      <div
        style={{
          justifyContent: 'center',
          flexDirection: 'row',
          paddingTop: 5,
        }}>
        <FooterLink href="https://github.com/OneGraph/onegraph-examples/tree/master/examples/youtube-captions">
          GitHub
        </FooterLink>
        <FooterLink href="https://www.onegraph.com">OneGraph</FooterLink>
        <FooterLink href="https://twitter.com/onegraphio">Twitter</FooterLink>
      </div>
    </div>
  )
}
