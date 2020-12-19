export default function calculateVersionScore(version) {
  return version
    .split('.')
    .reverse()
    .reduce(
      (sum, val, index) => (sum += parseInt(val) * Math.pow(10, index + 1)),
      0
    )
}
