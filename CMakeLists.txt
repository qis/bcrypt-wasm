cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(main VERSION 1.3.0 LANGUAGES C CXX)

option(BUILD_WASM "Build WebAssembly" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER build)

file(GLOB_RECURSE website docs/*.html docs/*.css docs/*.js)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/docs PREFIX docs FILES ${website})

file(GLOB_RECURSE headers include/*.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX include FILES ${headers})

file(GLOB_RECURSE sources src/*.h src/*.c src/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX src FILES ${sources})

file(GLOB exports main.syms)

add_executable(main ${website} ${headers} ${sources} ${exports})
target_include_directories(main PRIVATE include)

if(BUILD_WASM)
  target_link_libraries(main PRIVATE "-Wl,--allow-undefined-file=${exports}")
  install(TARGETS main RUNTIME DESTINATION docs)
else()
  set_target_properties(main PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  install(TARGETS main RUNTIME DESTINATION bin)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

