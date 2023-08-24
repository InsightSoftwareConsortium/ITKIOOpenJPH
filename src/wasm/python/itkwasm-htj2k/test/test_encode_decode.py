from pathlib import Path
import itk
from itkwasm import Image
from itkwasm_compare_images import compare_images
import numpy as np

def encode_decode_roundtrip(file_path):
    from itkwasm_htj2k import encode, decode

    image = itk.imread(file_path)
    image_dict = itk.dict_from_image(image)
    image = Image(**image_dict)

    codestream = encode(image, decompositions=5, tile_size=[256, 256], block_dimensions=[64, 64])
    decoded_image = decode(codestream)

    metrics, _, _ = compare_images(image, baseline_images=[decoded_image,])
    assert metrics['almostEqual'] == True

def test_encode_decode_rgba():
    input_file = 'itk_logo.png'
    input_file_path = Path('..', '..', 'test', 'data', 'input', input_file)

    encode_decode_roundtrip(input_file_path)

def test_encode_decode_short():
    input_file = 'CT1.mha'
    input_file_path = Path('..', '..', 'test', 'data', 'input', input_file)

    encode_decode_roundtrip(input_file_path)
