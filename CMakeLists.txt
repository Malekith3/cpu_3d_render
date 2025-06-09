cmake_minimum_required(VERSION 3.15)
project(MinimalSDL2App CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)

    # Embed runtime and libstdc++ for Windows static build
    # Used --allow-multiple-definition to avoid redefinition of std::type_info::operator==
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition")

endif()

# Clone SDL2 if not present
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/SDL2")
    execute_process(
            COMMAND git clone --branch SDL2 https://github.com/libsdl-org/SDL.git external/SDL2
            RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Failed to clone SDL2")
    endif()
endif()

# Clone GLM if not present
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/glm")
    execute_process(
            COMMAND git clone https://github.com/g-truc/glm.git external/glm
            RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Failed to clone GLM")
    endif()
endif()

# Add subdirectories
add_subdirectory(external/SDL2)
add_subdirectory(external/glm)

# Source files
file(GLOB_RECURSE CORE_SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/core/*.cpp)

# Executable
add_executable(${PROJECT_NAME} main.cpp ${CORE_SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/core)

target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE SDL2::SDL2 glm
)

# Copy SDL2 DLL post-build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL2::SDL2>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
