cmake_minimum_required(VERSION 2.8)

# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(bouge)

# include the configuration file
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# add the SFML header path
include_directories(${PROJECT_SOURCE_DIR}/include)

# add an option for choosing the build type (shared or static)
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "TRUE to build bouge as shared libraries, FALSE to build it as static libraries")

# add an option for building the examples
set(BUILD_EXAMPLES TRUE CACHE BOOL "TRUE to build the bouge examples, FALSE to ignore them")

# add an option for building the API documentation
set(BUILD_DOC FALSE CACHE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# add an option for choosing to statically link to the CRT
set(BUILD_STATICCRT FALSE CACHE BOOL "TRUE to statically link against the C Runtime, FALSE to dynamically link agains the C Runtime")

# define BOUGE_STATIC if the build type is not set to 'shared'
if(BUILD_SHARED_LIBS)
    add_definitions(-DBOUGE_EXPORT)
else()
    add_definitions(-DBOUGE_STATIC)
endif()

# remove SL security warnings with Visual C++
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

# Enable all kinds of warnings
if(COMPILER_GCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -fstack-protector-all")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -fstack-protector-all")
    endif()
endif()

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# add the subdirectories
add_subdirectory(src/bouge)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_DOC)
    add_subdirectory(doc)
endif()

#We may want to statically link to CRT to reduce dependancies
if(BUILD_STATICCRT)
    if(MSVC)
#        foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
#            if(${flag_var} MATCHES "/MD")
#                string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
#            endif(${flag_var} MATCHES "/MD")
#            if(${flag_var} MATCHES "/MDd")
#               string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
#            endif(${flag_var} MATCHES "/MDd")
#        endforeach(flag_var)
        foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            message(STATUS ${${flag_var}})
        endforeach()
    endif()
endif()

# setup the install rules
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        PATTERN ".svn" EXCLUDE)
install(FILES cmake/Modules/Findbouge.cmake DESTINATION ${CMAKE_ROOT}/Modules)
# install(FILES license.txt DESTINATION ${INSTALL_MISC_DIR})
