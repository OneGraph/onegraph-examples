import React from 'react'
import { useFela } from 'react-fela'

const keyframe = {
  '0%': {
    transform: 'scale(0.25, 0.25)',
    opacity: 1.0,
  },
  '50%': {
    transform: 'scale(1, 1)',
    opacity: 0.5,
  },
  '99%': {
    transform: 'scale(0.25, 0.25)',
    opacity: 1.0,
  },
}
const scale = {
  animationName: keyframe,
  animationDuration: '2s',
  animationTimingFunction: 'ease-in-out',
  animationIterationCount: 'infinite',
  width: 50,
  height: 50,
  borderRadius: 50,
  transformOrigin: '50% 50%',
  backgroundColor: 'grey',
  transform: 'scale(0.25, 0.25)',
}

// TODO: Cleanup
export default function Loading({ size }) {
  const { css } = useFela()

  return (
    <React.Fragment>
      <div className={css(scale)} />
      <div
        className={css(scale, {
          animationDelay: '1s',
          marginTop: -50,
        })}
      />
    </React.Fragment>
  )
}
