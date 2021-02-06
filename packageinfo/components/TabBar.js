import React from 'react'
import { useFela } from 'react-fela'

export default function TabBar({ tabs, activeTab }) {
  const { css, theme } = useFela()

  const tabItems = tabs.map(({ key, name, action }) => (
    <div
      key={name}
      onClick={action}
      className={css({
        flex: 1,
        padding: 8,
        marginTop: -1,
        textAlign: 'center',
        borderTopWidth: 3,
        borderTopStyle: 'solid',
        color: key === activeTab ? theme.colors.primary : 'black',
        borderTopColor:
          key === activeTab ? theme.colors.primary : 'transparent',
        cursor: 'pointer',
      })}>
      {name}
    </div>
  ))

  return <div className={css({ flexDirection: 'row' })}>{tabItems}</div>
}
