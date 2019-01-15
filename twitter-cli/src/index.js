import chalk from 'chalk'
import fetch from 'node-fetch'
import { APP_ID, AUTH_TOKEN } from '../env'

const userName = process.argv[2]

if (!userName) {
  console.log(
    chalk.red('You have to provide a Twitter user name id e.g. node lib sgrove')
  )
  process.exit(1)
}

const query = `
{
  twitter {
    user(screenName: "${userName}") {
      screenName
      followersCount
      name
      location
      description
    }
  }
}
`

fetch('https://serve.onegraph.com/dynamic?app_id=' + APP_ID, {
  method: 'POST',
  body: JSON.stringify({ query }),
  headers: {
    Authentication: 'Bearer ' + AUTH_TOKEN,
  },
})
  .then(r => r.json())
  .then(({ data, errors }) => {
    if (data.twitter.user) {
      const {
        screenName,
        followersCount,
        name,
        location,
        description,
      } = data.twitter.user

      console.log(
        chalk.bold(name) +
          ' (@' +
          screenName +
          ') from ' +
          chalk.green(location)
      )
      console.log(description)
      console.log(chalk.blue(followersCount) + ' followers')
    } else {
      console.log(chalk.red('No user with the name ' + userName + ' found.'))
    }
  })
  .catch(console.error)
