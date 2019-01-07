import React, { Component } from 'react'
import { createStore } from 'alveron'
import { formatDistance } from 'date-fns'
import { useFela } from 'react-fela'

import Tile from '../components/Tile'
import LinkTile from '../components/LinkTile'
import Tag from '../components/Tag'
import Conditional from '../components/Conditional'
import Spacer from '../components/Spacer'
import Link from '../components/Link'

import formatBundleSize from '../utils/formatBundleSize'

const { Wrapper } = createStore({
  model: {
    isLoading: false,
    size: undefined,
    gzip: undefined,
  },
  actions: {
    reset: (state, loading) => ({
      isLoading: true,
      size: undefined,
      gzip: undefined,
    }),
    setLoading: (state, loading) => ({
      ...state,
      isLoading: loading,
    }),
    setSizes: (state, size, gzip) => ({
      ...state,
      size,
      gzip,
    }),
  },
  effects: {
    fetchSizes: (actions, packageName) => {
      actions.reset(true)

      fetch(`https://bundlephobia.com/api/size?package=${packageName}`)
        .then(res => res.json())
        .then(data => actions.setSizes(data.size, data.gzip))
        .finally(() => actions.setLoading(false))
    },
  },
})

class BundleSize extends Component {
  componentDidMount() {
    this.props.fetchSizes(this.props.packageName)
  }

  componentDidUpdate(prevProps) {
    if (prevProps.packageName !== this.props.packageName) {
      this.props.fetchSizes(this.props.packageName)
    }
  }

  render() {
    return null
  }
}

export default function Info({ data }) {
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
    homepage,
    stars,
    forks,
    watchers,
    issues,
    pullRequests,
  } = data

  return (
    <Wrapper>
      {({ state, effects }) => {
        const { isLoading, size, gzip } = state

        const bundlephobiaFlag = !isLoading && !size ? '**' : undefined
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
                {state.isLoading
                  ? 'Loading...'
                  : state.size
                  ? formatBundleSize(size)
                  : '-'}
              </Tile>
              <Tile label="Bundle (gzip)" sup={bundlephobiaFlag}>
                {state.isLoading
                  ? 'Loading...'
                  : state.gzip
                  ? formatBundleSize(gzip)
                  : '-'}
              </Tile>

              <BundleSize
                packageName={name}
                fetchSizes={effects.fetchSizes}
                {...state}
              />
            </div>
            <Spacer size={20} />

            <div className={css({ fontSize: 12, color: 'rgb(100, 100, 100)' })}>
              <Conditional condition={githubFlag}>
                <div className={css({ flexDirection: 'row' })}>
                  <sup className={css({ paddingRight: 5 })}>
                    *<span className={css({ opacity: 0 })}>*</span>
                  </sup>
                  <p>
                    This npm package does not link to a Github or any repository
                    at all.
                  </p>
                </div>
              </Conditional>
              <Conditional condition={bundlephobiaFlag}>
                <div className={css({ flexDirection: 'row' })}>
                  <sup className={css({ paddingRight: 5 })}>**</sup>
                  <p>
                    Bundle size statistics are provided by{' '}
                    <Link href="https://bundlephobia.com/">BundlePhobia</Link>.
                    Some packages can not be bundled and analyzed.
                  </p>
                </div>
              </Conditional>
            </div>
          </div>
        )
      }}
    </Wrapper>
  )
}
