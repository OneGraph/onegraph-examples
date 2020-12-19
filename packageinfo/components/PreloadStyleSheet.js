import * as React from 'react'

export default function PreloadStyleSheet({ href }) {
  return (
    <link
      rel="preload"
      as="style"
      href={href}
      onLoad={(e) => {
        e.target.rel = 'stylesheet'
      }}
    />
  )
}
