set(SRCROOT ${CMAKE_SOURCE_DIR}/examples/plot-tf)
set(INCROOT ${CMAKE_SOURCE_DIR}/examples/plot-tf)

# all source files
set(SRC ${SRCROOT}/plot.cpp
        ${SRCROOT}/lodepng.cpp
        ${INCROOT}/lodepng.h)

# define the opengl target
bouge_add_example(plot-tf
                  SOURCES ${SRC}
                  DEPENDS bouge-math)
