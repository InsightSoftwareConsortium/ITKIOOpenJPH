// Generated file. To retain edits, remove this comment.

import {
  Image,
  BinaryStream,
  InterfaceTypes,
  PipelineOutput,
  PipelineInput,
  runPipeline
} from 'itk-wasm'

import EncodeOptions from './encode-options.js'
import EncodeResult from './encode-result.js'

import { getPipelinesBaseUrl } from './pipelines-base-url.js'
import { getPipelineWorkerUrl } from './pipeline-worker-url.js'

import { getDefaultWebWorker } from './default-web-worker.js'

/**
 * Encode an ITK Image into a High Throughput JPEG2000 codestream
 *
 * @param {Image} image - Input image
 * @param {EncodeOptions} options - options object
 *
 * @returns {Promise<EncodeResult>} - result object
 */
async function encode(
  image: Image,
  options: EncodeOptions = {}
) : Promise<EncodeResult> {

  const desiredOutputs: Array<PipelineOutput> = [
    { type: InterfaceTypes.BinaryStream },
  ]

  const inputs: Array<PipelineInput> = [
    { type: InterfaceTypes.Image, data: image },
  ]

  const args = []
  // Inputs
  const imageName = '0'
  args.push(imageName)

  // Outputs
  const outputName = '0'
  args.push(outputName)

  // Options
  args.push('--memory-io')
  if (options.decompositions) {
    args.push('--decompositions', options.decompositions.toString())

  }
  if (options.notReversible) {
    options.notReversible && args.push('--not-reversible')
  }
  if (options.quantizationStep) {
    args.push('--quantization-step', options.quantizationStep.toString())

  }
  if (options.progressionOrder) {
    args.push('--progression-order', options.progressionOrder.toString())

  }
  if (options.tileSize) {
    if(options.tileSize.length < 2) {
      throw new Error('"tile-size" option must have a length > 2')
    }
    args.push('--tile-size')

    await Promise.all(options.tileSize.map(async (value) => {
      args.push(value.toString())

    }))
  }
  if (options.tileOffset) {
    if(options.tileOffset.length < 2) {
      throw new Error('"tile-offset" option must have a length > 2')
    }
    args.push('--tile-offset')

    await Promise.all(options.tileOffset.map(async (value) => {
      args.push(value.toString())

    }))
  }
  if (options.blockDimensions) {
    if(options.blockDimensions.length < 2) {
      throw new Error('"block-dimensions" option must have a length > 2')
    }
    args.push('--block-dimensions')

    await Promise.all(options.blockDimensions.map(async (value) => {
      args.push(value.toString())

    }))
  }

  const pipelinePath = 'encode'

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
    output: (outputs[0]?.data as BinaryStream).data,
  }
  return result
}

export default encode
