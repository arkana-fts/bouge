# Locate the bouge library
#
# This module defines
#  BOUGE_FOUND, if false, do not try to link to SFML
#  BOUGE_XXX_LIBRARY, library corresponding to the XXX component (release)
#  BOUGE_XXX_LIBRARY_DEBUG, library corresponding to the XXX component (debug)
#  BOUGE_LIBRARIES, list containing all the libraries corresponding to the requested components
#  BOUGE_INCLUDE_DIR, where to find bouge/bouge.hpp
#
# By default, the dynamic libraries of BOUGE will be found. To find the static ones instead,
# you must set the BOUGE_STATIC_LIBRARIES variable to TRUE before calling find_package(bouge ...).
#
# If bouge is not installed in a standard path, you can use the BOUGEDIR CMake variable
# to tell CMake where BOUGE is.

# deduce the libraries suffix from the options
set(FIND_BOUGE_LIB_SUFFIX "")
if(BOUGE_STATIC_LIBRARIES)
    set(FIND_BOUGE_LIB_SUFFIX "${FIND_BOUGE_LIB_SUFFIX}-s")
endif()

# find the SFML include directory
find_path(BOUGE_INCLUDE_DIR bouge/Config.hpp
          PATH_SUFFIXES include
          PATHS
          ~/Library/Frameworks
          /Library/Frameworks
          /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/
          ${BOUGEDIR})

# check the version number
set(BOUGE_VERSION_OK TRUE)
if(BOUGE_FIND_VERSION AND BOUGE_INCLUDE_DIR)
    # extract the major and minor version numbers from bouge/Config.hpp
    FILE(READ "${BOUGE_INCLUDE_DIR}/bouge/Config.hpp" BOUGE_CONFIG_HPP_CONTENTS)
    STRING(REGEX REPLACE ".*#define BOUGE_VERSION_MAJOR ([0-9]+).*" "\\1" BOUGE_VERSION_MAJOR "${BOUGE_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define BOUGE_VERSION_MINOR ([0-9]+).*" "\\1" BOUGE_VERSION_MINOR "${BOUGE_CONFIG_HPP_CONTENTS}")
    math(EXPR BOUGE_REQUESTED_VERSION "${BOUGE_FIND_VERSION_MAJOR} * 10 + ${BOUGE_FIND_VERSION_MINOR}")

    # if we could extract them, compare with the requested version number
    if(BOUGE_VERSION_MAJOR)
        # transform version numbers to an integer
        math(EXPR BOUGE_VERSION "${BOUGE_VERSION_MAJOR} * 10 + ${BOUGE_VERSION_MINOR}")

        # compare them
        if(BOUGE_VERSION LESS BOUGE_REQUESTED_VERSION)
            set(BOUGE_VERSION_OK FALSE)
        endif()
    endif()
endif()

# find the requested components
set(FIND_BOUGE_LIB_PATHS ~/Library/Frameworks
                         /Library/Frameworks
                         /usr/local
                         /usr
                         /sw
                         /opt/local
                         /opt/csw
                         /opt
                         ${BOUGEDIR})
foreach(FIND_BOUGE_COMPONENT ${BOUGE_FIND_COMPONENTS})
    string(TOLOWER ${FIND_BOUGE_COMPONENT} FIND_BOUGE_COMPONENT_LOWER)
    string(TOUPPER ${FIND_BOUGE_COMPONENT} FIND_BOUGE_COMPONENT_UPPER)
    set(FIND_BOUGE_COMPONENT_VAR BOUGE_${FIND_BOUGE_COMPONENT_UPPER}_LIBRARY)
    set(FIND_BOUGE_COMPONENT_NAME bouge-${FIND_BOUGE_COMPONENT_LOWER}${FIND_BOUGE_LIB_SUFFIX})

    # release library
    find_library(${FIND_BOUGE_COMPONENT_VAR}
                 NAMES ${FIND_BOUGE_COMPONENT_NAME}
                 PATH_SUFFIXES lib64 lib
                 PATHS ${FIND_BOUGE_LIB_PATHS})

    # debug library
    find_library(${FIND_BOUGE_COMPONENT_VAR}_DEBUG
                 NAMES ${FIND_BOUGE_COMPONENT_NAME}-d
                 PATH_SUFFIXES lib64 lib
                 PATHS ${FIND_BOUGE_LIB_PATHS})

    set(BOUGE_LIBRARIES_NAMES ${BOUGE_LIBRARIES_NAMES} ${FIND_BOUGE_COMPONENT_VAR})
    set(BOUGE_LIBRARIES ${BOUGE_LIBRARIES} ${${FIND_BOUGE_COMPONENT_VAR}})
endforeach()

if(BOUGE_FIND_REQUIRED AND NOT BOUGE_VERSION_OK)
    message(SEND_ERROR "Bad bouge version (requested: ${BOUGE_FIND_VERSION}, found: ${BOUGE_VERSION_MAJOR}.${BOUGE_VERSION_MINOR})")
    set(BOUGE_FOUND FALSE)
else()
    # handle the QUIETLY and REQUIRED arguments and set BOUGE_FOUND to TRUE if all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(BOUGE DEFAULT_MSG BOUGE_INCLUDE_DIR ${BOUGE_LIBRARIES_NAMES})
    MARK_AS_ADVANCED(BOUGE_INCLUDE_DIR ${BOUGE_LIBRARIES_NAMES})
endif()
