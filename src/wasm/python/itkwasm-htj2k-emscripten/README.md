# itkwasm-htj2k-emscripten

[![PyPI version](https://badge.fury.io/py/itkwasm-htj2k-emscripten.svg)](https://badge.fury.io/py/itkwasm-htj2k-emscripten)

Wasm-SIMD accelerated decoding and encoding High-throughput JPEG2000 (HTJ2K) images. Emscripten implementation.

This package provides the Emscripten WebAssembly implementation. It is usually not called directly. Please use the [`itkwasm-htj2k`](https://pypi.org/project/itkwasm-htj2k/) instead.


## Installation

```sh
import micropip
await micropip.install('itkwasm-htj2k-emscripten')
```

## Development

```sh
pip install hatch
hatch run download-pyodide
hatch run test
```
