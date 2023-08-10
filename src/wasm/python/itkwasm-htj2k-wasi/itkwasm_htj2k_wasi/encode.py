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
    global _pipeline
    if _pipeline is None:
        _pipeline = Pipeline(file_resources('itkwasm_htj2k_wasi').joinpath(Path('wasm_modules') / Path('encode.wasi.wasm')))

    pipeline_outputs: List[PipelineOutput] = [
        PipelineOutput(InterfaceTypes.BinaryStream),
    ]

    pipeline_inputs: List[PipelineInput] = [
        PipelineInput(InterfaceTypes.Image, image),
    ]

    args: List[str] = ['--memory-io',]
    # Inputs
    args.append('0')
    # Outputs
    args.append('0')
    # Options
    if decompositions:
        args.append('--decompositions')
        args.append(str(decompositions))

    if not_reversible:
        args.append('--not-reversible')

    if quantization_step:
        args.append('--quantization-step')
        args.append(str(quantization_step))

    if progression_order:
        args.append('--progression-order')
        args.append(str(progression_order))

    if len(tile_size) > 1:
        if len(tile_size) < 2:
            raise ValueError('"tile-size" option must have a length > 2')

        args.append('--tile-size')
        for value in tile_size:
            args.append(str(value))

    if len(tile_offset) > 1:
        if len(tile_offset) < 2:
            raise ValueError('"tile-offset" option must have a length > 2')

        args.append('--tile-offset')
        for value in tile_offset:
            args.append(str(value))

    if len(block_dimensions) > 1:
        if len(block_dimensions) < 2:
            raise ValueError('"block-dimensions" option must have a length > 2')

        args.append('--block-dimensions')
        for value in block_dimensions:
            args.append(str(value))


    outputs = _pipeline.run(args, pipeline_outputs, pipeline_inputs)

    result = outputs[0].data.data
    return result

