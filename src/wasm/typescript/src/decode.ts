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

import { getDefaultWebWorker } from './default-web-worker.js'

/**
 * Decode a High Throughput JPEG2000 codestream and generate an ITK Image
 *
 * @param {Uint8Array} codestream - Input HTJ2K codestream
 * @param {DecodeOptions} options - options object
 *
 * @returns {Promise<DecodeResult>} - result object
 */
async function decode(
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
  if (options.decompositionLevel) {
    args.push('--decomposition-level', options.decompositionLevel.toString())

  }
  if (options.informationOnly) {
    options.informationOnly && args.push('--information-only')
  }

  const pipelinePath = 'decode'

  let workerToUse = options?.webWorker
  if (workerToUse === undefined) {
    workerToUse = await getDefaultWebWorker()
  }
  const {
    webWorker: usedWebWorker,
    returnValue,
    stderr,
    outputs
  } = await runPipeline(pipelinePath, args, desiredOutputs, inputs, { pipelineBaseUrl: getPipelinesBaseUrl(), pipelineWorkerUrl: getPipelineWorkerUrl(), webWorker: workerToUse, noCopy: options?.noCopy })
  if (returnValue !== 0 && stderr !== "") {
    throw new Error(stderr)
  }

  const result = {
    webWorker: usedWebWorker as Worker,
    image: outputs[0]?.data as Image,
  }
  return result
}

export default decode
