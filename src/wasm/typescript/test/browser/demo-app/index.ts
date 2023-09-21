// Generated file. To retain edits, remove this comment.

import * as htj2k from '../../../dist/bundles/htj2k.js'

// Use local, vendored WebAssembly module assets
const pipelinesBaseUrl: string | URL = new URL('/pipelines', document.location.origin).href
htj2k.setPipelinesBaseUrl(pipelinesBaseUrl)
const pipelineWorkerUrl: string | URL | null = new URL('/web-workers/pipeline.worker.js', document.location.origin).href
htj2k.setPipelineWorkerUrl(pipelineWorkerUrl)

import './decode-controller.js'
import './encode-controller.js'

const tabGroup = document.querySelector('sl-tab-group')
const params = new URLSearchParams(window.location.search)
if (params.has('functionName')) {
  const functionName = params.get('functionName')
  tabGroup.show(functionName + '-panel')
} else {
  tabGroup.show('decode-panel')
}
