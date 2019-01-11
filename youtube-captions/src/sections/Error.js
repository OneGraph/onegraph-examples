import Spacer from '../components/Spacer'

export default function Error({ error, videoId }) {
  return (
    <div
      style={{
        flex: 1,
        alignSelf: 'center',
        maxWidth: 880,
        width: '100%',
        padding: '5vh 10px 40px',
        color: 'rgb(50, 50, 50)',
        fontSize: 18,
      }}>
      <h2 style={{ color: 'red' }}>Error!</h2>
      <p>
        An error occured while trying to load the video: <b>{videoId}</b>
        {/* <br />
        <br />
        {error.message} */}
      </p>
      <Spacer size={10} />
      <hr
        style={{
          height: 1,
          backgroundColor: 'rgb(200, 200, 200)',
          border: 0,
        }}
      />
      <Spacer size={10} />
      <div style={{ flex: 1, alignSelf: 'flex-start' }}>
        <pre
          style={{
            width: '100%',
            fontSize: 16,
            overflow: 'auto',
            WebkitOverflowScrolling: 'smooth',
          }}>
          {JSON.stringify(error, null, 2)}
        </pre>
      </div>
    </div>
  )
}
