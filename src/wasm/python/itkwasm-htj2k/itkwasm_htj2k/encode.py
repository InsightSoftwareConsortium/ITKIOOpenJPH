# Generated file. Do not edit.

import os
from typing import Dict, Tuple, Optional, List, Any

from itkwasm import (
    environment_dispatch,
    Image,
    BinaryStream,
)

def encode(
    image: Image,
    decompositions: int = 5,
    not_reversible: bool = False,
    quantization_step: float = 1,
    progression_order: int = 2,
    tile_size: List[int] = [512,512],
    tile_offset: List[int] = [0,0],
    block_dimensions: List[int] = [64,64],
) -> bytes:
    """Encode an ITK Image into a High Throughput JPEG2000 codestream

    :param image: Input image
    :type  image: Image

    :param decompositions: Number of wavelet decompositions
    :type  decompositions: int

    :param not_reversible: Lossy and not reversible, i.e. not lossless, encoding
    :type  not_reversible: bool

    :param quantization_step: Quantization step size for lossy compression. Ignored unless not reversible is enabled.
    :type  quantization_step: float

    :param progression_order: Progression order. 0 = LRCP, 1 = RLCP, 2 = RPCL, 3 = PCRL, 4 = CPRL
    :type  progression_order: int

    :param tile_size: Tile size
    :type  tile_size: int

    :param tile_offset: Tile offset
    :type  tile_offset: int

    :param block_dimensions: Block dimensions
    :type  block_dimensions: int

    :return: Output binary HTJ2K codestream
    :rtype:  bytes
    """
    func = environment_dispatch("itkwasm_htj2k", "encode")
    output = func(image, decompositions=decompositions, not_reversible=not_reversible, quantization_step=quantization_step, progression_order=progression_order, tile_size=tile_size, tile_offset=tile_offset, block_dimensions=block_dimensions)
    return output
