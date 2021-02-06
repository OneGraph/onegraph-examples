import React, { Component, useState, useEffect, useContext } from 'react'
import Head from 'next/head'
import { useQuery } from 'urql'
import { useFela } from 'react-fela'
import { AuthContext } from 'react-onegraph'

import getFelaRenderer from '../styling/getFelaRenderer'
import FelaProvider from '../styling/FelaProvider'

import preparePackageData from '../utils/preparePackageData'
import padNumber from '../utils/padNumber'

import Loading from '../sections/Loading'
import Error from '../sections/Error'
import Placeholder from '../sections/Placeholder'
import Footer from '../sections/Footer'
import Info from '../sections/Info'
import Downloads from '../sections/Downloads'
import BundleSize from '../sections/BundleSize'
import Maintainers from '../sections/Maintainers'
import Keywords from '../sections/Keywords'
import Versions from '../sections/Versions'
import Dependencies from '../sections/Dependencies'
import ReadmeFile from '../sections/ReadmeFile'

import Conditional from '../components/Conditional'
import Spacer from '../components/Spacer'

// one request for the all data including 2 different services
// this is both convenient and mind-blowing
const GET_STATS = `
  query npm($package: String!, $startDate: String!, $endDate: String!) {
    npm {
      package(name: $package) {
        homepage
        keywords
        description
        keywords
        name
        readme
        readmeFilename
        license {
          type
        }
        maintainers {
          name
        }
        versions {
          version
          dependencies {
            name
            version
          }
        }
        downloads {
          period(startDate: $startDate, endDate: $endDate) {
            perDay {
              day
              count
            }
          }
        }
        time {
          versions {
            date
            version
          }
        }
        bundlephobia {
          gzip
          size
          history {
            gzip
            size
            version
          }
          dependencySizes {
            approximateSize
            name
          }
        }
        repository {
          url
          sourceRepository {
            ... on GitHubRepository {
              name
              url
              homepageUrl
              defaultBranchRef {
                name
              }
              stargazers {
                totalCount
              }
              watchers {
                totalCount
              }
              issues(states: OPEN) {
                totalCount
              }
              forks {
                totalCount
              }
              pullRequests(states: OPEN) {
                totalCount
              }
              primaryLanguage {
                name
                color
              }
            }
          }
        }
      }
    }
  }
`

const PackageInfo = ({
  fetching,
  error,
  data,
  search,
  updateSearch,
  status,
  login,
  logout,
}) => {
  const { css, theme } = useFela()

  if (fetching) {
    return <Loading />
  }

  if (!data || !data.npm.package) {
    if (error) {
      return <Error packageName={search} error={error} />
    }
    return (
      <Error packageName={search} error={{ message: 'Package not found.' }} />
    )
  }

  const packageData = preparePackageData({ ...data.npm.package })

  const {
    name,
    downloads,
    maintainers,
    keywords,
    versions,
    dependencies,
    bundlephobia,
    bundlephobiaHistory,
    readme,
    readmeUrl,
  } = packageData

  return (
    <div
      className={css({
        padding: 10,
        flex: 1,
        width: '100%',
        maxWidth: 900,
        alignSelf: 'center',
      })}>
      <div
        className={css({
          backgroundColor: 'rgba(240, 240, 240, 0.4)',
          color: 'rgb(80, 80, 80)',
          padding: 10,
          fontSize: 14,
        })}>
        {!status.github ? (
          <div
            className={css({
              justifyContent: 'space-between',
              textAlign: 'center',
              '@media (min-width: 640px)': {
                flexDirection: 'row',
              },
            })}>
            You are not logged into GitHub. Some information may not be missing.
            <Spacer size={10} />
            <span
              onClick={() => login('github')}
              className={css({
                cursor: 'pointer',

                color: theme.colors.primary,
              })}>
               Login with Github
            </span>
          </div>
        ) : (
          <div
            className={css({
              justifyContent: 'space-between',
              textAlign: 'center',
              '@media (min-width: 640px)': {
                flexDirection: 'row',
              },
            })}>
            You are logged into GitHub. You may log out at any time.
            <Spacer size={10} />
            <span
              onClick={() => logout('github')}
              className={css({
                cursor: 'pointer',

                color: theme.colors.primary,
              })}>
               Logout with Github
            </span>
          </div>
        )}
      </div>
      <Spacer size={10} />
      <Info data={packageData} githubStatus={status.github} />
      <Spacer size={20} />
      <Downloads downloads={downloads} />
      <Spacer size={20} />
      <Maintainers maintainers={maintainers} />
      <Spacer size={25} />
      <Conditional condition={dependencies.length > 0}>
        <Dependencies dependencies={dependencies} updateSearch={updateSearch} />
        <Spacer size={25} />
      </Conditional>
      <Conditional condition={keywords.length > 0}>
        <Keywords keywords={keywords} />
        <Spacer size={30} />
      </Conditional>
      <Conditional condition={bundlephobia}>
        <BundleSize bundlephobiaHistory={bundlephobiaHistory} />

        <Spacer size={25} />
      </Conditional>
      <Versions versions={versions} packageName={name} />
      <Spacer size={50} />
      <ReadmeFile readme={readme} readmeUrl={readmeUrl} />
      <Spacer size={40} />
    </div>
  )
}

