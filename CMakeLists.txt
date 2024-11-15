cmake_minimum_required(VERSION 3.14)
project(OptionLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gather source files for the library
file(GLOB_RECURSE LIBRARY_SOURCES src/*.cpp)

# Create the library target
add_library(OptionLib STATIC ${LIBRARY_SOURCES})

# Specify the include directory for the library target
target_include_directories(OptionLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Example executable for testing
add_executable(example examples/main.cpp)

# Link the library to the example executable
target_link_libraries(example PRIVATE OptionLib)

# Enable testing with CTest
include(CTest)
enable_testing()

# Set up Google Test using FetchContent
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0  # Choose a specific tag or branch
)

# Prevent Google Test from overriding CMake's standard variables
# (by default it sets these to ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Fetch and link Google Test
FetchContent_MakeAvailable(googletest)

find_package(Boost REQUIRED)  # Do not specify components for Boost.Math
if (Boost_FOUND)
    target_include_directories(OptionLib PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(OptionLib PUBLIC ${Boost_LIBRARIES})
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
endif()

find_package(Eigen3 REQUIRED NO_MODULE)
if (Eigen3_FOUND)
    target_link_libraries(OptionLib PUBLIC Eigen3::Eigen)
    message(STATUS "Eigen3 found: ${Eigen3_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Eigen3 not found")
endif()

# Define a test executable for GTest-based tests
add_executable(run_tests tests/OptionPricingTest.cpp
                         tests/HestonCharacteristicFunctionTest.cpp)

target_link_libraries(run_tests PRIVATE OptionLib gtest gtest_main)
add_test(NAME OptionLibTests COMMAND run_tests)

# Install library, headers, and executables
install(TARGETS OptionLib DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS example run_tests DESTINATION bin)