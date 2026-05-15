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
    raytracer_helper
        raytracer_math
        raytracer_scene
        z
)


raytracer_enable_target_warnings(bonus_assimp_loader)

add_library(bonus_OID_denoiser
    postprocess/denoise/OIDDenoiser.cpp
)

target_include_directories(bonus_OID_denoiser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/OID/OpenImageDenoise/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(bonus_OID_denoiser
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/OID/lib/libOpenImageDenoise.dylib
    raytracer_components

)

raytracer_enable_target_warnings(bonus_OID_denoiser)
