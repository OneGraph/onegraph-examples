import 'isomorphic-fetch'
import { createClient } from 'urql'

const APP_ID = process.env.APP_ID

export default createClient({
  url: 'https://serve.onegraph.com/dynamic?show_metrics=true&app_id=' + APP_ID,
})
