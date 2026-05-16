find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

add_library(raytracer_viewport STATIC
    Framebuffer.cpp
    StatusOverlay.cpp
    Viewport.cpp
    ViewportRunner.cpp
)

target_include_directories(raytracer_viewport
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_viewport
    PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
        raytracer_components
        raytracer_math
        raytracer_renderer
        raytracer_shading
        raytracer_scene
        raytracer_helper
        raytracer_exceptions
)

target_compile_definitions(raytracer_viewport
    PRIVATE
        VIEWPORT_FONT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/Roboto_Condensed-Regular.ttf"
)

raytracer_enable_target_warnings(raytracer_viewport)
