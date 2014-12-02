from distutils.core import setup, Extension
import numpy

envmodule = Extension('environment',
        sources = ['environmentmodule.c', 'grid.c', 'square.c', 'prob_dist.c', 'common.c'],
        libraries = ['gsl', 'gslcblas'])


setup (name = 'necsi-evolution-diversity',
        version = 1.0,
        description = 'A simulation thingy',
        include_dirs = [numpy.get_include()],
        ext_modules = [envmodule])
