export default function formatBundleSize(size) {
  return Math.round((size / 1000) * 100) / 100 + 'kb'
}
