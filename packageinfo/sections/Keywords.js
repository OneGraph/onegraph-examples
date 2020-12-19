import React from 'react'
import { useFela } from 'react-fela'

import Headline from '../components/Headline'

export default ({ keywords }) => {
  const { css } = useFela()

  return (
    <div>
      <Headline>Keywords</Headline>
      <div
        className={css({
          flexDirection: 'row',
          flexWrap: 'wrap',
        })}>
        {keywords.map(keyword => (
          <div
            key={keyword}
            className={css({
              padding: '4px 10px 5px',
              alignSelf: 'flex-start',
              marginRight: 7,
              fontSize: 14,
              marginTop: 7,
              borderRadius: 5,
              backgroundColor: 'rgba(18, 147, 154, 0.25)',
            })}>
            {keyword}
          </div>
        ))}
      </div>
    </div>
  )
}
