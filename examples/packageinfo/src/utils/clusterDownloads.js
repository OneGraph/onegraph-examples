export default function clusterDownloads(downloads, mode) {
  if (mode === 'week') {
    // only get the last 7 days of stats
    return downloads.slice(-7)
  }

  if (mode === 'month') {
    // only get the last 30 days of stats
    return downloads.slice(-30)
  }

  // accumulate 15 sets to 1 data point
  const newData = []
  while (downloads.length > 0) {
    const data = downloads.splice(0, 15)

    const value = data.reduce((total, point) => total + point.downloads, 0) * 2
    const date = data[Math.floor(data.length / 2)].day

    newData.push({
      day: date,
      downloads: value,
    })
  }

  return newData
}
