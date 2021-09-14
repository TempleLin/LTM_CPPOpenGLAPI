cmake_minimum_required(VERSION 3.21.1)
project(LTM_CPPOpenGLAPI_ExperimentalTests)
set (CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} CPPOGLAPI_ExperimentTestsMain.cpp glad.c)

if (CMAKE_BUILD_TYPE EQUAL "Release")
    set(SFML_LINK_DIR "${SFML_DIR}/lib/Release")
else()
    set(SFML_LINK_DIR "${SFML_DIR}/lib/Debug")
endif()

target_link_libraries( ${PROJECT_NAME}
    PUBLIC glfw
    sfml-system sfml-window sfml-graphics sfml-network sfml-audio
)
target_link_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_LINK_DIRS}
)
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_INCLUDE_DIRS}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(CPPOGLAPI_ExperimentTestsConfig.h.in CPPOGLAPI_ExperimentTestsConfig.h @ONLY)