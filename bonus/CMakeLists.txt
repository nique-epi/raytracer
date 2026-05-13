add_library(bonus_stb_image STATIC
    stb_image/stb_image_impl.cpp
)

target_include_directories(bonus_stb_image
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/stb_image
)

target_compile_options(bonus_stb_image PRIVATE -w)

add_library(bonus_assimp_loader
    Assimp/SceneLoader/AssimpLoader.cpp
    Assimp/SceneLoader/AssimpLoaderRegistration.cpp
    Assimp/SceneLoader/LightsParser.cpp
    Assimp/SceneLoader/NodeParser.cpp
    Assimp/SceneLoader/MaterialParser.cpp
    Assimp/SceneLoader/CameraParser.cpp
)


target_include_directories(bonus_assimp_loader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
)


target_link_libraries(bonus_assimp_loader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Assimp/libassimp.a
        config++
        raytracer_components
        raytracer_math
        raytracer_scene
        z
)


raytracer_enable_target_warnings(bonus_assimp_loader)
