cmake_minimum_required(VERSION 3.10)

set(ProjectNameParam "DefaultProject" CACHE STRING "Command line argument containing the project name.")
project(${ProjectNameParam} VERSION 1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_executable(dikr WIN32
    src/dikr.c
)

include_directories(submodules/Nuklear)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set_target_properties(dikr PROPERTIES VS_GLOBAL_VcpkgEnabled true)
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(dikr
            PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
    find_package(SDL2_ttf CONFIG REQUIRED)
    target_link_libraries(dikr PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts/cmake/")
    find_package(SDL2 CONFIG REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(dikr PUBLIC ${SDL2_LIBRARIES})
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts/cmake/")
    find_package(SDL2 CONFIG REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(dikr PUBLIC -lm -ldl ${SDL2_LIBRARIES})
endif()

add_custom_command(TARGET dikr POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)

# settings
add_executable(settings WIN32
    src/settings.c
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set_target_properties(settings PROPERTIES VS_GLOBAL_VcpkgEnabled true)
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(settings
            PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
    find_package(SDL2_ttf CONFIG REQUIRED)
    target_link_libraries(settings PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts/cmake/")
    find_package(SDL2 CONFIG REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(settings PUBLIC ${SDL2_LIBRARIES})
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts/cmake/")
    find_package(SDL2 CONFIG REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(settings PUBLIC -lm -ldl ${SDL2_LIBRARIES})
endif()

add_custom_command(TARGET settings POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)

