import { readImageArrayBuffer } from "itk-wasm"

export default async function encodeLoadSampleInputs (model, preRun=false) {
  const imageButton = document.querySelector('#encodeInputs sl-button[name=image-file-button]')
  if (!preRun) {
    imageButton.loading = true
  }
  const fileName = 'CT1.mha'
  const imageReponse = await fetch(`https://bafybeidgylhkxaiqdjd6tnlmayvzliqyxspymhjtvlooui37a6omsilwu4.ipfs.w3s.link/ipfs/bafybeidgylhkxaiqdjd6tnlmayvzliqyxspymhjtvlooui37a6omsilwu4/data/input/${fileName}`)
  const imageData = new Uint8Array(await imageReponse.arrayBuffer())
  const { webWorker, image } = await readImageArrayBuffer(null, imageData.buffer, fileName)
  model.inputs.set('image', image)
  if (!preRun) {
    const imageElement = document.querySelector('#encode-image-details')
    imageElement.innerHTML = `<pre>${globalThis.escapeHtml(JSON.stringify(image, globalThis.interfaceTypeJsonReplacer, 2))}</pre>`
    imageElement.disabled = false
    imageButton.loading = false
  }

  return model
}

export const usePreRun = true
