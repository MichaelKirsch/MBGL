cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME MBGL)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h") #glob all the source files intp one place

file(GLOB_RECURSE HEADERS "src/*.h")


file(COPY ${HEADERS} "libs/SFML-2.5.1/include/" "libs/glad/" "libs/glm/glm" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include)
file(COPY "libs/SFML-2.5.1/bin/openal32.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "data"  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories(libs/glad/)
include_directories(src)
include_directories(src/Support)
include_directories(src/StateMachine)
include_directories(src/Meta)
include_directories(libs/glm)
include_directories(src/Window)
find_package(OpenGL REQUIRED)

set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_USE_STATIC_STD_LIBS TRUE)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCES} libs/glad/glad.c )

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio ${OPENGL_LIBRARIES})

