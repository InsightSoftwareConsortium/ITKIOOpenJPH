# itkwasm-htj2k-wasi

[![PyPI version](https://badge.fury.io/py/itkwasm-htj2k-wasi.svg)](https://badge.fury.io/py/itkwasm-htj2k-wasi)

Read and write High-throughput JPEG2000 (HTJ2K) images. WASI implementation.

This package provides the WASI WebAssembly implementation. It is usually not called directly. Please use the [`itkwasm-htj2k`](https://pypi.org/project/itkwasm-htj2k/) instead.


## Installation

```sh
pip install itkwasm-htj2k-wasi
```

## Development

```sh
pip install pytest itkwasm-image-io itkwasm-compare-images
pip install -e .
pytest

# or
pip install hatch
hatch run test
```
