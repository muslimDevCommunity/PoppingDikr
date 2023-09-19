# بسم الله الرحمن الرحيم
project('Dikr', 'cpp')


sdl_dep = [
    dependency('sdl2'),
    dependency('sdl2_ttf')
]

executable('popping-dikr',
            'Dikr.cpp',
            win_subsystem: 'windows',
            dependencies: sdl_dep)

sdl_imgui_files = [
        'thirdparty/imgui/imgui.cpp',
        'thirdparty/imgui/imgui_demo.cpp',
        'thirdparty/imgui/imgui_widgets.cpp',
        'thirdparty/imgui/imgui_draw.cpp',
        'thirdparty/imgui/imgui_tables.cpp',
        'thirdparty/imgui/imgui_impl_sdl2.cpp',
        'thirdparty/imgui/imgui_impl_sdlrenderer.cpp']

executable('popping-dikr-settings',
        ['Settings.cpp', sdl_imgui_files],
        dependencies: sdl_dep)


