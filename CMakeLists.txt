cmake_minimum_required(VERSION 3.5)

project(learn_opengl)

set(CMAKE_CXX_STANDARD 20)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
find_package(GLFW3 REQUIRED)

# GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    link_libraries(${GLEW_LIBRARIES})
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

# GLM
find_package(GLM REQUIRED)
if(GLM_FOUND)
    message(STATUS ">>>>GLM found")
endif(GLM_FOUND)

set(STATIC_DIR include/)
# renderer lib
add_library(renderer STATIC src/custom/renderer.cpp ${STATIC_DIR}/custom/renderer.h)
# vertex_buffer lib
add_library(vertex_buffer STATIC src/custom/vertex_buffer.cpp ${STATIC_DIR}/custom/vertex_buffer.h)
# index_buffer lib
add_library(index_buffer STATIC src/custom/index_buffer.cpp ${STATIC_DIR}/custom/index_buffer.h)
# vertex_array lib
add_library(vertex_array STATIC src/custom/vertex_array.cpp ${STATIC_DIR}/custom/vertex_array.h)
# vertex_buffer_layout lib
add_library(vertex_buffer_layout STATIC src/custom/vertex_buffer_layout.cpp ${STATIC_DIR}/custom/vertex_buffer_layout.h)
# shader lib
add_library(shader STATIC src/custom/shader.cpp ${STATIC_DIR}/custom/shader.h)
# texture lib
add_library(texture STATIC src/custom/texture.cpp ${STATIC_DIR}/custom/texture.h)

# utils lib
add_library(utils STATIC src/utils.cpp ${STATIC_DIR}/utils.h)

# Put all libraries into a variable
set(LIBS
    glfw
    GLEW::GLEW
    renderer
    vertex_buffer
    index_buffer
    vertex_array
    vertex_buffer_layout
    shader
    texture
    utils
)
# renderer
target_include_directories(renderer
    PUBLIC
    # 如果是 INTERFACE myLib 本身不会 include 路径 就找不到 hpp 文件了会报错
        ${STATIC_DIR}
)
# vertex_buffer
target_include_directories(vertex_buffer
    PUBLIC
        ${STATIC_DIR}
)
# index_buffer
target_include_directories(index_buffer
    PUBLIC
        ${STATIC_DIR}
)
# vertex_array
target_include_directories(vertex_array
    PUBLIC
        ${STATIC_DIR}
)
# vertex_buffer_layout
target_include_directories(vertex_buffer_layout
    PUBLIC
        ${STATIC_DIR}
)
# shader
target_include_directories(shader
    PUBLIC
        ${STATIC_DIR}
)
# texture
target_include_directories(texture
    PUBLIC
        ${STATIC_DIR}
)
# utils
target_include_directories(utils
    PUBLIC
        ${STATIC_DIR}
)

# 自己的 lib 也需要 link
target_link_libraries(utils GLEW::GLEW)
target_link_libraries(renderer utils)
target_link_libraries(vertex_buffer utils)
target_link_libraries(index_buffer utils)
target_link_libraries(vertex_array utils)
target_link_libraries(vertex_buffer_layout utils)
target_link_libraries(shader utils)
target_link_libraries(texture utils)

add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/
)


target_link_libraries(${PROJECT_NAME} ${LIBS})
# g++ main.cpp -o test.o -lglfw -lGLEW -framework OpenGL