# @Author: Temple Lin

cmake_minimum_required(VERSION 3.21.1)
set (CMAKE_CXX_STANDARD 17)
project(LTM_CPPOpenGLAPI VERSION 0.1)

#---PASS ALL SOURCE FILES TO EXECUTABLE----------------------------------------------------------
#---"CONFIGURE_DEPENDS" WILL LET CMAKE AUTO CHECK/RESET FILES ON WHEN BUILD IS INVOKED-----------

file(GLOB SOURCES CONFIGURE_DEPENDS
    src/*.c
    src/*.cpp
    src/headers/*.h
    src/headers/*.hpp
    src/shaders/*.glsl

    external/dear_imgui/imconfig.h
    external/dear_imgui/imgui_demo.cpp
    external/dear_imgui/imgui_draw.cpp
    external/dear_imgui/imgui_internal.h
    external/dear_imgui/imgui_tables.cpp
    external/dear_imgui/imgui_widgets.cpp
    external/dear_imgui/imgui.cpp
    external/dear_imgui/imgui.h
    external/dear_imgui/imstb_rectpack.h
    external/dear_imgui/imstb_textedit.h
    external/dear_imgui/imstb_truetype.h
    external/dear_imgui/backends/imgui_impl_opengl3.h
    external/dear_imgui/backends/imgui_impl_opengl3.cpp
    external/dear_imgui/backends/imgui_impl_glfw.h
    external/dear_imgui/backends/imgui_impl_glfw.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------



#---DOWNLOAD ALL THE SUBMODULES------------------------------------------------------------------

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

#------------------------------------------------------------------------------------------------

#---INSTALL DEAR IMGUI AS LIBARY-----------------------------------------------------------------

list(APPEND DEAR_IMGUI_INCLUDE 
    "external/dear_imgui"
    "external/dear_imgui/backends"
)

#------------------------------------------------------------------------------------------------


#---find SFML 2.5--------------------------------------------------------------------------------

add_subdirectory(external/sfml)
set(SFML_DIR "${PROJECT_SOURCE_DIR}/external/sfml/build")
find_package(SFML 2.5 COMPONENTS graphics window system audio network REQUIRED)

if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()
if (CMAKE_BUILD_TYPE EQUAL "Release")
    set(SFML_LINK_DIR "${SFML_DIR}/lib/Release")
else()
    set(SFML_LINK_DIR "${SFML_DIR}/lib/Debug")
endif()

#------------------------------------------------------------------------------------------------


#---CREATE AND LINK DIRS FIRST BEFORE ADDING SUBDIRECTORIES TO PASS VARS TO IT----------------------------
#---GLFW LIBRARY SUBDIRECTORY WILL ALSO PASS TO SUBDIRECTORIES SINCE IT'S ADDED FIRST---------------------

list(APPEND PROJECT_INCLUDE_DIRS
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glad/include"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glad/src"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glfw/include"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glm"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/stb_image"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/dear_imgui"
    PUBLIC "${PROJECT_SOURCE_DIR}/external/sfml/include"
)
list(APPEND PROJECT_LINK_DIRS
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glfw/src"
    PUBLIC "${SFML_LINK_DIR}"
)


target_include_directories(${PROJECT_NAME}
    ${PROJECT_INCLUDE_DIRS}
    # @This one is for including the generated .h file.
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR} #CMAKE_CURRENT_BINARY_DIR: Full path to build directory for project.
    PUBLIC ${DEAR_IMGUI_INCLUDE}
)

target_link_directories(${PROJECT_NAME}
    ${PROJECT_LINK_DIRS}
)

target_link_libraries(${PROJECT_NAME} 
    glfw
    sfml-system sfml-window sfml-graphics sfml-network sfml-audio
)

#--------------------------------------------------------------------------------------------------------------------------------
#--------------------------------------------------------------------------------------------------------------------------------

#---ADD EXPERIMENTAL TESTS DIRECTORY AS SUBDIRECTORY--------------------------------------------------------------------------------------

add_subdirectory(ExperimentalTests/)

#--------------------------------------------------------------------------------------------------------------------------


#---ADD GOOGLE TESTING DIRECTORY AS SUBDIRECTORY--------------------------------------------------------------------------------------
#---COMPARE API AND UNIT TESTING VERSION TO PASS TO CONFIG TO PRINT OUT IN CONSOLE-------------------------------------------------------------

# @Check version of the Unit Testing solution.
option(COMPARE_TESTING_VERSION ON)
# @Add GoogleTest as subdirectory.
add_subdirectory(UnitTesting/GoogleTest)

if(COMPARE_TESTING_VERSION)
    message("API Version: ${CMAKE_PROJECT_VERSION}")
    message("Unit Testing Version: ${TESTING_VERSION}")
endif()

if(DEFINED TESTING_VERSION)
    if(${CMAKE_PROJECT_VERSION} GREATER ${TESTING_VERSION} OR ${CMAKE_PROJECT_VERSION} LESS ${TESTING_VERSION})
        set(API_TESTING_VERSIONNOTMATCH "API and Unit Testing version not match.")
        message("${API_TESTING_VERSIONNOTMATCH}")
    endif()
else()
    message(WARNING "Unit Testing version not set from subdirectory. Can't compare version with API.")
endif()

set(SHOW_API_VERSION "API Version: ")
set(SHOW_TESTING_VERSION "Unit Testing Version: ")
if(DEFINED API_TESTING_VERSIONNOTMATCH)
    set(API_TESTING_VER_COMPARE_MESSAGE "${API_TESTING_VERSIONNOTMATCH}")
else()
    set(API_TESTING_VER_COMPARE_MESSAGE "API and Unit Testing version match.")
endif()

#--------------------------------------------------------------------------------------------------------------------------
#--------------------------------------------------------------------------------------------------------------------------


set(PROJECT_SOURCE_FOLDERPATH "${PROJECT_SOURCE_DIR}")
set(PROJECT_SOURCE_RELATIVEPATH "../src")

configure_file(src/headers/LTM_CPPOpenGLAPIConfig.h.in LTM_CPPOpenGLAPIConfig.h @ONLY)