import React, { useState } from 'react'
import { formatDistance } from 'date-fns'
import { useFela } from 'react-fela'

import Conditional from '../components/Conditional'
import Link from '../components/Link'

export default function Versions({ versions, packageName }) {
  const [expanded, toggleExpansion] = useState(false)
  const { css } = useFela()

  const shownVersions = expanded ? versions : versions.slice(0, 10)
  const versionsCount = versions.length

  return (
    <div>
      <h2>Versions</h2>
      <hr
        className={css({
          height: 1,
          backgroundColor: 'rgb(200, 200, 200)',
          border: 0,
        })}
      />
      <div>
        {shownVersions.map(({ date, version }, index) => (
          <div
            key={date + version}
            className={css({
              flexDirection: 'row',
              borderBottomStyle: 'solid',
              borderBottomColor: 'rgb(230, 230, 230)',
              borderBottomWidth: versionsCount - 1 === index ? 0 : 1,
              padding: '5px 0',
              justifyContent: 'space-between',
            })}>
            <div
              className={css({
                flex: 1,
                minWidth: 120,
                paddingRight: 10,
              })}>
              {version}
            </div>

            <div
              className={css({
                color: 'grey',
                flex: '0 1 160px',
                textAlign: 'right',
                paddingRight: 30,
              })}>
              {formatDistance(new Date(date), new Date())}
            </div>
            <div className={css({ flexDirection: 'row' })}>
              <Link
                noUnderline
                href={`https://www.npmjs.com/package/${packageName}/v/${version}`}>
                npm
              </Link>

              <span className={css({ color: 'grey' })}>{' | '}</span>
              <Link
                noUnderline
                href={`https://unpkg.com/${packageName}@${version}`}>
                unpkg
              </Link>
            </div>
          </div>
        ))}

        <Conditional condition={!expanded && versionsCount > 10}>
          <div
            className={css({
              color: 'grey',
              padding: 10,
              cursor: 'pointer',
            })}
            onClick={() => toggleExpansion(true)}>
            Show All
          </div>
        </Conditional>
      </div>
    </div>
  )
}
