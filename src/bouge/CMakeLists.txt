# include the bouge specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# add the bouge sources path
include_directories(${PROJECT_SOURCE_DIR}/src)

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# set the output directory for bouge libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# define the export symbol
add_definitions(-DBOUGE_EXPORT)

# add the modules subdirectories
add_subdirectory(Math)
add_subdirectory(IOModules)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/bouge)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/bouge)

# all source files
set(SRC
    ${INCROOT}/bouge.hpp
    ${INCROOT}/bougefwd.hpp
    ${SRCROOT}/Animation.cpp
    ${INCROOT}/Animation.hpp
    ${SRCROOT}/BoneInstance.cpp
    ${INCROOT}/BoneInstance.hpp
    ${SRCROOT}/CoreAnimation.cpp
    ${INCROOT}/CoreAnimation.hpp
    ${SRCROOT}/CoreBone.cpp
    ${INCROOT}/CoreBone.hpp
    ${SRCROOT}/CoreHardwareMesh.cpp
    ${INCROOT}/CoreHardwareMesh.hpp
    ${SRCROOT}/CoreKeyframe.cpp
    ${INCROOT}/CoreKeyframe.hpp
    ${SRCROOT}/CoreMaterial.cpp
    ${INCROOT}/CoreMaterial.hpp
    ${SRCROOT}/CoreMaterialSet.cpp
    ${INCROOT}/CoreMaterialSet.hpp
    ${SRCROOT}/CoreMesh.cpp
    ${INCROOT}/CoreMesh.hpp
    ${SRCROOT}/CoreModel.cpp
    ${INCROOT}/CoreModel.hpp
    ${SRCROOT}/CoreSkeleton.cpp
    ${INCROOT}/CoreSkeleton.hpp
    ${SRCROOT}/CoreTrack.cpp
    ${INCROOT}/CoreTrack.hpp
    ${SRCROOT}/Exception.cpp
    ${INCROOT}/Exception.hpp
    ${SRCROOT}/Face.cpp
    ${SRCROOT}/Loader.cpp
    ${INCROOT}/Loader.hpp
    ${SRCROOT}/Mixer.cpp
    ${INCROOT}/Mixer.hpp
    ${SRCROOT}/ModelInstance.cpp
    ${INCROOT}/ModelInstance.hpp
    ${SRCROOT}/Saver.cpp
    ${INCROOT}/Saver.hpp
    ${SRCROOT}/SkeletonInstance.cpp
    ${INCROOT}/SkeletonInstance.hpp
    ${SRCROOT}/StaticModelInstance.cpp
    ${INCROOT}/StaticModelInstance.hpp
    ${SRCROOT}/UserData.cpp
    ${INCROOT}/UserData.hpp
    ${SRCROOT}/Util.cpp
    ${INCROOT}/Util.hpp
    ${SRCROOT}/Vertex.cpp
    ${INCROOT}/Vertex.hpp
)

# define the sfml-audio target
bouge_add_library(bouge SOURCES ${SRC} DEPENDS bouge-math)
