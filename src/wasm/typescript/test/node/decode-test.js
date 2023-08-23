import test from 'ava'
import path from 'path'
import fs from 'fs'

function mkdirP(dir) {
  try {
    fs.mkdirSync(dir, { recursive: true })
  } catch (err) {
    if (err.code !== 'EEXIST') throw err
  }
}


import { encodeNode, decodeNode } from '../../dist/bundles/htj2k-node.js'

const testInputPath = path.resolve('..', 'test', 'data', 'input')
const testOutputPath = path.resolve('..', 'test', 'output', 'typescript')
mkdirP(testOutputPath)

test('decode', async t => {
    const inputPath = path.join(testInputPath, 'CT1.j2c')
    const codestream = fs.readFileSync(inputPath)
    const { image } = await decodeNode(codestream)

    t.is(image.size[0], 512)
    t.is(image.size[1], 512)
})