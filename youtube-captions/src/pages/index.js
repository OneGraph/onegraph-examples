import React, { useState, useEffect, useRef, useContext } from 'react'
import Head from 'next/head'
import { withRouter } from 'next/router'
import { Query } from 'react-apollo'
import { gql } from 'apollo-boost'
import { AuthContext } from 'react-onegraph'
import YouTubePlayer from 'react-player/lib/players/YouTube'

import Loading from '../sections/Loading'
import Error from '../sections/Error'
import Captions from '../sections/Captions'
import NotFound from '../sections/NotFound'
import Placeholder from '../sections/Placeholder'
import Footer from '../sections/Footer'

import Spacer from '../components/Spacer'

import './index.css'

const GET_VIDEO = gql`
  query VideoWithCaptionsQuery($videoId: String!) {
    youTube {
      video(id: $videoId) {
        id
        snippet {
          title
        }
        captions {
          items {
            snippet {
              language
              status
            }
            body
          }
        }
      }
    }
  }
`

function App({ initialVideoId }) {
  const { status, headers, login, logout } = useContext(AuthContext)
  const [videoId, setVideoId] = useState(initialVideoId)
  const youTubeRef = useRef(null)

  // this will update the URL query parameter so that
  // we can share a direct URL to a specific package
  useEffect(
    () => {
      // don't update on mount as this'd break
      // the GitHub authentication redirect
      if (videoId !== initialVideoId) {
        window.history.replaceState(
          null,
          '',
          window.location.origin +
            '/' +
            (videoId.length > 0 ? '?videoId=' + videoId : '')
        )
      }
    },
    // only run if the search input changes
    [videoId]
  )

  return (
    <div
      style={{
        backgroundColor: 'white',
        minHeight: '100vh',
      }}>
      <Head>
        <title>OneGraph YouTube Captions</title>
      </Head>
      <div
        style={{
          padding: 10,
          backgroundColor: 'rgb(235, 235, 235)',
        }}>
        <div
          style={{
            flexDirection: 'row',
            alignItems: 'center',
            justifyContent: 'center',
            fontFamily: 'Roboto, Inter UI, Helvetica, -apple-system',
          }}>
          <img
            src="/static/logo.png"
            style={{ width: 30, height: 30, paddingRight: 5 }}
          />
          OneGraph YouTube Captions
        </div>

        <Spacer size={10} />
        <div
          style={{
            fontSize: 18,
            flexDirection: 'row',
            WebkitAppearance: 'none',
            backgroundColor: 'white',
            appearance: 'none',
            padding: '5px 10px',
            width: '100%',
            maxWidth: 880,
            alignSelf: 'center',
            border: '1px solid rgb(200, 200, 200)',
            borderRadius: 2,
            boxSizing: 'border-box',
            color: 'rgb(180, 180, 180)',
          }}>
          https://www.youtube.com/watch?v=
          <input
            type="text"
            placeholder="VIDEO_ID"
            value={videoId}
            disabled={!status.youtube}
            onChange={e => setVideoId(e.target.value)}
            style={{
              fontSize: 18,
              flex: 1,
              display: 'flex',
              WebkitAppearance: 'none',
              appearance: 'none',
              color: 'black',
              border: 'none',
              outline: 'none',
            }}
          />
        </div>
      </div>
      {!status.youtube ? (
        <div
          style={{
            flex: 1,
            alignSelf: 'center',
            maxWidth: 880,
            width: '100%',
            textAlign: 'center',
            padding: '5vh 10% 40px',
            color: 'rgb(50, 50, 50)',
            fontSize: 18,
          }}>
          <h2>Login with YouTube</h2>
          <br />
          <p>
            In order to fetch YouTube video captions, you must login with
            YouTube.
          </p>
          <br />
          <button
            onClick={() => login('youtube')}
            style={{
              alignSelf: 'center',
              cursor: 'pointer',
              padding: '10px 20px',
              backgroundColor: 'red',
              borderRadius: 5,
              color: 'white',
              border: 0,
              fontSize: 16,
            }}>
            Login with YouTube
          </button>
        </div>
      ) : null}
      {status.youtube && videoId.length > 0 ? (
        <Query
          query={GET_VIDEO}
          // passing the headers to the variables is a neat hack
          // forcing Apollo to refetch the data using the new headers
          variables={{ videoId }}
          context={{ headers }}
          // TODO: investigate why this is required
          // without it won't always update on input change
          // even if the input value is an existing package
          fetchPolicy="cache-and-network"
          // using this to display npm data even if GitHub auth throws
          // otherwise Apollo would not pass any data at all
          errorPolicy="all">
          {({ loading, error, data }) => {
            if (loading) {
              return <Loading />
            }

            if (error) {
              return <Error error={error} videoId={videoId} />
            }

            if (!data.youTube.video) {
              return <NotFound videoId={videoId} />
            }

            return (
              <div
                style={{
                  padding: 10,
                  paddingBottom: 50,
                  flex: 1,
                  width: '100%',
                  maxWidth: 900,
                  alignSelf: 'center',
                }}>
                <div
                  style={{
                    backgroundColor: 'rgba(240, 240, 240, 0.4)',
                    color: 'rgb(80, 80, 80)',
                    padding: 10,
                    fontSize: 14,
                  }}>
                  <div
                    style={{
                      justifyContent: 'space-between',
                      textAlign: 'center',
                      flexDirection: 'row',
                    }}>
                    You are logged into YouTube.
                    <Spacer size={10} />
                    <span
                      onClick={() => logout('youtube')}
                      style={{
                        cursor: 'pointer',
                        color: 'red',
                      }}>
                       Logout from YouTube
                    </span>
                  </div>
                </div>
                <Spacer size={10} />
                <h1 style={{ textAlign: 'center', fontSize: 24 }}>
                  {data.youTube.video.snippet.title}
                </h1>
                <Spacer size={10} />
                <YouTubePlayer
                  width="100%"
                  url={`https://www.youtube.com/watch?v=${videoId}`}
                  ref={youTubeRef}
                  controls
                />
                <Spacer size={10} />
                <Captions
                  data={data.youTube.video}
                  videoId={videoId}
                  youTubeRef={youTubeRef}
                />
              </div>
            )
          }}
        </Query>
      ) : status.youtube && videoId.length === 0 ? (
        <Placeholder setVideoId={setVideoId} />
      ) : null}
      <Footer />
    </div>
  )
}

export default withRouter(({ router }) => (
  <App initialVideoId={router.query.videoId || ''} />
))
