import React from 'react'
import { useFela } from 'react-fela'

import Link from '../components/Link'

export default function Dependencies({ dependencies, updateSearch }) {
  const { css, theme } = useFela()

  const dependencyCount = dependencies.length

  return (
    <div>
      <h2>Dependencies</h2>
      <hr
        className={css({
          height: 1,
          backgroundColor: 'rgb(200, 200, 200)',
          border: 0,
        })}
      />
      <div>
        {dependencies.map(({ name, version }, index) => (
          <div
            key={name + version}
            className={css({
              flexDirection: 'row',
              borderBottomStyle: 'solid',
              borderBottomColor: 'rgb(230, 230, 230)',
              borderBottomWidth: dependencyCount - 1 === index ? 0 : 1,
              padding: '5px 0',
              justifyContent: 'space-between',
            })}>
            <span className={css({ color: 'grey', padding: '0 10px 0 0' })}>
              {index + 1}
            </span>
            <div
              className={css({
                flex: 1,
                minWidth: 120,
                paddingRight: 10,
              })}>
              <div
                className={css({
                  color: theme.colors.primary,
                  cursor: 'pointer',
                })}
                onClick={() => updateSearch(name)}>
                {name}
              </div>
            </div>

            <div className={css({ flexDirection: 'row' })}>
              <Link noUnderline href={`https://www.npmjs.com/package/${name}`}>
                npm
              </Link>

              <span className={css({ color: 'grey' })}>{' | '}</span>
              <Link noUnderline href={`https://unpkg.com/${name}/`}>
                unpkg
              </Link>
            </div>
          </div>
        ))}
      </div>
    </div>
  )
}
