set(INCROOT ${PROJECT_SOURCE_DIR}/include/bouge/IOModules/Cal3dX)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/bouge/IOModules/Cal3dX)

# all source files
set(SRC
    ${SRCROOT}/CoreAnimation_Handler.cpp
    ${INCROOT}/CoreAnimation_Handler.hpp
    ${SRCROOT}/CoreMaterial_Handler.cpp
    ${INCROOT}/CoreMaterial_Handler.hpp
    ${SRCROOT}/CoreMesh_Handler.cpp
    ${INCROOT}/CoreMesh_Handler.hpp
    ${SRCROOT}/CoreSkeleton_Handler.cpp
    ${INCROOT}/CoreSkeleton_Handler.hpp
    ${SRCROOT}/Loader.cpp
    ${INCROOT}/Loader.hpp
)

# define the bouge-cal3dxio target
bouge_add_library(bouge-cal3dxio
                  SOURCES ${SRC}
                  DEPENDS bouge bouge-xml-common)
