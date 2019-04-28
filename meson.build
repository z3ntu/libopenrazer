project('libopenrazer',
        'cpp',
        version : '0.0.1',)

qt5 = import('qt5')

qt5_dep = dependency('qt5', modules : ['Core', 'DBus', 'Gui'])
razer_test_dep = dependency('razer_test', fallback : ['razer_test', 'razer_test_dep'])

sources = [
    'src/device.cpp',
    'src/led.cpp',
    'src/libopenrazer.cpp',
    'src/manager.cpp',
    'src/razercapability.cpp'
]
sources += qt5.preprocess(
    moc_headers : ['include/libopenrazer.h']
)

inc = include_directories('include')

openrazerlib = library('openrazer',
                       sources,
                       version : meson.project_version(),
                       dependencies : [qt5_dep, razer_test_dep],
                       include_directories : inc,
                       install : not meson.is_subproject())

libopenrazer_dep = declare_dependency(
    dependencies : [qt5_dep, razer_test_dep],
    include_directories : inc,
    link_with : openrazerlib
)

# Stop execution here if we are in a subproject
if meson.is_subproject()
  subdir_done()
endif

install_headers('include/libopenrazer.h')

pkg = import('pkgconfig')
pkg.generate(name : meson.project_name(),
             description : 'Qt wrapper around the D-Bus API from OpenRazer',
             libraries : openrazerlib,
             version : meson.project_version())


# Demo executable
if get_option('buildtype') == 'debug'
  message('Building libopenrazerdemo...')
  executable('libopenrazerdemo',
             'src/demo/libopenrazerdemo.cpp',
             dependencies : [qt5_dep, razer_test_dep, libopenrazer_dep])
endif
