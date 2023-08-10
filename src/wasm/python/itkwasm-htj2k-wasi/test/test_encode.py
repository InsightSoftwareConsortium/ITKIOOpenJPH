from itkwasm_htj2k_wasi import encode
from pathlib import Path
from PIL import Image

def test_encode():
    input_file = 'itk_logo.png'
    input_file_path = Path('..', '..', 'test', 'data', 'input', input_file)

    image = Image.open(input_file_path).tobytes()
    print(image)
