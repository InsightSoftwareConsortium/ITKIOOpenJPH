# @itk-wasm/htj2k

[![npm version](https://badge.fury.io/js/@itk-wasm%2Fhtj2k.svg)](https://www.npmjs.com/package/@itk-wasm/htj2k)

> Wasm-SIMD accelerated decoding and encoding of High-throughput JPEG2000 (HTJ2K) images.

[👨‍💻 **Live API Demo** ✨](https://js.app.htj2k.wasm.itk.eth.limo/ ':include :type=iframe width=100% height=800px')

[🕮 **Documentation** 📚](https://js.docs.htj2k.wasm.itk.eth.limo/)

## Installation

```sh
npm install @itk-wasm/htj2k
```

## Usage

### Browser interface

Import:

```js
import {
  decode,
  encode,
  setPipelinesBaseUrl,
  getPipelinesBaseUrl,
  setPipelineWorkerUrl,
  getPipelineWorkerUrl,
} from "@itk-wasm/htj2k"
```

#### decode

*Decode a High Throughput JPEG2000 codestream and generate an ITK Image*

```ts
async function decode(
  webWorker: null | Worker,
  codestream: Uint8Array,
  options: DecodeOptions = {}
) : Promise<DecodeResult>
```

|   Parameter  |     Type     | Description            |
| :----------: | :----------: | :--------------------- |
| `codestream` | *Uint8Array* | Input HTJ2K codestream |

**`DecodeOptions` interface:**

|       Property       |    Type   | Description                                                        |
| :------------------: | :-------: | :----------------------------------------------------------------- |
| `decompositionLevel` |  *number* | Decomposition level to decode. 0 is the highest resolution.        |
|   `informationOnly`  | *boolean* | Only read metadata for the image but do not decode the pixel data. |

**`DecodeResult` interface:**

|    Property   |   Type   | Description                    |
| :-----------: | :------: | :----------------------------- |
| **webWorker** | *Worker* | WebWorker used for computation |
|    `image`    |  *Image* | Output image                   |

#### encode

*Encode an ITK Image into a High Throughput JPEG2000 codestream*

```ts
async function encode(
  webWorker: null | Worker,
  image: Image,
  options: EncodeOptions = {}
) : Promise<EncodeResult>
```

| Parameter |   Type  | Description |
| :-------: | :-----: | :---------- |
|  `image`  | *Image* | Input image |

**`EncodeOptions` interface:**

|      Property      |    Type    | Description                                                                             |
| :----------------: | :--------: | :-------------------------------------------------------------------------------------- |
|  `decompositions`  |  *number*  | Number of wavelet decompositions                                                        |
|   `notReversible`  |  *boolean* | Lossy and not reversible, i.e. not lossless, encoding                                   |
| `quantizationStep` |  *number*  | Quantization step size for lossy compression. Ignored unless not reversible is enabled. |
| `progressionOrder` |  *number*  | Progression order. 0 = LRCP, 1 = RLCP, 2 = RPCL, 3 = PCRL, 4 = CPRL                     |
|     `tileSize`     | *number[]* | Tile size                                                                               |
|    `tileOffset`    | *number[]* | Tile offset                                                                             |
|  `blockDimensions` | *number[]* | Block dimensions                                                                        |

**`EncodeResult` interface:**

|    Property   |     Type     | Description                    |
| :-----------: | :----------: | :----------------------------- |
| **webWorker** |   *Worker*   | WebWorker used for computation |
|    `output`   | *Uint8Array* | Output binary HTJ2K codestream |

#### setPipelinesBaseUrl

*Set base URL for WebAssembly assets when vendored.*

```ts
function setPipelinesBaseUrl(
  baseUrl: string | URL
) : void
```

#### getPipelinesBaseUrl

*Get base URL for WebAssembly assets when vendored.*

```ts
function getPipelinesBaseUrl() : string | URL
```

#### setPipelineWorkerUrl

*Set base URL for the itk-wasm pipeline worker script when vendored.*

```ts
function setPipelineWorkerUrl(
  baseUrl: string | URL
) : void
```

#### getPipelineWorkerUrl

*Get base URL for the itk-wasm pipeline worker script when vendored.*

```ts
function getPipelineWorkerUrl() : string | URL
```

### Node interface

Import:

```js
import {
  decodeNode,
  encodeNode,
  setPipelinesBaseUrl,
  getPipelinesBaseUrl,
  setPipelineWorkerUrl,
  getPipelineWorkerUrl,
} from "@itk-wasm/htj2k"
```

#### decodeNode

*Decode a High Throughput JPEG2000 codestream and generate an ITK Image*

```ts
async function decodeNode(
  codestream: Uint8Array,
  options: DecodeOptions = {}
) : Promise<DecodeNodeResult>
```

|   Parameter  |     Type     | Description            |
| :----------: | :----------: | :--------------------- |
| `codestream` | *Uint8Array* | Input HTJ2K codestream |

**`DecodeNodeOptions` interface:**

|       Property       |    Type   | Description                                                        |
| :------------------: | :-------: | :----------------------------------------------------------------- |
| `decompositionLevel` |  *number* | Decomposition level to decode. 0 is the highest resolution.        |
|   `informationOnly`  | *boolean* | Only read metadata for the image but do not decode the pixel data. |

**`DecodeNodeResult` interface:**

| Property |   Type  | Description  |
| :------: | :-----: | :----------- |
|  `image` | *Image* | Output image |

#### encodeNode

*Encode an ITK Image into a High Throughput JPEG2000 codestream*

```ts
async function encodeNode(
  image: Image,
  options: EncodeOptions = {}
) : Promise<EncodeNodeResult>
```

| Parameter |   Type  | Description |
| :-------: | :-----: | :---------- |
|  `image`  | *Image* | Input image |

**`EncodeNodeOptions` interface:**

|      Property      |    Type    | Description                                                                             |
| :----------------: | :--------: | :-------------------------------------------------------------------------------------- |
|  `decompositions`  |  *number*  | Number of wavelet decompositions                                                        |
|   `notReversible`  |  *boolean* | Lossy and not reversible, i.e. not lossless, encoding                                   |
| `quantizationStep` |  *number*  | Quantization step size for lossy compression. Ignored unless not reversible is enabled. |
| `progressionOrder` |  *number*  | Progression order. 0 = LRCP, 1 = RLCP, 2 = RPCL, 3 = PCRL, 4 = CPRL                     |
|     `tileSize`     | *number[]* | Tile size                                                                               |
|    `tileOffset`    | *number[]* | Tile offset                                                                             |
|  `blockDimensions` | *number[]* | Block dimensions                                                                        |

**`EncodeNodeResult` interface:**

| Property |     Type     | Description                    |
| :------: | :----------: | :----------------------------- |
| `output` | *Uint8Array* | Output binary HTJ2K codestream |
