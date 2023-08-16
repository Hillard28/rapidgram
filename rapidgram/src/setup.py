from setuptools import setup, Extension
from Cython.Build import cythonize

name="gram"
sources=["gram.pyx"]
extensions = Extension(name, sources, language="c++", extra_compile_args=["-O3", "-march=native"])

setup(ext_modules = cythonize(
     extensions,
     language_level=3,
))
