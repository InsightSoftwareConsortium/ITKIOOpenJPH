// Generated file. To retain edits, remove this comment.

import { WorkerPoolFunctionOption } from 'itk-wasm'

interface DecodeOptions extends WorkerPoolFunctionOption {
  /** Decomposition level to decode. 0 is the highest resolution. */
  decompositionLevel?: number

  /** Only read metadata for the image but do not decode the pixel data. */
  informationOnly?: boolean

}

export default DecodeOptions
