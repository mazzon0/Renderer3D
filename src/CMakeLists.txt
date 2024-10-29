set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
file(GLOB_RECURSE SRC_FILES "*.cpp")

add_executable(render3d "${SRC_FILES}")

target_link_libraries(render3d PUBLIC glfw)
target_include_directories(render3d PRIVATE
    "${PROJECT_SOURCE_DIR}/ext")