find_package(OpenGL REQUIRED)

add_subdirectory(concurrentqueue)
add_subdirectory(FastNoiseLite)
add_subdirectory(glad)
add_subdirectory(readerwriterqueue)

add_library(robinhood INTERFACE)
target_include_directories(robinhood INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/robin-hood-hashing/src/include)

add_library(livepp INTERFACE)
target_compile_definitions(livepp INTERFACE LIVEPP_ENABLED LIVEPP_DIR=L"${CMAKE_CURRENT_SOURCE_DIR}/LivePP")
target_compile_options(livepp INTERFACE /Z7 /Gy /Gw)
target_link_options(livepp INTERFACE /FUNCTIONPADMIN /OPT:NOREF /OPT:NOICF)
target_include_directories(livepp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/LivePP/API)

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(sdl2)

add_subdirectory(VMem)