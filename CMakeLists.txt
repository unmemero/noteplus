cmake_minimum_required(VERSION 3.15)
project(noteplus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Add Dear ImGui sources
set(IMGUI_DIR src/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp  # Add this
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Find GLFW
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Build target
add_executable(noteplus src/main.cpp src/gui/gui.cpp ${IMGUI_SOURCES})
target_link_libraries(noteplus glfw OpenGL::GL)
