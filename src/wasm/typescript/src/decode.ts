// Generated file. To retain edits, remove this comment.

import {
  Image,
  InterfaceTypes,
  PipelineOutput,
  PipelineInput,
  runPipeline
} from 'itk-wasm'

import DecodeOptions from './decode-options.js'
import DecodeResult from './decode-result.js'


import { getPipelinesBaseUrl } from './pipelines-base-url.js'
import { getPipelineWorkerUrl } from './pipeline-worker-url.js'

/**
 * Decode a High Throughput JPEG2000 codestream and generate an ITK Image
 *
 * @param {Uint8Array} codestream - Input HTJ2K codestream
 * @param {DecodeOptions} options - options object
 *
 * @returns {Promise<DecodeResult>} - result object
 */
async function decode(
  webWorker: null | Worker,
  codestream: Uint8Array,
  options: DecodeOptions = {}
) : Promise<DecodeResult> {

  const desiredOutputs: Array<PipelineOutput> = [
    { type: InterfaceTypes.Image },
  ]

  const inputs: Array<PipelineInput> = [
    { type: InterfaceTypes.BinaryStream, data: { data: codestream }  },
  ]

  const args = []
  // Inputs
  const codestreamName = '0'
  args.push(codestreamName)

  // Outputs
  const imageName = '0'
  args.push(imageName)

  // Options
  args.push('--memory-io')
  if (typeof options.decompositionLevel !== "undefined") {
    args.push('--decomposition-level', options.decompositionLevel.toString())

  }
  if (typeof options.informationOnly !== "undefined") {
    options.informationOnly && args.push('--information-only')
  }

  const pipelinePath = 'decode'

  const {
    webWorker: usedWebWorker,
    returnValue,
    stderr,
    outputs
  } = await runPipeline(webWorker, pipelinePath, args, desiredOutputs, inputs, { pipelineBaseUrl: getPipelinesBaseUrl(), pipelineWorkerUrl: getPipelineWorkerUrl() })
  if (returnValue !== 0) {
    throw new Error(stderr)
  }

  const result = {
    webWorker: usedWebWorker as Worker,
    image: outputs[0].data as Image,
  }
  return result
}

export default decode
