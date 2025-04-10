# utils/CMakeLists.txt
# This file will now depend on the 'common' interface library

# Collect all .cpp files from the utils directory
file(GLOB_RECURSE  UTILS_SOURCES "src/*.cpp")
file(GLOB_RECURSE  UTILS_HEADERS "inc/*.h")

include_directories(utils)

# Create a library for utils
add_library(utils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})

# Link the common interface library to utils
target_link_libraries(utils PUBLIC common glm)

# Include directories for utils (the headers from common are already included via `common` link)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)