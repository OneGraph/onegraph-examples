export default function padNumber(value) {
  if (value < 10) {
    return '0' + value
  }

  return value
}
