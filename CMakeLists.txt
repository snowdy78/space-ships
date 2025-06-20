cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)
set(MY_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(MY_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MY_PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${MY_PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MY_PROJECT_SOURCE_DIR}/bin)
# disable build .dll files
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

function(target_link_sfml TARGET)
    target_link_libraries(${TARGET} PUBLIC sfml-graphics)
    target_link_libraries(${TARGET} PUBLIC sfml-window)
    target_link_libraries(${TARGET} PUBLIC sfml-audio)
    target_link_libraries(${TARGET} PUBLIC sfml-system)
    target_link_libraries(${TARGET} PUBLIC sfml-network)
    target_link_libraries(${TARGET} PUBLIC sfml-main)
endfunction()


FetchContent_Declare(RuneEngine
    GIT_REPOSITORY https://github.com/snowdy78/cmake-rune-engine.git
    GIT_TAG 0.3.1)
FetchContent_MakeAvailable(RuneEngine)
file(
    GLOB_RECURSE
    PROJECT_SOURCE_FILES
    ${MY_PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE
    PROJECT_INCLUDE_FILES
    ${MY_PROJECT_SOURCE_DIR}/include/*.hpp
)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES} ${PROJECT_INCLUDE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC RuneEngine)
target_link_sfml(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${MY_PROJECT_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
add_subdirectory(test)
