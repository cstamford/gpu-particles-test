file(GLOB_RECURSE SRC *.cpp *.hpp)
add_executable(borealis WIN32 ${SRC})

IF (WIN32)
    string(REGEX REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    target_compile_options(borealis PRIVATE /W4 /EHs-c- /GR-)
    target_compile_definitions(borealis PRIVATE _HAS_EXCEPTIONS=0 _CRT_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN NOMINMAX)
    target_link_libraries(borealis PRIVATE $<$<CONFIG:DEBUG>:livepp>)
ENDIF()

target_include_directories(borealis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(borealis PRIVATE concurrentqueue glad SDL2-static SDL2main VMem ${OPENGL_LIBRARIES})
