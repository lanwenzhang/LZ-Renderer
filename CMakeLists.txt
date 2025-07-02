# 1 Set version and project name
project(GL_Render_Pad)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
add_definitions (-DDEBUG)

# 2 Copy assets and .dll
file(GLOB copyResources "./assets" "./renderer/shaders""./third_party/bin/assimp/assimp-vc143-mtd.dll" )
file(COPY ${copyResources} DESTINATION ${CMAKE_BINARY_DIR})

# 3 Third party
# 3.1 Include third party .h
include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/loader/miniz
)

# 3.2 Link third party .lib
link_directories(

	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib
)

# 4 Add subdirectory
add_subdirectory(core)
add_subdirectory(imgui)
add_subdirectory(loader)
add_subdirectory(renderer)
add_subdirectory(tools)
add_subdirectory(wrapper)

# 5.1 Main directory - pbr
add_executable(pbr_demo "examples/pbr_demo.cpp" "glad.c")
target_link_libraries(pbr_demo glfw3.lib assimp-vc143-mtd.lib zlibstaticd.lib core imguilib loader renderer tools wrapper)

# 5.2 Main directory - deferred
add_executable(deferred_demo "examples/deferred_demo.cpp" "glad.c")
target_link_libraries(deferred_demo glfw3.lib assimp-vc143-mtd.lib zlibstaticd.lib core imguilib loader renderer tools wrapper)

# 5.3 Main directory - shadow
add_executable(shadow_demo "examples/shadow_demo.cpp" "glad.c")
target_link_libraries(shadow_demo glfw3.lib assimp-vc143-mtd.lib zlibstaticd.lib core imguilib loader renderer tools wrapper)