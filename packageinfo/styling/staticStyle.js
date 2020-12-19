export default [
  {
    selector: '*',
    style: {
      margin: 0,
      padding: 0,
    },
  },
  {
    selector: '::-webkit-scrollbar',
    style: {
      display: 'none',
    },
  },
  {
    selector: 'div',
    style: {
      display: 'flex',
      alignSelf: 'stretch',
      flexDirection: 'column',
      flexShrink: 0,
      maxWidth: '100%',
      boxSizing: 'border-box',
    },
  },
  {
    selector: 'body',
    style: {
      backgroundColor: 'rgb(235, 235, 235)',
      fontFamily: 'Inter, Helvetica Neue,-apple-system',
      fontSize: 16,
    },
  },
  {
    selector: 'html, body, #__next',
    style: {
      minHeight: '100%',
    },
  },
]
