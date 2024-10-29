add_executable(r3d_imp "pch.cpp" "r3d_imp.cpp" "SceneLoader.cpp")

target_link_libraries(r3d_imp PRIVATE glm assimp yaml-cpp)
target_include_directories(r3d_imp PRIVATE
    "${PROJECT_SOURCE_DIR}/ext/glm"
    "${PROJECT_SOURCE_DIR}/ext/assimp/include"
    "${PROJECT_SOURCE_DIR}/ext/yaml-cpp/include")

target_precompile_headers(r3d_imp PRIVATE "pch.h")