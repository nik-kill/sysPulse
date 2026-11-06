cmake_minimum_required(VERSION 3.20)

project(SysPulse VERSION 0.2.0 LANGUAGES CXX)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(NOT WIN32)
	message(FATAL_ERROR "SysPulse targets Windows and requires a Windows build.")
endif()

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.91.9b
	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_demo.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)
target_compile_features(imgui PUBLIC cxx_std_20)

find_package(OpenGL REQUIRED)

add_executable(syspulse src/main.cpp)
target_compile_features(syspulse PRIVATE cxx_std_20)
target_link_libraries(syspulse PRIVATE imgui OpenGL::GL)

add_library(syspulse_core STATIC
	src/collector.cpp
	src/factory.cpp
	platforms/windows_collector.cpp
)
target_include_directories(syspulse_core
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/platforms
)
target_compile_features(syspulse_core PUBLIC cxx_std_20)

add_executable(syspulse_collector src/collector_console.cpp)
target_include_directories(syspulse_collector PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/platforms
)
target_compile_features(syspulse_collector PRIVATE cxx_std_20)
target_link_libraries(syspulse_collector PRIVATE syspulse_core)

if(WIN32)
	target_link_libraries(syspulse PRIVATE opengl32)
endif()

if(MSVC)
	target_compile_options(syspulse PRIVATE /W4 /permissive-)
	target_compile_options(syspulse_core PRIVATE /W4 /permissive-)
	target_compile_options(syspulse_collector PRIVATE /W4 /permissive-)
else()
	target_compile_options(syspulse PRIVATE -Wall -Wextra -Wpedantic)
	target_compile_options(syspulse_core PRIVATE -Wall -Wextra -Wpedantic)
	target_compile_options(syspulse_collector PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(syspulse PROPERTIES
	CXX_EXTENSIONS OFF
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(syspulse_collector PROPERTIES
	CXX_EXTENSIONS OFF
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
