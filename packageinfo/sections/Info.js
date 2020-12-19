import React from 'react'
import { formatDistance } from 'date-fns'
import { useFela } from 'react-fela'

import Tile from '../components/Tile'
import LinkTile from '../components/LinkTile'
import Tag from '../components/Tag'
import Conditional from '../components/Conditional'
import Spacer from '../components/Spacer'
import Link from '../components/Link'

import formatBundleSize from '../utils/formatBundleSize'

export default function Info({ data, githubStatus }) {
  const { css } = useFela()

  const {
    name,
    description,
    primaryLanguage,
    license,
    repository,
    repositoryUrl,
    currentVersion,
    lastPublishDate,
    bundlephobia,
    homepage,
    stars,
    forks,
    watchers,
    issues,
    pullRequests,
  } = data

  const bundlephobiaFlag = !bundlephobia ? '**' : undefined
  const githubFlag = repository ? undefined : '*'

  return (
    <div>
      <h1 className={css({ textAlign: 'center' })}>{name}</h1>
      <p className={css({ textAlign: 'center', padding: '0 5px' })}>
        {description}
      </p>
      <Spacer size={5} />
      <div
        className={css({
          justifyContent: 'center',
          alignItems: 'center',
          flexDirection: 'row',
        })}>
        <LinkTile title="npm" link={'http://npmjs.com/package/' + name} />
        <Conditional condition={repository}>
          <LinkTile title="github" link={repositoryUrl} />
        </Conditional>
        <Conditional condition={homepage}>
          <LinkTile title="homepage" link={homepage} />
        </Conditional>
        <LinkTile title="unpkg" link={'http://unpkg.com/' + name + '/'} />
      </div>
      <Spacer size={10} />
      <div
        className={css({
          flexDirection: 'row',
          justifyContent: 'center',
        })}>
        <Conditional condition={repository}>
          <Tag color={primaryLanguage.color}>{primaryLanguage.name}</Tag>
        </Conditional>

        <Conditional condition={license}>
          <Tag color="rgb(200, 200, 200)">{license}</Tag>
        </Conditional>
      </div>
      <Spacer size={20} />
      <div
        className={css({
          flexDirection: 'row',
          justifyContent: 'center',
          flexWrap: 'wrap',
        })}>
        <Tile label="Version">{currentVersion}</Tile>
        <Tile label="Last Publish">
          {formatDistance(lastPublishDate, new Date())}
        </Tile>

        <Tile label="Stars" sup={githubFlag}>
          {stars ? stars.toLocaleString() : '-'}
        </Tile>
        <Tile label="Forks" sup={githubFlag}>
          {forks ? forks.toLocaleString() : '-'}
        </Tile>
        <Tile label="Watchers" sup={githubFlag}>
          {watchers ? watchers.toLocaleString() : '-'}
        </Tile>
        <Tile label="Pull Requests" sup={githubFlag}>
          {pullRequests ? pullRequests.toLocaleString() : '-'}
        </Tile>
        <Tile label="Issues" sup={githubFlag}>
          {issues ? issues.toLocaleString() : '-'}
        </Tile>

        <Tile label="Bundle" sup={bundlephobiaFlag}>
          {bundlephobia ? formatBundleSize(bundlephobia.size) : '-'}
        </Tile>
        <Tile label="Bundle (gzip)" sup={bundlephobiaFlag}>
          {bundlephobia ? formatBundleSize(bundlephobia.gzip) : '-'}
        </Tile>
      </div>
      <Spacer size={20} />

      <div className={css({ fontSize: 12, color: 'rgb(100, 100, 100)' })}>
        <Conditional condition={githubFlag}>
          <div className={css({ flexDirection: 'row' })}>
            <sup className={css({ paddingRight: 5 })}>
              *<span className={css({ opacity: 0 })}>*</span>
            </sup>
            <p>
              {!githubStatus
                ? 'You need to login with GitHub in order to see that information.'
                : 'This npm package does not link to a GitHub repository.'}
            </p>
          </div>
        </Conditional>
        <Conditional condition={bundlephobiaFlag}>
          <div className={css({ flexDirection: 'row' })}>
            <sup className={css({ paddingRight: 5 })}>**</sup>
            <p>
              Bundle size statistics are provided by{' '}
              <Link href="https://bundlephobia.com/">BundlePhobia</Link>. Some
              packages can not be bundled and analyzed.
            </p>
          </div>
        </Conditional>
      </div>
    </div>
  )
}
