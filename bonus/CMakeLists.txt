
add_library(bonus_assimp_loader
    Assimp/SceneLoader/AssimpLoader.cpp
)


target_include_directories(bonus_assimp_loader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Assimp/include
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
