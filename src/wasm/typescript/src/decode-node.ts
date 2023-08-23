// Generated file. To retain edits, remove this comment.

import {
  Image,
  InterfaceTypes,
  PipelineOutput,
  PipelineInput,
  runPipelineNode
} from 'itk-wasm'

import DecodeOptions from './decode-options.js'
import DecodeNodeResult from './decode-node-result.js'


import path from 'path'

/**
 * Decode a High Throughput JPEG2000 codestream and generate an ITK Image
 *
 * @param {Uint8Array} codestream - Input HTJ2K codestream
 * @param {DecodeOptions} options - options object
 *
 * @returns {Promise<DecodeNodeResult>} - result object
 */
async function decodeNode(
  codestream: Uint8Array,
  options: DecodeOptions = {}
) : Promise<DecodeNodeResult> {

  const desiredOutputs: Array<PipelineOutput> = [
    { type: InterfaceTypes.Image },
  ]

  const inputs: Array<PipelineInput> = [
    { type: InterfaceTypes.BinaryStream, data: { data: codestream }  },
  ]

  const args = []
  // Inputs
  const codestreamName = '0'
  args.push(codestreamName as string)

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

  const pipelinePath = path.join(path.dirname(import.meta.url.substring(7)), '..', 'pipelines', 'decode')

  const {
    returnValue,
    stderr,
    outputs
  } = await runPipelineNode(pipelinePath, args, desiredOutputs, inputs)
  if (returnValue !== 0) {
    throw new Error(stderr)
  }

  const result = {
    image: outputs[0].data as Image,
  }
  return result
}

export default decodeNode
