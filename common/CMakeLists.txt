# common/CMakeLists.txt
# We're creating a header-only library by using `INTERFACE` target type
# This ensures that only the header files are included, and no compilation is done

# Create an INTERFACE library called 'common'
add_library(common INTERFACE)

# Include the header files in the common directory for all projects that link with 'common'
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)