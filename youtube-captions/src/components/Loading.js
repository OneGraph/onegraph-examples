import React, { Fragment } from 'react'

// TODO: Cleanup
export default function Loading({ size }) {
  return (
    <Fragment>
      <div
        style={{
          animationName: 'loading',
          animationDuration: '2s',
          animationTimingFunction: 'ease-in-out',
          animationIterationCount: 'infinite',
          position: 'relative',
          left: -25,
          width: 50,
          height: 50,
          borderRadius: 50,
          transformOrigin: '50% 50%',
          backgroundColor: 'grey',
          transform: 'scale(0.25, 0.25)',
        }}
      />
      <div
        style={{
          animationName: 'loading',
          animationDuration: '2s',
          animationTimingFunction: 'ease-in-out',
          animationIterationCount: 'infinite',
          animationDelay: '1s',
          marginTop: -50,
          position: 'relative',
          left: -25,
          width: 50,
          height: 50,
          borderRadius: 50,
          transformOrigin: '50% 50%',
          backgroundColor: 'grey',
          transform: 'scale(0.25, 0.25)',
        }}
      />
    </Fragment>
  )
}
