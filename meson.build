project('libopenrazer',
        'cpp',
        version : '0.0.1',
        default_options : ['cpp_std=c++11'])

qt5 = import('qt5')

qt5_dep = dependency('qt5', modules : ['Core', 'DBus', 'Gui', 'Xml'])
razer_test_dep = dependency('razer_test', fallback : ['razer_test', 'razer_test_dep'])

sources = [
    'src/dbusexception.cpp',
    'src/misc.cpp',
    'src/razercapability.cpp',
]

target = get_option('daemon_target')
if target == 'openrazer'
    message('Building for openrazer...')
    sources += [
        'src/openrazer/device.cpp',
        'src/openrazer/led.cpp',
        'src/openrazer/manager.cpp',
    ]
elif target == 'razer_test'
    message('Building for razer_test...')
    sources += [
        'src/razer_test/device.cpp',
        'src/razer_test/led.cpp',
        'src/razer_test/manager.cpp',
    ]
else
    error('Unsupported daemon_target!')
endif

sources += qt5.preprocess(
    moc_headers : [
        'include/libopenrazer/device.h',
        'include/libopenrazer/led.h',
        'include/libopenrazer/manager.h',
    ]
)

inc = include_directories('include')
srcinc = include_directories('src')

openrazerlib = library('openrazer',
                       sources,
                       version : meson.project_version(),
                       dependencies : [qt5_dep, razer_test_dep],
                       include_directories : [inc, srcinc],
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
install_headers('include/libopenrazer/dbusexception.h',
                'include/libopenrazer/manager.h',
                subdir : 'libopenrazer')

pkg = import('pkgconfig')
pkg.generate(name : meson.project_name(),
             description : 'Qt wrapper around the D-Bus API from OpenRazer',
             libraries : openrazerlib,
             version : meson.project_version())


# Demo executable
if get_option('demo') == true
  message('Building libopenrazerdemo...')
  executable('libopenrazerdemo',
             'src/demo/libopenrazerdemo.cpp',
             dependencies : [qt5_dep, razer_test_dep, libopenrazer_dep])
endif
