# @Author: Temple Lin

cmake_minimum_required(VERSION 3.21.1)
set (CMAKE_CXX_STANDARD 17)
project(LTM_CPPOpenGLAPI VERSION 0.1)

add_executable(${PROJECT_NAME} src/LTM_CPPOpenGLAPI.cpp)

configure_file(src/headers/LTM_CPPOpenGLAPI.h.in LTM_CPPOpenGLAPI.h @ONLY)


#       DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()
#       CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed.")
endif()


add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME}
    PRIVATE external/glad/include
    PRIVATE external/glad/src
    PRIVATE external/glfw/include
    PRIVATE external/glm
    # @This one is for including the generated .h file.
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR} #CMAKE_CURRENT_BINARY_DIR: Full path to build directory for project.
    #${EXTRA_INCLUDES_DIRS}
)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    #${EXTRA_LIB_DIRS}
)

target_link_libraries(${PROJECT_NAME} glfw)