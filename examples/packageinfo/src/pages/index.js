import React, { Component, useState, useEffect } from 'react'
import { withRouter } from 'next/router'
import { Query } from 'react-apollo'
import { gql } from 'apollo-boost'
import { useFela } from 'react-fela'

import getFelaRenderer from '../styling/getFelaRenderer'
import FelaProvider from '../styling/FelaProvider'

import preparePackageData from '../utils/preparePackageData'

import Loading from '../sections/Loading'
import Error from '../sections/Error'
import Placeholder from '../sections/Placeholder'
import Footer from '../sections/Footer'
import Info from '../sections/Info'
import Downloads from '../sections/Downloads'
import Maintainers from '../sections/Maintainers'
import Keywords from '../sections/Keywords'
import Versions from '../sections/Versions'
import Dependencies from '../sections/Dependencies'
import Readme from '../sections/Readme'

import Conditional from '../components/Conditional'
import Spacer from '../components/Spacer'

const GET_STATS = gql`
  query npm($package: String!) {
    npm {
      package(name: $package) {
        homepage
        keywords
        description
        keywords
        license
        name
        readme
        readmeFilename
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
        downloadsRange(period: "last-year") {
          downloads {
            downloads
            day
          }
        }
        time {
          versions {
            date
            version
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

const Page = ({ initialSearch }) => {
  const [search, updateSearch] = useState(initialSearch)
  const { css } = useFela()

  useEffect(
    () => {
      window.history.replaceState(
        null,
        '',
        window.location.origin +
          '/' +
          (search.length > 0 ? '?package=' + search : '')
      )
    },
    [search]
  )

  return (
    <div
      className={css({
        backgroundColor: 'white',
        minHeight: '100vh',
      })}>
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
            src="/static/logo.png"
            className={css({ width: 30, height: 30, paddingRight: 5 })}
          />
          OneGraph PackageInfo
        </div>

        <Spacer size={10} />
        <input
          type="text"
          pattern="[a-zA-Z0-9-@_/]"
          placeholder="Search for a package ..."
          value={search}
          onChange={e => updateSearch(e.target.value.trim().toLowerCase())}
          className={css({
            fontSize: 18,
            WebkitAppearance: 'none',
            appearance: 'none',
            padding: '5px 10px',
            width: '100%',
            maxWidth: 860,
            alignSelf: 'center',
            border: '1px solid rgb(200, 200, 200)',
            borderRadius: 2,
            boxSizing: 'border-box',
          })}
        />
      </div>

      {search.length > 0 ? (
        <Query
          query={GET_STATS}
          variables={{ package: search }}
          fetchPolicy="cache-and-network">
          {({ loading, error, data }) => {
            if (loading) {
              return <Loading />
            }

            if (error) {
              return <Error packageName={search} error={error} />
            }

            const packageData = preparePackageData(data.npm.package)

            const {
              name,
              downloads,
              maintainers,
              keywords,
              versions,
              dependencies,
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
                <Info data={packageData} />
                <Spacer size={20} />
                <Downloads downloads={downloads} />
                <Spacer size={20} />
                <Maintainers maintainers={maintainers} />
                <Spacer size={25} />
                <Conditional condition={dependencies.length > 0}>
                  <Dependencies
                    dependencies={dependencies}
                    updateSearch={updateSearch}
                  />
                  <Spacer size={25} />
                </Conditional>
                <Conditional condition={keywords.length > 0}>
                  <Keywords keywords={keywords} />
                  <Spacer size={30} />
                </Conditional>
                <Versions versions={versions} packageName={name} />
                <Spacer size={50} />
                <Readme readme={readme} readmeUrl={readmeUrl} />
                <Spacer size={40} />
              </div>
            )
          }}
        </Query>
      ) : (
        <Placeholder updateSearch={updateSearch} />
      )}
      <Footer />
    </div>
  )
}

export default withRouter(({ router }) => (
  <FelaProvider>
    <Page initialSearch={router.query.package || ''} />
  </FelaProvider>
))
