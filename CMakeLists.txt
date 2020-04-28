cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME MBGL)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
link_libraries(stdc++fs) #link the filesystem

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h") #glob all the source files intp one place
file(GLOB_RECURSE HEADERS "src/*.h")


file(COPY ${HEADERS} "libs/SFML-2.5.1/include/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include)
file(COPY "libs/SFML-2.5.1/bin/openal32.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories("libs/glad/include/")

find_package(OpenGL REQUIRED)

set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCES} libs/glad/src/glad.c)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} sfml-audio sfml-graphics sfml-window sfml-system)

