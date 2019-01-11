import LoadingSpinner from '../components/Loading'

export default function Loading() {
  return (
    <div
      style={{
        flex: 1,
        paddingTop: '10vh',
        alignSelf: 'center',
      }}>
      <LoadingSpinner />
    </div>
  )
}
