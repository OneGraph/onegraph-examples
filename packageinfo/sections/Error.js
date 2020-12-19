import React from 'react'
import { useFela } from 'react-fela'

import Spacer from '../components/Spacer'

export default ({ error, packageName }) => {
  const { css, theme } = useFela()

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
