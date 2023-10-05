ITKIOOpenJPH
=================================

.. image:: https://github.com/InsightSoftwareConsortium/ITKIOOpenJPH/actions/workflows/build-test-package.yml/badge.svg
    :target: https://github.com/InsightSoftwareConsortium/ITKIOOpenJPH/actions/workflows/build-test-package.yml
    :alt: Build Status

.. image:: https://img.shields.io/pypi/v/itk-ioopenjph.svg
    :target: https://pypi.python.org/pypi/itk-ioopenjph
    :alt: PyPI Version

.. image:: https://img.shields.io/badge/License-Apache%202.0-blue.svg
    :target: https://github.com/InsightSoftwareConsortium/ITKIOOpenJPH/blob/main/LICENSE
    :alt: License

Overview
--------

An ITK module to read and write High-throughput JPEG2000 (HTJ2K) images.

HTJ2K is a a multiscale image format supporting lossless and lossy compression. This module is based on the `OpenJPH`_ accelerated codec implementation.

👨‍💻 `Live API Demo`_ ✨


Installation
------------

WebAssembly Python packages::

  pip install itkwasm-htj2k

Native Python packages, which allow reading / write HTJ2K codestreams with
``itk.imread`` and ``itk.imwrite``::

  pip install itk-ioopenjph

JavaScript package::

  npm install @itk-wasm/htj2k

Usage
------

- 🕮 `Wasm Python Documentation`_ 📚
- 🕮 `JavaScript Documentation`_ 📚
- 🕮 `ITK Python IO Documentation`_ 📚


Related Information
--------------------

- `htj2k.com`_
- `HTJ2K Primer`_
- `HTJ2K Resources`_

Related Projects
----------------

- `OpenJPH`_
- `openhtjs`_
- `openjphjs`_
- `OpenHTJ2K`_
- `htj2k-js`_

.. _htj2k.com: https://htj2k.com
.. _HTJ2K Primer: http://ds.jpeg.org/whitepapers/jpeg-htj2k-whitepaper.pdf
.. _HTJ2K Resources: https://github.com/chafey/HTJ2KResources

.. _OpenJPH: https://github.com/aous72/OpenJPH
.. _Live API Demo: https://itk-wasm-htj2k-app-js.on.fleek.co/
.. _Wasm Python Documentation: https://py.docs.htj2k.wasm.itk.eth.limo/
.. _JavaScript Documentation: https://js.docs.htj2k.wasm.itk.eth.limo/
.. _ITK Python IO Documentation: https://itkpythonpackage.readthedocs.io/en/master/Quick_start_guide.html#input-output-io
.. _openhtjs: https://github.com/sandflow/openhtjs
.. _openjphjs: https://github.com/chafey/openjphjs
.. _OpenHTJ2K: https://github.com/osamu620/OpenHTJ2K
.. _htj2k-js: https://github.com/PantelisGeorgiadis/htj2k-js
