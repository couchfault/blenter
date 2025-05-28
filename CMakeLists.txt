cmake_minimum_required(VERSION 3.27)
project(lighting)

set(CMAKE_CXX_STANDARD 23)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(lighting main.cpp
        lib/graphics/shader.cpp
        lib/graphics/shader.hpp
        shapes.cpp
        shapes.hpp
        texture.cpp
        texture.hpp
        lib/graphics/material.cpp
        lib/graphics/material.hpp
        lib/graphics/light.cpp
        lib/graphics/light.hpp
        lib/graphics/camera.hpp
        lib/graphics/camera.cpp
        lib/graphics/cursor.cpp
        lib/graphics/cursor.hpp
        lib/graphics/cursor_type.hpp
        block.hpp
        lib/graphics/view.cpp
        lib/graphics/view.hpp
        lib/graphics/window.hpp
        lib/graphics/window.cpp
        lib/graphics/scene.cpp
        lib/graphics/scene.hpp
        object_editor.cpp
        object_editor.hpp
        engine.cpp
        engine.hpp
        ui_scene.cpp
        ui_scene.hpp
        lib/graphics/entity.cpp
        lib/graphics/entity.hpp
        lib/graphics/event.cpp
        lib/graphics/event.hpp
        level_editor.cpp
        level_editor.hpp
        lib/graphics/style.cpp
        lib/graphics/style.hpp
        lib/graphics/mouse.cpp
        lib/graphics/mouse.hpp
        lib/graphics/color.hpp
        lib/graphics/keyboard.cpp
        lib/graphics/keyboard.hpp
        lib/graphics/animate.cpp
        lib/graphics/animate.hpp
        lib/serialize/glm_vec.hpp
        lib/serialize/tranform.hpp
        lib/serialize/glm_mat.hpp
        lib/serialize/style.hpp
        lib/serialize/cursor_type.hpp
        lib/serialize/render_mode.hpp
        lib/serialize/entity.hpp
)

target_link_libraries(lighting PRIVATE glad::glad)
target_link_libraries(lighting PRIVATE glfw)
target_link_libraries(lighting PRIVATE glm::glm)

target_include_directories(lighting PRIVATE ${Stb_INCUDE_DIR})

add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/main_shader.frag
        ${PROJECT_BINARY_DIR}/main_shader.frag)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/light_shader.frag
        ${PROJECT_BINARY_DIR}/light_shader.frag)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/main_shader.vert
        ${PROJECT_BINARY_DIR}/main_shader.vert)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/shaders/instanced_shader.vert
        ${PROJECT_BINARY_DIR}/instanced_shader.vert)

add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources)
add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/example
        ${PROJECT_BINARY_DIR}/example)
