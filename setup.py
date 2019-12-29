from distutils.core import setup, Extension

bismuth_csprng_module = Extension('bismuth_csprng',
                                 sources = ['csprng.c', 'csprng-python.c'],
                                 include_dirs = ['.'],
                                 libraries = ['ssl', 'crypto'])

setup (name = 'bismuth_csprng',
       version = '1.0',
       description = 'Binding for Bismuth csprng seedfile generator in c.',
       ext_modules = [bismuth_csprng_module])
