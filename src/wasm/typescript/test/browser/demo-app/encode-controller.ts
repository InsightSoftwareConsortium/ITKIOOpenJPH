// Generated file. To retain edits, remove this comment.

import { readImageFile, copyImage } from 'itk-wasm'
import * as htj2k from '../../../dist/bundles/htj2k.js'
import encodeLoadSampleInputs from "./encode-load-sample-inputs.js"

class EncodeModel {

  inputs: Map<string, any>
  options: Map<string, any>
  outputs: Map<string, any>

  constructor() {
    this.inputs = new Map()
    this.options = new Map()
    this.outputs = new Map()
    }
  }


class EncodeController  {

  constructor(loadSampleInputs) {
    this.loadSampleInputs = loadSampleInputs

    this.model = new EncodeModel()
    const model = this.model

    this.webWorker = null

    if (loadSampleInputs) {
      const loadSampleInputsButton = document.querySelector("#encodeInputs [name=loadSampleInputs]")
      loadSampleInputsButton.setAttribute('style', 'display: block-inline;')
      loadSampleInputsButton.addEventListener('click', async (event) => {
        loadSampleInputsButton.loading = true
        await loadSampleInputs(model)
        loadSampleInputsButton.loading = false
      })
    }

    // ----------------------------------------------
    // Inputs
    const imageElement = document.querySelector('#encodeInputs input[name=image-file]')
    imageElement.addEventListener('change', async (event) => {
        const dataTransfer = event.dataTransfer
        const files = event.target.files || dataTransfer.files

        const { image, webWorker } = await readImageFile(null, files[0])
        webWorker.terminate()
        model.inputs.set("image", image)
        const details = document.getElementById("encode-image-details")
        details.innerHTML = `<pre>${globalThis.escapeHtml(JSON.stringify(image, globalThis.interfaceTypeJsonReplacer, 2))}</pre>`
        details.disabled = false
    })

    // ----------------------------------------------
    // Options
    const decompositionsElement = document.querySelector('#encodeInputs sl-input[name=decompositions]')
    decompositionsElement.addEventListener('sl-change', (event) => {
        model.options.set("decompositions", parseInt(decompositionsElement.value))
    })

    const notReversibleElement = document.querySelector('#encodeInputs sl-checkbox[name=not-reversible]')
    notReversibleElement.addEventListener('sl-change', (event) => {
        model.options.set("notReversible", notReversibleElement.checked)
    })

    const quantizationStepElement = document.querySelector('#encodeInputs sl-input[name=quantization-step]')
    quantizationStepElement.addEventListener('sl-change', (event) => {
        model.options.set("quantizationStep", parseFloat(quantizationStepElement.value))
    })

    const progressionOrderElement = document.querySelector('#encodeInputs sl-input[name=progression-order]')
    progressionOrderElement.addEventListener('sl-change', (event) => {
        model.options.set("progressionOrder", parseInt(progressionOrderElement.value))
    })

    const tileSizeElement = document.querySelector('#encodeInputs sl-input[name=tile-size]')
    tileSizeElement.addEventListener('sl-change', (event) => {
        globalThis.applyInputParsedJson(tileSizeElement, model.options, "tileSize")
    })

    const tileOffsetElement = document.querySelector('#encodeInputs sl-input[name=tile-offset]')
    tileOffsetElement.addEventListener('sl-change', (event) => {
        globalThis.applyInputParsedJson(tileOffsetElement, model.options, "tileOffset")
    })

    const blockDimensionsElement = document.querySelector('#encodeInputs sl-input[name=block-dimensions]')
    blockDimensionsElement.addEventListener('sl-change', (event) => {
        globalThis.applyInputParsedJson(blockDimensionsElement, model.options, "blockDimensions")
    })

    // ----------------------------------------------
    // Outputs
    const outputOutputDownload = document.querySelector('#encodeOutputs sl-button[name=output-download]')
    outputOutputDownload.addEventListener('click', (event) => {
        event.preventDefault()
        event.stopPropagation()
        if (model.outputs.has("output")) {
            globalThis.downloadFile(model.outputs.get("output"), "output.bin")
        }
    })

    const runButton = document.querySelector('#encodeInputs sl-button[name="run"]')
    runButton.addEventListener('click', async (event) => {
      event.preventDefault()

      if(!model.inputs.has('image')) {
        globalThis.notify("Required input not provided", "image", "danger", "exclamation-octagon")
        return
      }


      try {
        runButton.loading = true
        const t0 = performance.now()

        const { webWorker, output, } = await htj2k.encode(this.webWorker,
          copyImage(model.inputs.get('image')),
          Object.fromEntries(model.options.entries())
        )

        const t1 = performance.now()
        globalThis.notify("encode successfully completed", `in ${t1 - t0} milliseconds.`, "success", "rocket-fill")
        this.webWorker = webWorker

        model.outputs.set("output", output)
        outputOutputDownload.variant = "success"
        outputOutputDownload.disabled = false
        const outputOutput = document.getElementById("encode-output-details")
        outputOutput.innerHTML = `<pre>${globalThis.escapeHtml(output.subarray(0, 1024).toString() + ' ...')}</pre>`
        outputOutput.disabled = false
      } catch (error) {
        globalThis.notify("Error while running pipeline", error.toString(), "danger", "exclamation-octagon")
        throw error
      } finally {
        runButton.loading = false
      }
    })
  }
}

const encodeController = new EncodeController(encodeLoadSampleInputs)
