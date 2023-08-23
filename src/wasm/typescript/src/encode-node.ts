// Generated file. To retain edits, remove this comment.

import {
  Image,
  BinaryStream,
  InterfaceTypes,
  PipelineOutput,
  PipelineInput,
  runPipelineNode
} from 'itk-wasm'

import EncodeOptions from './encode-options.js'
import EncodeNodeResult from './encode-node-result.js'


import path from 'path'

/**
 * Encode an ITK Image into a High Throughput JPEG2000 codestream
 *
 * @param {Image} image - Input image
 * @param {EncodeOptions} options - options object
 *
 * @returns {Promise<EncodeNodeResult>} - result object
 */
async function encodeNode(
  image: Image,
  options: EncodeOptions = {}
) : Promise<EncodeNodeResult> {

  const desiredOutputs: Array<PipelineOutput> = [
    { type: InterfaceTypes.BinaryStream },
  ]

  const inputs: Array<PipelineInput> = [
    { type: InterfaceTypes.Image, data: image },
  ]

  const args = []
  // Inputs
  const imageName = '0'
  args.push(imageName as string)

  // Outputs
  const outputName = '0'
  args.push(outputName)

  // Options
  args.push('--memory-io')
  if (typeof options.decompositions !== "undefined") {
    args.push('--decompositions', options.decompositions.toString())

  }
  if (typeof options.notReversible !== "undefined") {
    options.notReversible && args.push('--not-reversible')
  }
  if (typeof options.quantizationStep !== "undefined") {
    args.push('--quantization-step', options.quantizationStep.toString())

  }
  if (typeof options.progressionOrder !== "undefined") {
    args.push('--progression-order', options.progressionOrder.toString())

  }
  if (typeof options.tileSize !== "undefined") {
    if(options.tileSize.length < 2) {
      throw new Error('"tile-size" option must have a length > 2')
    }
    args.push('--tile-size')

    options.tileSize.forEach((value) => {
      args.push(value.toString())

    })
  }
  if (typeof options.tileOffset !== "undefined") {
    if(options.tileOffset.length < 2) {
      throw new Error('"tile-offset" option must have a length > 2')
    }
    args.push('--tile-offset')

    options.tileOffset.forEach((value) => {
      args.push(value.toString())

    })
  }
  if (typeof options.blockDimensions !== "undefined") {
    if(options.blockDimensions.length < 2) {
      throw new Error('"block-dimensions" option must have a length > 2')
    }
    args.push('--block-dimensions')

    options.blockDimensions.forEach((value) => {
      args.push(value.toString())

    })
  }

  const pipelinePath = path.join(path.dirname(import.meta.url.substring(7)), '..', 'pipelines', 'encode')

  const {
    returnValue,
    stderr,
    outputs
  } = await runPipelineNode(pipelinePath, args, desiredOutputs, inputs)
  if (returnValue !== 0) {
    throw new Error(stderr)
  }

  const result = {
    output: (outputs[0].data as BinaryStream).data,
  }
  return result
}

export default encodeNode
