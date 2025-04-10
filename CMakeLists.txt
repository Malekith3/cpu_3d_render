cmake_minimum_required(VERSION 3.15)
project(MinimalSDL2App CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if SDL2 directory exists, and if not, clone it from the SDL2 branch
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/SDL2")
    execute_process(
            COMMAND git clone --branch SDL2 https://github.com/libsdl-org/SDL.git ${CMAKE_SOURCE_DIR}/external/SDL2
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE error_output
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    if(result)
        message(FATAL_ERROR "Git clone failed: ${error_output}")
    else()
        message(STATUS "SDL2 cloned successfully")
    endif()
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/glm")
    execute_process(
            COMMAND git clone https://github.com/g-truc/glm.git ${CMAKE_SOURCE_DIR}/external/glm
            RESULT_VARIABLE glm_result
            OUTPUT_VARIABLE glm_output
            ERROR_VARIABLE glm_error_output
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    if(glm_result)
        message(FATAL_ERROR "GLM clone failed: ${glm_error_output}")
    else()
        message(STATUS "GLM cloned successfully")
    endif()
endif()


set("SDL2_BUILD_PATH" "${CMAKE_BINARY_DIR}/external/SDL2")
# Add SDL2 as a subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL2)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm)

add_subdirectory(${CMAKE_SOURCE_DIR}/graphics)
add_subdirectory(${CMAKE_SOURCE_DIR}/common)
add_subdirectory(${CMAKE_SOURCE_DIR}/utils)

# Add your application source files
add_executable(${PROJECT_NAME} main.cpp)

# Ensure the correct include directories are used
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_SOURCE_DIR}/include)
# Include directories for header files

# Link SDL2 libraries in the correct order
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2 glm graphics utils)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_BUILD_PATH}/SDL2d.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)