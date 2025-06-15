cmake_minimum_required(VERSION 3.22.1)

# Set project name and version
project(Redislite VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all .cpp files recursively from Base/src
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Base/src/*.cpp)

# Include headers from Base/include
# include_directories(${CMAKE_SOURCE_DIR}/Base/include)

# Add the executable target
add_executable(JaisriRama ${SRC_FILES})

target_include_directories(JaisriRama PRIVATE ${CMAKE_SOURCE_DIR}/Base/include)

# message(STATUS "Sources: ${SRC_FILES}")


find_package(GTest QUIET)

if (NOT GTest_FOUND)
    message(STATUS "System GTest not found. Fetching via FetchContent...")

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # for MSVC
    FetchContent_MakeAvailable(googletest)
endif()


enable_testing()
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/tests/*.cpp)
add_executable(UnitTests ${TEST_FILES}
                         ${CMAKE_SOURCE_DIR}/Base/src/JaiSriRama.cpp
)

target_include_directories(UnitTests PRIVATE
    ${CMAKE_SOURCE_DIR}/Base/include
    ${CMAKE_SOURCE_DIR}/tests/include     
)

# Link GTest (either system or downloaded)
if (GTest_FOUND)
    target_link_libraries(UnitTests GTest::GTest GTest::Main)
else()
    target_link_libraries(UnitTests gtest_main)
endif()

include(GoogleTest)
gtest_discover_tests(UnitTests)




