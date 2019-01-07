import React from 'react'
import { useFela } from 'react-fela'

export default ({ keywords }) => {
  const { css } = useFela()

  return (
    <div>
      <h2>Keywords</h2>
      <hr
        className={css({
          height: 1,
          backgroundColor: 'rgb(200, 200, 200)',
          border: 0,
        })}
      />
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
