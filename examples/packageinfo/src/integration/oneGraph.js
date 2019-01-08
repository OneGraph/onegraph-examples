import OneGraphAuth from 'onegraph-auth'

import { APP_ID } from '../../env'

let auth
export function getAuth() {
  if (!process.browser) {
    return auth
  }

  if (!auth) {
    auth = new OneGraphAuth({
      appId: APP_ID,
    })

    window.auth = auth
  }

  return auth
}

export function loginGithub(onSuccess) {
  const auth = getAuth()

  auth.login('github').then(() => {
    auth.isLoggedIn('github').then(isLoggedIn => {
      if (isLoggedIn) {
        console.log('Successfully logged in to GitHub.')
        if (onSuccess) {
          onSuccess()
        }
      } else {
        console.log('Did not grant auth for GitHub.')
      }
    })
  })
}
