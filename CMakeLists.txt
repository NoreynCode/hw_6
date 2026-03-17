cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(Matrix VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

# Библиотека matrix_lib
add_library(matrix_lib INTERFACE)

target_include_directories(matrix_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Основной исполняемый файл
add_executable(matrix src/main.cpp)

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(matrix
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(matrix PRIVATE
    matrix_lib
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    
    add_executable(matrix_tests tests/tests.cpp)
    
    set_target_properties(matrix_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_include_directories(matrix_tests
        PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    set_target_properties(matrix_tests PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
    
    target_link_libraries(matrix_tests
        ${Boost_LIBRARIES}
        matrix_lib
    )
endif()

if (MSVC)
    target_compile_options(matrix PRIVATE /W4)
    if(WITH_BOOST_TEST)
        target_compile_options(matrix_tests PRIVATE /W4)
    endif()
else ()
    target_compile_options(matrix PRIVATE -Wall -Wextra -pedantic -Werror)
    if(WITH_BOOST_TEST)
        target_compile_options(matrix_tests PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()

install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(matrix_tests matrix_tests)
endif()