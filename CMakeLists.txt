cmake_minimum_required(VERSION 3.21)

project(Renderer3D)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp build tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Assimp install" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Assimp install PDB" FORCE)
add_subdirectory(ext/assimp)
target_compile_options(assimp PRIVATE -Wno-error=array-bounds)
target_compile_options(assimp PRIVATE -Wno-unknown-pragmas)

add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)
add_subdirectory(ext/yaml-cpp)
add_subdirectory(src)
add_subdirectory(tools/r3d_imp)