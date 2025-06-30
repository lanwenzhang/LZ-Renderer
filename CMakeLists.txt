# 1 Set version and project name
project(LZRender)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
add_definitions (-DDEBUG)

# 2 Copy assets and .dll
file(GLOB copyResources "./assets" "./thirdParty/bin/assimp/assimp-vc143-mtd.dll")
file(COPY ${copyResources} DESTINATION ${CMAKE_BINARY_DIR})

# 3 Third party
# 3.1 Include third party .h
include_directories(

	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/application/miniz
)

# 3.2 Link third party .lib
link_directories(

	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

# 4 Add subdirectory
add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)
add_subdirectory(imgui)

# 5.1 Main directory - pbr
add_executable(pbr "examples/pbr.cpp" "glad.c")
target_link_libraries(pbr glfw3.lib assimp-vc143-mtd.lib zlibstaticd.lib wrapper app fw imguilib)

# 5.2 Main directory - deferred
add_executable(deferred "examples/deferred.cpp" "glad.c")
target_link_libraries(deferred glfw3.lib assimp-vc143-mtd.lib zlibstaticd.lib wrapper app fw imguilib)

# 5.3 Main directory - shadow
add_executable(shadow "examples/shadow.cpp" "glad.c")
target_link_libraries(shadow glfw3.lib assimp-vc143-mtd.lib zlibstaticd.lib wrapper app fw imguilib)