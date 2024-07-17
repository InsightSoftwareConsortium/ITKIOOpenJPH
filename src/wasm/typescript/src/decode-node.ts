// Generated file. To retain edits, remove this comment.

import {
  Image,
  InterfaceTypes,
  PipelineOutput,
  PipelineInput,
  runPipelineNode
} from 'itk-wasm'

import DecodeNodeOptions from './decode-node-options.js'
import DecodeNodeResult from './decode-node-result.js'

import path from 'path'
import { fileURLToPath } from 'url'

/**
 * Decode a High Throughput JPEG2000 codestream and generate an ITK Image
 *
 * @param {Uint8Array} codestream - Input HTJ2K codestream
 * @param {DecodeNodeOptions} options - options object
 *
 * @returns {Promise<DecodeNodeResult>} - result object
 */
async function decodeNode(
  codestream: Uint8Array,
  options: DecodeNodeOptions = {}
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

  const pipelinePath = path.join(path.dirname(fileURLToPath(import.meta.url)), 'pipelines', 'decode')

  const {
    returnValue,
    stderr,
    outputs
  } = await runPipelineNode(pipelinePath, args, desiredOutputs, inputs)
  if (returnValue !== 0 && stderr !== "") {
    throw new Error(stderr)
  }

  const result = {
    image: outputs[0]?.data as Image,
  }
  return result
}

export default decodeNode
