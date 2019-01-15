import React from 'react'
import { useFela } from 'react-fela'

import Link from '../components/Link'
import Headline from '../components/Headline'

export default function Maintainers({ maintainers }) {
  const { css } = useFela()

  const maintainersCount = maintainers.length

  return (
    <div>
      <Headline>Maintainers</Headline>
      {maintainers.map(({ name }, index) => (
        <div
          key={name}
          className={css({
            flexDirection: 'row',
            borderBottomStyle: 'solid',
            borderBottomColor: 'rgb(230, 230, 230)',
            borderBottomWidth: maintainersCount - 1 === index ? 0 : 1,
            padding: '5px 0',
          })}>
          <span className={css({ color: 'grey', padding: '0 10px 0 0' })}>
            {index + 1}
          </span>
          <Link noUnderline href={'https://www.npmjs.com/~' + name}>
            {name}
          </Link>
        </div>
      ))}
    </div>
  )
}
