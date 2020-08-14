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
    dl_dep = cc.find_library('dl')
    dependencies += [gtkdep, dl_dep]
    build_args += ['-DLINUX=1']
elif  host_machine.system() == 'windows'
    d2d1_lib = cc.find_library('d2d1')
    comctrl_lib = cc.find_library('Comctl32')
    dwrite_lib = cc.find_library('Dwrite')
    
    dependencies += [d2d1_lib, comctrl_lib, dwrite_lib, cc.find_library('user32'),
                        cc.find_library('kernel32'),
                        cc.find_library('gdi32'),
                        cc.find_library('uxtheme'),
                        cc.find_library('msimg32'),
                        cc.find_library('comdlg32'),
                        cc.find_library('ole32'),
                        cc.find_library('oleaut32'),
                        cc.find_library('oleacc'),
                        cc.find_library('uuid'),
                        cc.find_library('windowscodecs')]
endif

project_target = executable(
  meson.project_name(),
  project_source_files,
  install : true,
  cpp_args : build_args,
  dependencies: dependencies,
  include_directories: libui_incdir
)
