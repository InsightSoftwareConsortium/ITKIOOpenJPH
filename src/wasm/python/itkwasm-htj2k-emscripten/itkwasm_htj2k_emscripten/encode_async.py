# Generated file. To retain edits, remove this comment.

# Generated file. Do not edit.

from pathlib import Path
import os
from typing import Dict, Tuple, Optional, List, Any

from .js_package import js_package

from itkwasm.pyodide import (
    to_js,
    to_py,
    js_resources
)
from itkwasm import (
    InterfaceTypes,
    Image,
    BinaryStream,
)

async def encode_async(
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
    js_module = await js_package.js_module
    web_worker = js_resources.web_worker

    kwargs = {}
    if decompositions:
        kwargs["decompositions"] = to_js(decompositions)
    if not_reversible:
        kwargs["notReversible"] = to_js(not_reversible)
    if quantization_step:
        kwargs["quantizationStep"] = to_js(quantization_step)
    if progression_order:
        kwargs["progressionOrder"] = to_js(progression_order)
    if tile_size:
        kwargs["tileSize"] = to_js(tile_size)
    if tile_offset:
        kwargs["tileOffset"] = to_js(tile_offset)
    if block_dimensions:
        kwargs["blockDimensions"] = to_js(block_dimensions)

    outputs = await js_module.encode(web_worker, to_js(image), **kwargs)

    output_web_worker = None
    output_list = []
    outputs_object_map = outputs.as_object_map()
    for output_name in outputs.object_keys():
        if output_name == 'webWorker':
            output_web_worker = outputs_object_map[output_name]
        else:
            output_list.append(to_py(outputs_object_map[output_name]))

    js_resources.web_worker = output_web_worker

    if len(output_list) == 1:
        return output_list[0]
    return tuple(output_list)
