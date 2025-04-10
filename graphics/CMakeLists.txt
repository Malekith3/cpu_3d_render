# graphics/CMakeLists.txt
file(GLOB_RECURSE  GRAPHICS_SRC "*.cpp")
file(GLOB_RECURSE  GRAPHICS_HDR "*.h")

add_library(graphics ${GRAPHICS_SRC} ${GRAPHICS_HDR})
target_link_libraries(graphics PUBLIC common glm)

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rendering/inc
                                           ${CMAKE_CURRENT_SOURCE_DIR}/shapes/inc )