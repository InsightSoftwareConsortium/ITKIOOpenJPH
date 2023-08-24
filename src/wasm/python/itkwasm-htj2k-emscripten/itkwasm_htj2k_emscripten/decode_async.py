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
    js_module = await js_package.js_module
    web_worker = js_resources.web_worker

    kwargs = {}
    if decomposition_level:
        kwargs["decompositionLevel"] = to_js(decomposition_level)
    if information_only:
        kwargs["informationOnly"] = to_js(information_only)

    outputs = await js_module.decode(web_worker, to_js(codestream), **kwargs)

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
