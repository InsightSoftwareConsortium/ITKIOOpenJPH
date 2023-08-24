from itkwasm.pyodide import JsPackageConfig, JsPackage

from ._version import __version__

default_config = JsPackageConfig(f"https://cdn.jsdelivr.net/npm/@itk-wasm/htj2k@{__version__}/dist/bundles/htj2k.js")
js_package = JsPackage(default_config)
