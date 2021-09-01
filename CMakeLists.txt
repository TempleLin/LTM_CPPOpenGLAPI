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
)

add_executable(${PROJECT_NAME} ${SOURCES})

#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------



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



#---CREATE AND LINK DIRS FIRST BEFORE ADDING GOOGLE TESTING SUBDIRECTORY TO PASS VARS TO IT----------------------------
#---GLFW LIBRARY SUBDIRECTORY WILL ALSO PASS TO GOOGLE TESTING SUBDIRECTORY SINCE IT'S ADDED FIRST---------------------

list(APPEND PROJECT_INCLUDE_DIRS
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glad/include" 
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glad/src" 
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glfw/include" 
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glm"
    PUBLIC "${CMAKE_SOURCE_DIR}/external/stb_image"
)
list(APPEND PROJECT_LINK_DIRS
    PUBLIC "${CMAKE_SOURCE_DIR}/external/glfw/src"
)


target_include_directories(${PROJECT_NAME}
    ${PROJECT_INCLUDE_DIRS}
    # @This one is for including the generated .h file.
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR} #CMAKE_CURRENT_BINARY_DIR: Full path to build directory for project.
)

target_link_directories(${PROJECT_NAME}
    ${PROJECT_LINK_DIRS}
)

target_link_libraries(${PROJECT_NAME} glfw)

#--------------------------------------------------------------------------------------------------------------------------------
#--------------------------------------------------------------------------------------------------------------------------------



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


configure_file(src/headers/LTM_CPPOpenGLAPIConfig.h.in LTM_CPPOpenGLAPIConfig.h @ONLY)

