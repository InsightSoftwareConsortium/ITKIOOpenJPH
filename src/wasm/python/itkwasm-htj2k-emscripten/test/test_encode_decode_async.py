import sys

if sys.version_info < (3,10):
    pytest.skip("Skipping pyodide tests on older Python", allow_module_level=True)

from pytest_pyodide import run_in_pyodide
from .fixtures import package_wheel, input_data

@run_in_pyodide(packages=['micropip', 'numpy'])
async def test_encode_decode_rgba_async(selenium, package_wheel, input_data):
    import micropip
    await micropip.install([package_wheel, 'numpy', 'itkwasm', 'itkwasm-compare-images'])

    from itkwasm_htj2k_emscripten import encode_async, decode_async
    from itkwasm_compare_images import compare_images_async
    import numpy as np
    from itkwasm import Image

    input_file = 'itk_logo.png'
    image = Image(**input_data[input_file])

    codestream = await encode_async(image, decompositions=5, tile_size=[256, 256], block_dimensions=[64, 64])
    decoded_image = await decode_async(codestream)

    metrics, _, _ = await compare_images_async(image, baseline_images=[decoded_image,])
    assert metrics['almostEqual'] == True


@run_in_pyodide(packages=['micropip', 'numpy'])
async def test_encode_decode_short_async(selenium, package_wheel, input_data):
    import micropip
    await micropip.install([package_wheel, 'numpy', 'itkwasm', 'itkwasm-compare-images'])

    from itkwasm_htj2k_emscripten import encode_async, decode_async
    from itkwasm_compare_images import compare_images_async
    import numpy as np
    from itkwasm import Image

    input_file = 'CT1.mha'
    image = Image(**input_data[input_file])

    codestream = await encode_async(image, decompositions=5, tile_size=[256, 256], block_dimensions=[64, 64])
    decoded_image = await decode_async(codestream)

    metrics, _, _ = await compare_images_async(image, baseline_images=[decoded_image,])
    assert metrics['almostEqual'] == True