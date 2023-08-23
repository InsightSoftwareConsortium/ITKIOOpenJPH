export default async function decodeLoadSampleInputs (model) {
  const codestreamButton = document.querySelector('#decodeInputs sl-button[name=codestream-file-button]')
  codestreamButton.loading = true
  const fileName = 'CT1.j2c'
  const codestreamReponse = await fetch(`https://bafybeidgylhkxaiqdjd6tnlmayvzliqyxspymhjtvlooui37a6omsilwu4.ipfs.w3s.link/ipfs/bafybeidgylhkxaiqdjd6tnlmayvzliqyxspymhjtvlooui37a6omsilwu4/data/input/${fileName}`)
  const codestreamData = new Uint8Array(await codestreamReponse.arrayBuffer())
  model.inputs.set('codestream', codestreamData)
  const codestreamElement = document.querySelector('#decode-codestream-details')
  codestreamElement.innerHTML = `<pre>${globalThis.escapeHtml(codestreamData.subarray(0, 50).toString())}</pre>`
  codestreamElement.disabled = false
  codestreamButton.loading = false

  return model
}
