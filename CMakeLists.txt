cmake_minimum_required(VERSION 3.16)

project(termat LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE _UNICODE)

# Diretório de saída dos executáveis
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Includes públicos da engine (<termat/...>)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# --- Engine library (static)
file(GLOB_RECURSE ENGINE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_SOURCE_DIR}/src/gfx/*.cpp
    ${CMAKE_SOURCE_DIR}/src/input/*.cpp
    ${CMAKE_SOURCE_DIR}/src/scene/*.cpp
    ${CMAKE_SOURCE_DIR}/src/res/*.cpp
    ${CMAKE_SOURCE_DIR}/src/net/*.cpp
    ${CMAKE_SOURCE_DIR}/src/sys/*.cpp
)

add_library(termat STATIC ${ENGINE_SOURCES})
target_include_directories(termat PUBLIC
    ${CMAKE_SOURCE_DIR}/src/include
)

# --- Executável do usuário
add_executable(main ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(main PRIVATE termat)

# Força saída organizada para builds multi-config (Visual Studio)
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release
)
