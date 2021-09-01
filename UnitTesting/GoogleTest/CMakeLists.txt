cmake_minimum_required(VERSION 3.21.1)
project(LTM_CPPOpenGLAPI_UNIT_TESTING VERSION 0.0)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# @Give version of the Unit Testing solution to root.
if (COMPARE_TESTING_VERSION)
  set (TESTING_VERSION "${PROJECT_VERSION}" PARENT_SCOPE)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable( ${PROJECT_NAME}
  LTM_CPPOpenGLAPI_UNIT_TESTING.cpp
)
target_link_libraries( ${PROJECT_NAME}
  PUBLIC gtest_main
  PUBLIC glfw
)
target_link_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_LINK_DIRS}
)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_INCLUDE_DIRS}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})