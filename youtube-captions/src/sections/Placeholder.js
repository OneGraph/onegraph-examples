export default function Placeholder({ setVideoId }) {
  return (
    <div
      style={{
        flex: 1,
        textAlign: 'center',
        paddingTop: '10vh',
        paddingLeft: '10%',
        paddingRight: '10%',
        color: 'rgb(180, 180, 180)',
        fontSize: 22,
      }}>
      Enter a video id to see the captions.
      <br />
      <br />
      <span style={{ fontSize: 18 }}>
        Try{' '}
        <span
          onClick={() => setVideoId('t6CRZ-iG39g')}
          style={{ color: 'rgb(150, 150, 150)', cursor: 'pointer' }}>
          <i>t6CRZ-iG39g</i>
        </span>
        {' '}
        for example.
      </span>
    </div>
  )
}
