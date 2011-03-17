set(SRCROOT ${CMAKE_SOURCE_DIR}/examples/io)

# all source files
set(SRC ${SRCROOT}/io.cpp)

# define the opengl target
bouge_add_example(io
                  SOURCES ${SRC}
                  DEPENDS bouge bouge-xmlio bouge-tinyxml bouge-math)
