cmake_minimum_required(VERSION 3.21.1)
project(LTM_CPPOpenGLAPI_ExperimentalTests)
set (CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} CPPOGLAPI_ExperimentTestsMain.cpp)

target_link_libraries( ${PROJECT_NAME}
    PUBLIC glfw
)
target_link_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_LINK_DIRS}
)
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_INCLUDE_DIRS}
)

configure_file(CPPOGLAPI_ExperimentTestsConfig.h.in CPPOGLAPI_ExperimentTestsConfig.h @ONLY)