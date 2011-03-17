set(INCROOT ${PROJECT_SOURCE_DIR}/include/bouge/Math)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/bouge/Math)

# all source files
set(SRC
    ${SRCROOT}/Vector.cpp
    ${INCROOT}/Vector.hpp
    ${SRCROOT}/Matrix.cpp
    ${INCROOT}/Matrix.hpp
    ${SRCROOT}/Quaternion.cpp
    ${INCROOT}/Quaternion.hpp
    ${SRCROOT}/TimeFunction.cpp
    ${INCROOT}/TimeFunction.hpp
    ${INCROOT}/Util.hpp
)

# define the bouge-tinyxml target
bouge_add_library(bouge-math
                  SOURCES ${SRC})
