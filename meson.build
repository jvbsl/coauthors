project(
    'coauthor',
    'cpp',
    version: '1.0.0',
    default_options : ['warning_level=3', 'cpp_std=c++17', 'buildtype=minsize']
)


project_source_files = [
    'src/main.cpp'
]
build_args = [
    
]

cc = meson.get_compiler('cpp')



libui_incdir = include_directories('libui')
libui_lib = cc.find_library('ui', dirs : [meson.current_source_dir() + '/libui/build/meson-out/'])


dependencies = [libui_lib]


build_args += [
  '-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
]

if host_machine.system() == 'linux'
    gtkdep = dependency('gtk+-3.0')
    dl_dep = cc.find_library('dl', required : false)
    dependencies += [gtkdep, dl_dep]
    build_args += ['-DLINUX=1']
endif

project_target = executable(
  meson.project_name(),
  project_source_files,
  install : true,
  cpp_args : build_args,
  dependencies: dependencies,
  include_directories: libui_incdir
)
