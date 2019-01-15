import { gql } from 'apollo-boost'
import { useState, useContext } from 'react'
import { Query } from 'react-apollo'
import { AuthContext } from 'react-onegraph'

const GET_TWITTER = gql`
  query twitterUser {
    twitter {
      user(screenName: "sgrove") {
        screenName
        followersCount
        description
        name
        location
        profileImageUrl
      }
    }
  }
`

export default () => {
  const { login, status, headers } = useContext(AuthContext)

  if (!status.twitter) {
    return (
      <div>
        <h1>Login with Twitter</h1>
        <p>In order to see your profile, you'll have to login with Twitter.</p>
        <button onClick={() => login('twitter')}>Login with Twitter</button>
      </div>
    )
  }

  return (
    <Query query={GET_TWITTER} context={{ headers }}>
      {({ loading, error, data }) => {
        if (loading) {
          return <div>Loading</div>
        }

        if (error) {
          return <div>Error</div>
        }

        const {
          screenName,
          followersCount,
          description,
          name,
          location,
          profileImageUrl,
        } = data.twitter.user

        return (
          <div>
            <img src={profileImageUrl} />
            <h1>{name}</h1>
            <div>
              <b>@{screenName}</b> from {location}
            </div>
            <p>{description}</p>
          </div>
        )
      }}
    </Query>
  )
}
