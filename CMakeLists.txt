cmake_minimum_required(VERSION 3.22)

#---- Project Configuration
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

project(
	EldenRingLauncher
	LANGUAGES CXX
	VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(GENERATED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/include)

#---- SDL2
add_subdirectory(./dependencies/SDL2)

#---- ImGui
set(IMGUI_DIRECTORY ./dependencies/ImGui)

file(GLOB IMGUI_SOURCE_FILES
	${IMGUI_DIRECTORY}/imgui.cpp
	${IMGUI_DIRECTORY}/imgui_demo.cpp
	${IMGUI_DIRECTORY}/imgui_draw.cpp
	${IMGUI_DIRECTORY}/imgui_tables.cpp
	${IMGUI_DIRECTORY}/imgui_widgets.cpp
	${IMGUI_DIRECTORY}/backends/imgui_impl_sdl.cpp
	${IMGUI_DIRECTORY}/backends/imgui_impl_sdlrenderer.cpp
)
file(GLOB_RECURSE IMGUI_HEADERS ${IMGUI_DIRECTORY}/*.h)
file(COPY ${IMGUI_HEADERS} DESTINATION ${GENERATED_HEADERS})

#---- Ruda Font
include_directories(./dependencies/RudaFont/include)

#---- Boost
add_subdirectory(./dependencies/Boost)

#---- Project Source
file(GLOB_RECURSE SOURCE_FILES ./src/*.cpp ./src/*.h)
file(GLOB_RECURSE TEMPLATE_HEADERS ./resources/*.h.in)

foreach (OLDFILE ${TEMPLATE_HEADERS})
	get_filename_component(NEWFILE ${OLDFILE} NAME_WLE)

	configure_file(${OLDFILE} ${GENERATED_HEADERS}/${NEWFILE})
endforeach ()

#---- Include Generated Headers
include_directories(${GENERATED_HEADERS})

#---- Compilation
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES} ${IMGUI_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "start_protected_game")

#---- Linking
target_link_libraries(${PROJECT_NAME} SDL2main SDL2-static Boost::process)
