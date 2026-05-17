# Déterminer l'extension de la bibliothèque Assimp selon l'OS
if(APPLE)
    set(ASSIMP_LIB_EXT ".a")
elseif(UNIX AND NOT APPLE)
    set(ASSIMP_LIB_EXT ".so")
else()
    message(WARNING "OS non reconnu pour Assimp, utilisation de .a par défaut")
    set(ASSIMP_LIB_EXT ".a")
endif()

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
        ${CMAKE_CURRENT_SOURCE_DIR}/Assimp/libassimp${ASSIMP_LIB_EXT}
        config++
        raytracer_components
    raytracer_helper
        raytracer_math
        raytracer_scene
        z
)


raytracer_enable_target_warnings(bonus_assimp_loader)

add_library(bonus_json_loader
    json/JsonSettingsLoader.cpp
)

target_include_directories(bonus_json_loader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bonus_json_loader
    PUBLIC
        nlohmann_json::nlohmann_json
        raytracer_scene
)

raytracer_enable_target_warnings(bonus_json_loader)


if(APPLE)
    set(OIDN_LIB_EXT ".dylib")
elseif(UNIX AND NOT APPLE)
    set(OIDN_LIB_EXT ".so")
else()
    message(WARNING "OS non reconnu pour OpenImageDenoise, utilisation de .dylib par défaut")
    set(OIDN_LIB_EXT ".dylib")
endif()

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
    ${CMAKE_CURRENT_SOURCE_DIR}/OID/lib/libOpenImageDenoise${OIDN_LIB_EXT}
    raytracer_components

)

raytracer_enable_target_warnings(bonus_OID_denoiser)
