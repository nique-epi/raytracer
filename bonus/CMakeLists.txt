# Bonus library with Assimp integration
add_library(bonus_assimp_loader
    Assimp/SceneLoader/AssimpLoader.cpp
)

# Include directories
target_include_directories(bonus_assimp_loader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Assimp/include
        ${CMAKE_SOURCE_DIR}/src
)

# Link the precompiled Assimp library
target_link_libraries(bonus_assimp_loader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Assimp/libassimp.a
)

# Apply project warnings
raytracer_enable_target_warnings(bonus_assimp_loader)
