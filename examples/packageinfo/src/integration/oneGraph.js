import OneGraphAuth from 'onegraph-auth'

export const APP_ID = '55e199e4-fc81-4887-b5ab-dc3f35fb71cf'

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
