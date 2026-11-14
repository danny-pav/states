# Glob only files in the current directory (non-recursive)
file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB LIB_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

set(LIB_NAME "states")
# Create the static library
add_library(${LIB_NAME} STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Allow the main executable to include headers from this directory
target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
