# ── Primitives ──────────────────────────────────────────────────────────────

add_library(Sphere SHARED SphereEntry.cpp)
target_link_libraries(Sphere PRIVATE raytracer_components raytracer_plugin)
set_target_properties(Sphere PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(Sphere)

add_library(Plane SHARED PlaneEntry.cpp)
target_link_libraries(Plane PRIVATE raytracer_components raytracer_plugin)
set_target_properties(Plane PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(Plane)

add_library(Cylinder SHARED CylinderEntry.cpp)
target_link_libraries(Cylinder PRIVATE raytracer_components raytracer_plugin)
set_target_properties(Cylinder PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(Cylinder)

add_library(Cone SHARED ConeEntry.cpp)
target_link_libraries(Cone PRIVATE raytracer_components raytracer_plugin)
set_target_properties(Cone PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(Cone)

# ── Lights ───────────────────────────────────────────────────────────────────

add_library(raytracer_ambient_light SHARED AmbientEntry.cpp)
target_link_libraries(raytracer_ambient_light PRIVATE raytracer_components raytracer_plugin)
set_target_properties(raytracer_ambient_light PROPERTIES PREFIX "" OUTPUT_NAME "ambient"
    SUFFIX ".so" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(raytracer_ambient_light)

# Directional depends on Scene (shadow rays) — links raytracer_scene.
add_library(raytracer_directional_light SHARED DirectionalEntry.cpp)
target_link_libraries(raytracer_directional_light PRIVATE raytracer_plugin raytracer_scene)
set_target_properties(raytracer_directional_light PROPERTIES PREFIX "" OUTPUT_NAME "directional"
    SUFFIX ".so" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(raytracer_directional_light)

# ── Camera ───────────────────────────────────────────────────────────────────

add_library(raytracer_perspective_camera SHARED PerspectiveCameraEntry.cpp)
target_link_libraries(raytracer_perspective_camera PRIVATE raytracer_components raytracer_plugin)
set_target_properties(raytracer_perspective_camera PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(raytracer_perspective_camera)

# ── Materials ────────────────────────────────────────────────────────────────

add_library(DiffuseMaterial SHARED DiffuseMaterialEntry.cpp)
target_link_libraries(DiffuseMaterial PRIVATE raytracer_components raytracer_plugin)
set_target_properties(DiffuseMaterial PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(DiffuseMaterial)

add_library(GlossyMaterial SHARED GlossyMaterialEntry.cpp)
target_link_libraries(GlossyMaterial PRIVATE raytracer_components raytracer_plugin)
set_target_properties(GlossyMaterial PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(GlossyMaterial)

add_library(GlassMaterial SHARED GlassMaterialEntry.cpp)
target_link_libraries(GlassMaterial PRIVATE raytracer_components raytracer_plugin)
set_target_properties(GlassMaterial PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(GlassMaterial)

# ── Transformations ──────────────────────────────────────────────────────────

add_library(raytracer_rotation_transformation SHARED
    ../src/components/Transformations/Rotation/Rotation.cpp)
target_link_libraries(raytracer_rotation_transformation PRIVATE raytracer_transformations)
set_target_properties(raytracer_rotation_transformation PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(raytracer_rotation_transformation)

add_library(raytracer_translation_transformation SHARED
    ../src/components/Transformations/Translation/Translation.cpp)
target_link_libraries(raytracer_translation_transformation PRIVATE raytracer_transformations)
set_target_properties(raytracer_translation_transformation PROPERTIES PREFIX "" SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins")
raytracer_enable_target_warnings(raytracer_translation_transformation)
