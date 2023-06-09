# SPDX-FileCopyrightText: 2020 Josh Morman 
#

project('gnuradio', 'cpp', 
  version : '4.0.0.0-preview0',
  license : 'GPLv3',
  default_options : ['cpp_std=c++20'])

## Compilers
cc = meson.get_compiler('cpp')
rt_dep = cc.find_library('rt', required : false)
c_available = add_languages('c', required : true)
cuda_available = add_languages('cuda', required : false)

IMPLEMENT_CUDA = false # By default
IMPLEMENT_CPU = true

SCRIPTS_DIR=join_paths(meson.project_source_root(),'utils','blockbuilder','scripts')

## Build Dependencies
volk_dep = dependency('volk', version : '>=2.2')
yaml_dep = dependency('yaml-cpp', version : '>=0.6')
fmt_dep = dependency('fmt')
spdlog_dep = dependency('spdlog')
threads_dep = dependency('threads')

# Import python3 meson module which is used to find the Python dependencies.
# Python3 is required for code generation
py3 = import('python').find_installation('python3')
py3_version = py3.language_version()
if py3_version.version_compare('< 3.6')
  error('Invalid python version!?')
endif
py3_purelib = (get_option('libdir') == 'lib')
py3_install_dir = py3.get_install_dir( pure : py3_purelib)
GR_PURELIB = py3_purelib
if (get_option('enable_testing'))
gtest_dep = dependency('gtest', main : true, version : '>=1.10', required : get_option('enable_testing'))
endif

GR_ENABLE_PYTHON = false
if (get_option('enable_python'))
python3_dep = dependency('python3', required : get_option('enable_python'))
python3_embed_dep = dependency('python3-embed', required : get_option('enable_python'))
pybind11_dep = dependency('pybind11', required : get_option('enable_python'))
incdir_numpy = run_command(py3,
  ['-c', 'import os; os.chdir(".."); import numpy; print(numpy.get_include())'],
  check : true
).stdout().strip()

# Python module min versions
# TODO: distinguish between build and runtime requirements
py_check_script = join_paths(meson.project_source_root(),'utils','py_module_check.py')
run_command('python3', py_check_script, 'numpy', '1.22', check:true)
run_command('python3', py_check_script, 'jinja2', '3.0.2', check:true)
run_command('python3', py_check_script, 'yaml', '5.3.1', check:true)
run_command('python3', py_check_script, 'jsonschema', '3.2.0', check:true)
run_command('python3', py_check_script, 'jsonschema_default', check:true)
GR_ENABLE_PYTHON = true
endif

if (get_option('enable_cuda'))
cuda_dep = dependency('cuda', version : '>=10.1', required : true, modules : ['cufft'])
if not cuda_available
error('CUDA compiler not found, but enable_cuda option specified')
endif
IMPLEMENT_CUDA = true
endif



## Subprojects

libpmtv = subproject('pmt')
pmt_dep = libpmtv.get_variable('pmt_dep')
#py3_install_dir_dep = libpmtv.get_variable('py3_install_dir_dep')
CLI11_dep = dependency('cli11')

fmt_dep = dependency('fmt', version:'8.1.1')

cmake = import('cmake')
libreflcpp = cmake.subproject('refl-cpp')
reflcpp_dep = libreflcpp.dependency('refl-cpp')

ut_dep = dependency('boost.ut')

# opt_var = cmake.subproject_options()
# opt_var.add_cmake_defines({'ENABLE_TESTING': false})
# libgp = cmake.subproject('graph-prototype', options: opt_var)
# gp_dep = libcppzmq.dependency('cppzmq')

opt_var = cmake.subproject_options()
opt_var.add_cmake_defines({'CPPZMQ_BUILD_TESTS': false})
# libcppzmq = cmake.subproject('cppzmq', options: opt_var)
# cppzmq_dep = libcppzmq.dependency('cppzmq')

if IMPLEMENT_CUDA
  libcusp = subproject('cusp')
  cusp_dep = libcusp.get_variable('cusp_dep')
endif

prefix = get_option('prefix')

summary({'bindir': get_option('bindir'),
        'libdir': get_option('libdir'),
        'datadir': get_option('datadir'),
        'prefix': get_option('prefix'),
        # 'Custom blocks path': get_option('custom_blocks_path'),
        'Python dir': py3_install_dir,
        }, section: 'Directories')


subdir('graph')
subdir('blocks-lgpl')