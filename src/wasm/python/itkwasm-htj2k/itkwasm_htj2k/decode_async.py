# Generated file. Do not edit.

import os
from typing import Dict, Tuple, Optional, List, Any

from itkwasm import (
    environment_dispatch,
    BinaryStream,
    Image,
)

async def decode_async(
    codestream: bytes,
    decomposition_level: int = 0,
    information_only: bool = False,
) -> Image:
    """Decode a High Throughput JPEG2000 codestream and generate an ITK Image

    :param codestream: Input HTJ2K codestream
    :type  codestream: bytes

    :param decomposition_level: Decomposition level to decode. 0 is the highest resolution.
    :type  decomposition_level: int

    :param information_only: Only read metadata for the image but do not decode the pixel data.
    :type  information_only: bool

    :return: Output image
    :rtype:  Image
    """
    func = environment_dispatch("itkwasm_htj2k", "decode_async")
    output = await func(codestream, decomposition_level=decomposition_level, information_only=information_only)
    return output
