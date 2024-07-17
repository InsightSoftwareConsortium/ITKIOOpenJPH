// Generated file. To retain edits, remove this comment.

import * as htj2k from '../../../dist/index.js'
globalThis.htj2k = htj2k

// Use local, vendored WebAssembly module assets
const viteBaseUrl = import.meta.env.BASE_URL
const pipelinesBaseUrl: string | URL = new URL(`${viteBaseUrl}pipelines`, document.location.origin).href
htj2k.setPipelinesBaseUrl(pipelinesBaseUrl)


const params = new URLSearchParams(window.location.search)
if (!params.has('functionName')) {
  params.set('functionName', 'decode')
  const url = new URL(document.location)
  url.search = params
  window.history.replaceState({ functionName: 'decode' }, '', url)
}
import './decode-controller.js'
import './encode-controller.js'

