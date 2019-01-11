export default function NotFound({ videoId }) {
  return (
    <div
      style={{
        flex: 1,
        textAlign: 'center',
        paddingTop: '10vh',
        paddingLeft: '10%',
        paddingRight: '10%',
        flexDirection: 'row',
        justifyContent: 'center',
        display: 'block',
        fontSize: 18,
      }}>
      A video with the id{' '}
      <b>{videoId}</b> does not exist.
      <br />
      Please try another one.
    </div>
  )
}
