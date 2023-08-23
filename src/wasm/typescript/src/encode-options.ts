// Generated file. To retain edits, remove this comment.

interface EncodeOptions {
  /** Number of wavelet decompositions */
  decompositions?: number

  /** Lossy and not reversible, i.e. not lossless, encoding */
  notReversible?: boolean

  /** Quantization step size for lossy compression. Ignored unless not reversible is enabled. */
  quantizationStep?: number

  /** Progression order. 0 = LRCP, 1 = RLCP, 2 = RPCL, 3 = PCRL, 4 = CPRL */
  progressionOrder?: number

  /** Tile size */
  tileSize?: number[]

  /** Tile offset */
  tileOffset?: number[]

  /** Block dimensions */
  blockDimensions?: number[]

}

export default EncodeOptions
