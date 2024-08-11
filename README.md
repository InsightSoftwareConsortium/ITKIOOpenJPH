# ITKIOOpenJPH

[![Build Status](https://github.com/InsightSoftwareConsortium/ITKIOOpenJPH/actions/workflows/build-test-package.yml/badge.svg)](https://github.com/InsightSoftwareConsortium/ITKIOOpenJPH/actions/workflows/build-test-package.yml)

[![PyPI Version](https://img.shields.io/pypi/v/itk-ioopenjph.svg)](https://pypi.python.org/pypi/itk-ioopenjph)

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://github.com/InsightSoftwareConsortium/ITKIOOpenJPH/blob/main/LICENSE)

## Overview

An ITK module to read and write High-throughput JPEG2000 (HTJ2K) images.

HTJ2K is a a multiscale image format supporting lossless and lossy
compression. This module is based on the
[OpenJPH](https://github.com/aous72/OpenJPH) accelerated codec
implementation.

👨‍💻 [Live API Demo](https://itk-wasm-htj2k-app-js.on.fleek.co/) ✨

## Installation

WebAssembly Python packages:

    pip install itkwasm-htj2k

Native Python packages, which allow reading / write HTJ2K codestreams
with `itk.imread` and `itk.imwrite`:

    pip install itk-ioopenjph

JavaScript package:

    npm install @itk-wasm/htj2k

## Usage

-   🕮 [Wasm Python
    Documentation](https://py.docs.htj2k.wasm.itk.eth.limo/) 📚
-   🕮 [JavaScript
    Documentation](https://js.docs.htj2k.wasm.itk.eth.limo/) 📚
-   🕮 [ITK Python IO
    Documentation](https://itkpythonpackage.readthedocs.io/en/master/Quick_start_guide.html#input-output-io)
    📚

## Related Information

-   [htj2k.com](https://htj2k.com)
-   [HTJ2K
    Primer](http://ds.jpeg.org/whitepapers/jpeg-htj2k-whitepaper.pdf)
-   [HTJ2K Resources](https://github.com/chafey/HTJ2KResources)

## Related Projects

-   [OpenJPH](https://github.com/aous72/OpenJPH)
-   [openhtjs](https://github.com/sandflow/openhtjs)
-   [openjphjs](https://github.com/chafey/openjphjs)
-   [OpenHTJ2K](https://github.com/osamu620/OpenHTJ2K)
-   [htj2k-js](https://github.com/PantelisGeorgiadis/htj2k-js)
