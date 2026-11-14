cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)
project(
    STATES
    VERSION 0.0.0
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(states)

FILE(GLOB TASK_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB TASK_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

SET(TASK_NAME "main")
add_executable(${TASK_NAME} ${TASK_SOURCES} ${TASK_HEADERS})
target_compile_options(${TASK_NAME} PRIVATE -g -Wall -Werror -Wno-deprecated-declarations)

# Link the static library to the executable
target_link_libraries(${TASK_NAME} PRIVATE states)
