cmake_minimum_required(VERSION 3.15)
project(MinimalSDL2App CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Sadly its required to make this cross-platform at elast for linx and win systems
if(WIN32)

    # Embed runtime and libstdc++ for Windows static build
    # Used --allow-multiple-definition to avoid redefinition of std::type_info::operator==
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition")

endif()


# -------------------------------------------------------------
# External Dependencies
# -------------------------------------------------------------
# SDL2
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/SDL2")
    execute_process(
            COMMAND git clone --branch SDL2 https://github.com/libsdl-org/SDL.git "${CMAKE_SOURCE_DIR}/external/SDL2"
            RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Failed to clone SDL2")
    endif()
endif()

# GLM
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/glm")
    execute_process(
            COMMAND git clone https://github.com/g-truc/glm.git "${CMAKE_SOURCE_DIR}/external/glm"
            RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Failed to clone GLM")
    endif()
endif()

# Doctest (header-only)
# Download doctest.h directly instead of cloning full repo.
# This avoids pulling unnecessary files and keeps the external/
# directory minimal. No hash check is used because doctest updates
# frequently, and pinning a hash would require maintenance.
set(DOCTEST_DIR "${CMAKE_SOURCE_DIR}/external/doctest")

if(NOT EXISTS "${DOCTEST_DIR}/doctest.h")
    file(DOWNLOAD
            https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h
            "${DOCTEST_DIR}/doctest.h"
            SHOW_PROGRESS
            STATUS download_status
    )
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Failed to download doctest.h")
    endif()
endif()
# -------------------------------------------------------------

# Add subdirectories
add_subdirectory(external/SDL2)
add_subdirectory(external/glm)

# Source files
file(GLOB_RECURSE CORE_SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/core/*.cpp)

# Filter out files inside any "test" directory
foreach(file IN LISTS CORE_SRC_FILES)
    if(file MATCHES "/test/")
        list(REMOVE_ITEM CORE_SRC_FILES ${file})
    endif()
endforeach()

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


### ─────────────────────────────────────────────────────────────
### Tests Executables
### ─────────────────────────────────────────────────────────────

add_executable(Math3DTest
        ${CMAKE_SOURCE_DIR}/core/common/test/Math3DTest.cpp
        ${CMAKE_SOURCE_DIR}/core/common/src/Math3D.cpp
)

target_include_directories(Math3DTest PRIVATE
        ${CMAKE_SOURCE_DIR}/core
)

target_include_directories(Math3DTest SYSTEM PRIVATE
        ${CMAKE_SOURCE_DIR}/external
)
