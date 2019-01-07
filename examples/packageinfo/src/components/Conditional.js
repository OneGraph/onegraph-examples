import React from 'react'

export default function Conditional({ condition, children }) {
  return condition ? children : null
}
