import React from 'react'
import { useFela } from 'react-fela'

import { loginGithub } from '../integration/oneGraph'

import Spacer from '../components/Spacer'

export default ({ error, packageName }) => {
  const { css, theme } = useFela()

  // Error handling like a boss
  let isMissingAuth =
    error.graphQLErrors &&
    error.graphQLErrors[0] &&
    error.graphQLErrors[0].extensions &&
    error.graphQLErrors[0].extensions.type === 'auth/missing-auth'

  if (
    !isMissingAuth &&
    error.graphQLErrors &&
    error.graphQLErrors[0] &&
    error.graphQLErrors[0].message
  ) {
    try {
      const json = JSON.parse(error.graphQLErrors[0].message)

      if (json.message === 'Bad credentials') {
        isMissingAuth = true
      }
    } catch (e) {}
  }

  if (isMissingAuth) {
    return (
      <div
        className={css({
          flex: 1,
          alignSelf: 'center',
          maxWidth: 880,
          width: '100%',
          textAlign: 'center',
          padding: '5vh 10% 40px',
          color: 'rgb(50, 50, 50)',
          fontSize: 18,
        })}>
        <h2>Login with Github</h2>
        <br />
        <p>
          In order to fetch the relevant package information for{' '}
          <b>{packageName}</b>, you have to authenticate with Github once.
        </p>
        <br />
        <button
          onClick={() => loginGithub(() => location.reload())}
          className={css({
            alignSelf: 'center',
            cursor: 'pointer',
            padding: '10px 20px',
            backgroundColor: theme.colors.primary,
            borderRadius: 5,
            color: 'white',
            border: 0,
            fontSize: 16,
          })}>
          Login with Github
        </button>
      </div>
    )
  }

  return (
    <div
      className={css({
        flex: 1,
        alignSelf: 'center',
        maxWidth: 880,
        width: '100%',
        padding: '5vh 10px 40px',
        color: 'rgb(50, 50, 50)',
        fontSize: 18,
      })}>
      <h2 className={css({ color: 'red' })}>Error!</h2>
      <p>
        An error occured while trying to fetch information for the following
        package: <b>{packageName}</b>
        <br />
        <br />
        {error.message}
      </p>
      <Spacer size={10} />
      <hr
        className={css({
          height: 1,
          backgroundColor: 'rgb(200, 200, 200)',
          border: 0,
        })}
      />
      <Spacer size={10} />
      <div className={css({ flex: 1, alignSelf: 'flex-start' })}>
        <pre
          className={css({
            width: '100%',
            fontSize: 16,
            overflow: 'auto',
          })}>
          {JSON.stringify(error, null, 2)}
        </pre>
      </div>
    </div>
  )
}
