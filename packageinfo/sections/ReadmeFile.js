import React, { createElement } from 'react'
import Markdown from 'react-markdown-github'
import renderers from 'react-markdown-github-renderers'

export default function ReadmeFile({ readme, readmeUrl }) {
  return (
    <div>
      <Markdown
        source={readme}
        sourceUri={readmeUrl}
        escapeHtml={false}
        renderers={renderers}
      />
    </div>
  )
}
