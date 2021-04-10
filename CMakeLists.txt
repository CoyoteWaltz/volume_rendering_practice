cmake_minimum_required(VERSION 3.5)

project(test)

set(CMAKE_CXX_STANDARD 20)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
find_package(GLFW3 REQUIRED)

# GLEW
find_package(GLEW REQUIRED)

# GLM
find_package(GLM REQUIRED)
if(GLM_FOUND)
    message(STATUS ">>>>GLM found")
endif(GLM_FOUND)

# Put all libraries into a variable
set(LIBS glfw GLEW::GLEW)

# set(STATIC_DIR include/)

# add_library(libs
#     src/triangle.cpp
# )

add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# add_library(myLib STATIC src/std_suggestion.cpp ${STATIC_DIR}/std_suggestion.hpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/
)


target_link_libraries(${PROJECT_NAME} ${LIBS})
# g++ main.cpp -o test.o -lglfw -lGLEW -framework OpenGL