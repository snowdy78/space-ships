
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

# use it after including CTest
enable_testing()
include(CTest)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)

function(link_target_test TARGET)
    target_include_directories(
        ${TARGET}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )
    target_link_libraries(${TARGET} PUBLIC RuneEngine)
    target_link_libraries(${TARGET} PUBLIC Catch2::Catch2WithMain)
    target_link_libraries(${TARGET} PRIVATE ${PROJECT_LIBRARY_NAME})
    target_link_sfml_libraries(${TARGET})
endfunction()

function(add_catch2_test TARGET)

    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)

    link_target_test(${TARGET})

    add_test(NAME project_tests COMMAND ${TARGET})
    catch_discover_tests(${TARGET})
endfunction()

add_executable(proj_main_test ${PROJECT_SOURCE_DIR}/test/main.cpp)

add_catch2_test(main_test)

add_subdirectory(Collision)