const getDateString = date =>
  [
    date.getFullYear(),
    padNumber(date.getMonth() + 1),
    padNumber(date.getDate()),
  ].join('-')

export default function Page({ initialSearch = '' }) {
  const onegraph = useContext(AuthContext)
  const [search, updateSearch] = useState(initialSearch)
  const { css, theme } = useFela()

  // this will update the URL query parameter so that
  // we can share a direct URL to a specific package
  useEffect(
    () => {
      window.onpopstate = () => {
        // weird browser behaviour dunno what to do
        const pkg = history.state.package
          ? history.state.package
          : history.state.url.substr(10, history.state.url.length)
        updateSearch(pkg)
      }

      // don't update on mount as this'd break
      // the GitHub authentication redirect
      if (search !== initialSearch) {
        window.history.replaceState(
          { package: search },
          '',
          window.location.origin +
            '/' +
            (search.length > 0 ? '?package=' + search : '')
        )
      }
    },
    // only run if the search input changes
    [search]
  )

  const [res] = useQuery({
    query: GET_STATS,
    pause: search.length === 0,
    variables: {
      package: search,
      endDate: getDateString(new Date()),
      startDate: getDateString(
        new Date(new Date().setFullYear(new Date().getFullYear() - 1))
      ),
    },
  })

  return (
    <div
      className={css({
        backgroundColor: 'white',
        minHeight: '100vh',
      })}>
      <Head>
        <title>PackageInfo | npm package information using OneGraph</title>
      </Head>
      <div
        className={css({
          padding: 10,
          backgroundColor: 'rgb(235, 235, 235)',
        })}>
        <div
          className={css({
            flexDirection: 'row',
            alignItems: 'center',
            justifyContent: 'center',
            fontFamily: 'Roboto, Inter UI, Helvetica, -apple-system',
          })}>
          <img
            src="/logo.png"
            className={css({ width: 30, height: 30, paddingRight: 5 })}
          />
          OneGraph PackageInfo
        </div>

        <Spacer size={10} />
        <input
          type="text"
          placeholder="Search for a package ..."
          value={search}
          onChange={e =>
            // we only allow valid package name input
            e.target.value.match(/[a-zA-Z0-9-@_\/]*/) !== null &&
            updateSearch(e.target.value.trim().toLowerCase())
          }
          className={css({
            fontSize: 18,
            WebkitAppearance: 'none',
            appearance: 'none',
            padding: '5px 10px',
            width: '100%',
            maxWidth: 880,
            alignSelf: 'center',
            border: '1px solid rgb(200, 200, 200)',
            borderRadius: 2,
            boxSizing: 'border-box',
          })}
        />
      </div>

      {search.length > 0 ? (
        <PackageInfo
          {...res}
          {...onegraph}
          search={search}
          updateSearch={updateSearch}
        />
      ) : (
        <Placeholder updateSearch={updateSearch} />
      )}
      <Footer />
    </div>
  )
}

Page.getInitialProps = ({ query }) => ({
  initialSearch: query.package || '',
})
