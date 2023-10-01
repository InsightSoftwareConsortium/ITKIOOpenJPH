// Generated file. To retain edits, remove this comment.

import { writeImageArrayBuffer } from 'itk-wasm'
import { copyImage } from 'itk-wasm'
import * as htj2k from '../../../dist/bundles/htj2k.js'
import decodeLoadSampleInputs, { usePreRun } from "./decode-load-sample-inputs.js"

class DecodeModel {

  inputs: Map<string, any>
  options: Map<string, any>
  outputs: Map<string, any>

  constructor() {
    this.inputs = new Map()
    this.options = new Map()
    this.outputs = new Map()
    }
  }


class DecodeController  {

  constructor(loadSampleInputs) {
    this.loadSampleInputs = loadSampleInputs

    this.model = new DecodeModel()
    const model = this.model

    this.webWorker = null

    if (loadSampleInputs) {
      const loadSampleInputsButton = document.querySelector("#decodeInputs [name=loadSampleInputs]")
      loadSampleInputsButton.setAttribute('style', 'display: block-inline;')
      loadSampleInputsButton.addEventListener('click', async (event) => {
        loadSampleInputsButton.loading = true
        await loadSampleInputs(model)
        loadSampleInputsButton.loading = false
      })
    }

    // ----------------------------------------------
    // Inputs
    const codestreamElement = document.querySelector('#decodeInputs input[name=codestream-file]')
    codestreamElement.addEventListener('change', async (event) => {
        const dataTransfer = event.dataTransfer
        const files = event.target.files || dataTransfer.files

        const arrayBuffer = await files[0].arrayBuffer()
        model.inputs.set("codestream", new Uint8Array(arrayBuffer))
        const details = document.getElementById("decode-codestream-details")
        details.innerHTML = `<pre>${globalThis.escapeHtml(model.inputs.get("codestream").subarray(0, 50).toString() + ' ...')}</pre>`
        details.disabled = false
    })

    // ----------------------------------------------
    // Options
    const decompositionLevelElement = document.querySelector('#decodeInputs sl-input[name=decomposition-level]')
    decompositionLevelElement.addEventListener('sl-change', (event) => {
        model.options.set("decompositionLevel", parseInt(decompositionLevelElement.value))
    })

    const informationOnlyElement = document.querySelector('#decodeInputs sl-checkbox[name=information-only]')
    informationOnlyElement.addEventListener('sl-change', (event) => {
        model.options.set("informationOnly", informationOnlyElement.checked)
    })

    // ----------------------------------------------
    // Outputs
    const imageOutputDownload = document.querySelector('#decodeOutputs sl-button[name=image-download]')
    imageOutputDownload.addEventListener('click', async (event) => {
        event.preventDefault()
        event.stopPropagation()
        if (model.outputs.has("image")) {
            const imageDownloadFormat = document.getElementById('image-output-format')
            const downloadFormat = imageDownloadFormat.value || 'nrrd'
            const fileName = `image.${downloadFormat}`
            const { webWorker, arrayBuffer } = await writeImageArrayBuffer(null, copyImage(model.outputs.get("image")), fileName)

            webWorker.terminate()
            globalThis.downloadFile(arrayBuffer, fileName)
        }
    })

    const preRun = async () => {
      if (!this.webWorker && loadSampleInputs && usePreRun) {
        await loadSampleInputs(model, true)
        await this.run()
      }
    }

    const onSelectTab = async (event) => {
      if (event.detail.name === 'decode-panel') {
        const params = new URLSearchParams(window.location.search)
        if (!params.has('functionName') || params.get('functionName') !== 'decode') {
          params.set('functionName', 'decode')
          const url = new URL(document.location)
          url.search = params
          window.history.replaceState({ functionName: 'decode' }, '', url)
          await preRun()
        }
      }
    }

    const tabGroup = document.querySelector('sl-tab-group')
    tabGroup.addEventListener('sl-tab-show', onSelectTab)
    function onInit() {
      const params = new URLSearchParams(window.location.search)
      if (params.has('functionName') && params.get('functionName') === 'decode') {
        tabGroup.show('decode-panel')
        preRun()
      }
    }
    onInit()

    const runButton = document.querySelector('#decodeInputs sl-button[name="run"]')
    runButton.addEventListener('click', async (event) => {
      event.preventDefault()

      if(!model.inputs.has('codestream')) {
        globalThis.notify("Required input not provided", "codestream", "danger", "exclamation-octagon")
        return
      }


      try {
        runButton.loading = true

        const t0 = performance.now()
        const { image, } = await this.run()
        const t1 = performance.now()
        globalThis.notify("decode successfully completed", `in ${t1 - t0} milliseconds.`, "success", "rocket-fill")

        model.outputs.set("image", image)
        imageOutputDownload.variant = "success"
        imageOutputDownload.disabled = false
        const imageDetails = document.getElementById("decode-image-details")
        imageDetails.innerHTML = `<pre>${globalThis.escapeHtml(JSON.stringify(image, globalThis.interfaceTypeJsonReplacer, 2))}</pre>`
        imageDetails.disabled = false
        const imageOutput = document.getElementById('decode-image-details')
      } catch (error) {
        globalThis.notify("Error while running pipeline", error.toString(), "danger", "exclamation-octagon")
        throw error
      } finally {
        runButton.loading = false
      }
    })
  }

  async run() {
    const { webWorker, image, } = await htj2k.decode(this.webWorker,
      this.model.inputs.get('codestream').slice(),
      Object.fromEntries(this.model.options.entries())
    )
    this.webWorker = webWorker

    return { image, }
  }
}

const decodeController = new DecodeController(decodeLoadSampleInputs)
