cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)
set(MY_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(MY_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MY_PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${MY_PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MY_PROJECT_SOURCE_DIR}/bin)
include(FetchContent)

FetchContent_Declare(RuneEngine
    GIT_REPOSITORY https://github.com/snowdy78/cmake-rune-engine.git
    GIT_TAG 0.3.0)
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

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(${PROJECT_NAME} PUBLIC RuneEngine)

target_include_directories(${PROJECT_NAME} PRIVATE "${MY_PROJECT_SOURCE_DIR}/include")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM)
endif()

function(target_link_sfml_libraries TARGET)
    target_link_libraries(${TARGET} PUBLIC sfml-graphics)
    target_link_libraries(${TARGET} PUBLIC sfml-network)
    target_link_libraries(${TARGET} PUBLIC sfml-window)
    target_link_libraries(${TARGET} PUBLIC sfml-system)
    target_link_libraries(${TARGET} PUBLIC sfml-audio)
    target_link_libraries(${TARGET} PUBLIC sfml-main)
endfunction()

set(PROJECT_LIBRARY_NAME ${PROJECT_NAME}Lib)

add_library(${PROJECT_LIBRARY_NAME} STATIC ${PROJECT_SOURCE_FILES} ${PROJECT_INCLUDE_FILES})
target_link_libraries(${PROJECT_LIBRARY_NAME} PUBLIC RuneEngine)
target_link_sfml_libraries(${PROJECT_LIBRARY_NAME})
target_include_directories(
    ${PROJECT_LIBRARY_NAME}
    PRIVATE
    ${APP_SOURCE_DIR}/include
)
target_include_directories(
    ${PROJECT_LIBRARY_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
add_subdirectory(test)
