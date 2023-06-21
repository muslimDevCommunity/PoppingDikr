# بسم الله الرحمن الرحيم
project('Dikr', 'cpp',
        default_options: 'default_library=static')


sdl_dep = [
    dependency('sdl2'),
    dependency('sdl2_ttf')
]

executable('PoppingDikr',
            'Dikr.cpp',
            dependencies: sdl_dep)

sdl_imgui_files = [
        'thirdparty/imgui/imgui.cpp',
        'thirdparty/imgui/imgui_demo.cpp',
        'thirdparty/imgui/imgui_widgets.cpp',
        'thirdparty/imgui/imgui_draw.cpp',
        'thirdparty/imgui/imgui_tables.cpp',
        'thirdparty/imgui/imgui_impl_sdl2.cpp',
        'thirdparty/imgui/imgui_impl_sdlrenderer.cpp']

executable('PoppingDikr-Settings',
        ['Settings.cpp', sdl_imgui_files],
        dependencies: sdl_dep)

Settings_files = [
    'settings_exe/main.cpp'
]

