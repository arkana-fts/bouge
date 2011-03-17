set(SRCROOT ${CMAKE_SOURCE_DIR}/examples/cal3dx-to-bougexml)

# all source files
set(SRC ${SRCROOT}/cal3dx-to-bougexml.cpp)

# define the opengl target
bouge_add_example(cal3dx-to-bougexml
                  SOURCES ${SRC}
                  DEPENDS bouge bouge-cal3dxio bouge-xmlio bouge-tinyxml bouge-math)
