import 'isomorphic-fetch'
import { createClient } from 'urql'

import { APP_ID } from '../../env'

export default createClient({
  url: 'https://serve.onegraph.com/dynamic?show_metrics=true&app_id=' + APP_ID,
})
