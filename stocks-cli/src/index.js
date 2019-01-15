import chalk from 'chalk'
import fetch from 'node-fetch'
import { APP_ID } from '../env'

const stockId = process.argv[2]

if (!stockId) {
  console.log(
    chalk.red('You have to provide a stock id e.g. node src/server AAPL')
  )
  process.exit(1)
}

const query = `
{
  stockDemo {
    quote(ticker: "${stockId}") {
      changePercent
      latestPrice
      companyName
    }
  }
}
`

fetch('https://serve.onegraph.com/dynamic?app_id=' + APP_ID, {
  method: 'POST',
  body: JSON.stringify({ query }),
})
  .then(r => r.json())
  .then(({ data }) => {
    if (data.stockDemo.quote) {
      const { changePercent, latestPrice, companyName } = data.stockDemo.quote

      const arrow = changePercent > 0 ? '⇧' : '⇩'

      console.log('_'.repeat(companyName.length))
      console.log(companyName)
      console.log(latestPrice)
      console.log(
        chalk[changePercent > 0 ? 'green' : 'red'](
          Math.round(changePercent * 10000) / 100 + '% ' + arrow
        )
      )
    } else {
      console.log(
        chalk.red('No stock with the stock id ' + stockId + ' exists.')
      )
    }
  })
  .catch(console.error)
