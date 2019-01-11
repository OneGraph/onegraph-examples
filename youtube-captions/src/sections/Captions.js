import { Fragment, useState } from 'react'

import Spacer from '../components/Spacer'

export default function Captions({ youTubeRef, data, videoId }) {
  const [captionSearch, setCaptionSearch] = useState('')

  const normalizedCaptionSearch = captionSearch.trim().toLowerCase()

  const captions = data.captions.items.find(
    caption =>
      caption.snippet.status === 'serving' && caption.snippet.language === 'en'
  )

  const captionLines =
    captions &&
    captions.body &&
    captions.body
      .split('\n\n')

      .map(line => {
        const [range, label = ''] = line.split('\n')
        return {
          timestamp: range.split(',')[0],
          label: label.toLowerCase().trim(),
        }
      })

  const filteredCaptionLines =
    captionLines &&
    captionLines.filter(
      line => line.label && line.label.indexOf(normalizedCaptionSearch) !== -1
    )

  return (
    <Fragment>
      <h2>Captions</h2>
      <hr
        style={{
          height: 1,
          backgroundColor: 'rgb(200, 200, 200)',
          border: 0,
        }}
      />
      <Spacer size={10} />
      <input
        type="text"
        value={captionSearch}
        placeholder="Filter captions"
        onChange={e => setCaptionSearch(e.target.value)}
        style={{
          fontSize: 18,
          WebkitAppearance: 'none',
          appearance: 'none',
          padding: '5px 10px',
          width: '100%',
          alignSelf: 'center',
          border: '1px solid rgb(200, 200, 200)',
          borderRadius: 2,
          boxSizing: 'border-box',
        }}
      />
      <Spacer size={5} />
      {normalizedCaptionSearch && captionLines.length > 0 ? (
        <div
          style={{
            display: 'block',
            color: 'rgb(100,100,100)',
            fontSize: 14,
          }}>
          Showing <b>{filteredCaptionLines.length}</b> captions of{' '}
          <b>{captionLines.length}</b> total captions
        </div>
      ) : null}
      <Spacer size={10} />
      {captionLines ? (
        <div
          style={{
            maxHeight: 500,
            overflow: 'scroll',
            WebkitOverflowScrolling: 'auto',
            borderBottom: '1px solid rgb(200, 200, 200)',
          }}>
          {filteredCaptionLines.map(({ timestamp, label }, index) => (
            <div
              key={timestamp + label}
              tabInde={index + 10}
              onClick={() => {
                const [hour, minute, second] = timestamp
                  .split(':')
                  .map(parseFloat)
                const wasPlaying = youTubeRef.current.player.isPlaying

                youTubeRef.current.seekTo(hour * 60 * 60 + minute * 60 + second)

                if (wasPlaying) {
                  youTubeRef.current.getInternalPlayer().playVideo()
                }
              }}
              style={{
                cursor: 'pointer',
                flexDirection: 'row',
                borderBottomStyle: 'solid',
                borderBottomColor: 'rgb(230, 230, 230)',
                borderBottomWidth:
                  filteredCaptionLines.length - 1 === index ? 0 : 1,
                padding: '5px 0',
              }}>
              <span
                style={{
                  color: 'grey',
                  padding: '0 10px 0 0',
                  WebkitFontFeatureSettings: '"tnum"',
                  MozFontFeatureSettings: '"tnum"',
                  fontFeatureSettings: '"tnum"',
                }}>
                {timestamp}
              </span>
              <span
                dangerouslySetInnerHTML={{
                  __html: label
                    .split(normalizedCaptionSearch)
                    .join('<b>' + normalizedCaptionSearch + '</b>'),
                }}
              />
            </div>
          ))}
        </div>
      ) : (
        <div style={{ paddingTop: 10, textAlign: 'center' }}>
          No captions found for the video <b>{videoId}</b>.
        </div>
      )}
    </Fragment>
  )
}
