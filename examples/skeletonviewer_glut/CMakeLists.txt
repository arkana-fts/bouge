set(SRCROOT ${CMAKE_SOURCE_DIR}/examples/skeletonviewer_glut)
set(COMMONROOT ${CMAKE_SOURCE_DIR}/examples/common_glut)

include_directories(${COMMONROOT})
include_directories(${CMAKE_SOURCE_DIR}/examples/freeglut/include)

add_definitions(-DFREEGLUT_LIB_PRAGMAS=0)
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DFREEGLUT_STATIC)
endif()

# all source files
set(SRC ${SRCROOT}/GLSLSkeletonViewer.cpp
        ${SRCROOT}/GLSLSkeletonViewer.hpp
        ${COMMONROOT}/bicali/bicali.cpp
        ${COMMONROOT}/bicali/bicali.hpp
        ${COMMONROOT}/ourgl3.hpp
        ${COMMONROOT}/Viewer.hpp
        ${COMMONROOT}/Viewer.cpp
        ${COMMONROOT}/main.cpp
   )

if(${WINDOWS})
    set(OTHERLIBS bouge-freeglut)
else()
    set(OTHERLIBS glut)
endif()

# define the opengl target
bouge_add_example(skeletonviewer
                  SOURCES ${SRC}
                  DEPENDS bouge bouge-xmlio bouge-tinyxml bouge-math ${OTHERLIBS})
