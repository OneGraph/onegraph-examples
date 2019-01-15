const sumDownloads = arr => arr.reduce((total, { count }) => total + count, 0)

export default function getTotalDownloads(downloads, mode) {
  if (mode === 'week') {
    return sumDownloads(downloads.slice(-7))
  }

  if (mode === 'month') {
    return sumDownloads(downloads.slice(-30))
  }

  return sumDownloads(downloads)
}
