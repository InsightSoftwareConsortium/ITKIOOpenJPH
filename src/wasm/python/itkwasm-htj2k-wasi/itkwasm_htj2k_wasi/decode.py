# Generated file. Do not edit.

from pathlib import Path, PurePosixPath
import os
from typing import Dict, Tuple, Optional, List

from importlib_resources import files as file_resources

_pipeline = None

from itkwasm import (
    InterfaceTypes,
    PipelineOutput,
    PipelineInput,
    Pipeline,
    BinaryStream,
    Image,
)

def decode(
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
    global _pipeline
    if _pipeline is None:
        _pipeline = Pipeline(file_resources('itkwasm_htj2k_wasi').joinpath(Path('wasm_modules') / Path('decode.wasi.wasm')))

    pipeline_outputs: List[PipelineOutput] = [
        PipelineOutput(InterfaceTypes.Image),
    ]

    pipeline_inputs: List[PipelineInput] = [
        PipelineInput(InterfaceTypes.BinaryStream, BinaryStream(codestream)),
    ]

    args: List[str] = ['--memory-io',]
    # Inputs
    args.append('0')
    # Outputs
    args.append('0')
    # Options
    if decomposition_level:
        args.append('--decomposition-level')
        args.append(str(decomposition_level))

    if information_only:
        args.append('--information-only')


    outputs = _pipeline.run(args, pipeline_outputs, pipeline_inputs)

    result = outputs[0].data
    return result

