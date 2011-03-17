set(INCROOT ${PROJECT_SOURCE_DIR}/include/bouge/IOModules/XML)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/bouge/IOModules/XML)

# all source files
set(SRC
    ${SRCROOT}/CoreAnimation_Handler.cpp
    ${INCROOT}/CoreAnimation_Handler.hpp
    ${SRCROOT}/CoreMaterial_Handler.cpp
    ${INCROOT}/CoreMaterial_Handler.hpp
    ${SRCROOT}/CoreMaterialSet_Handler.cpp
    ${INCROOT}/CoreMaterialSet_Handler.hpp
    ${SRCROOT}/CoreMesh_Handler.cpp
    ${INCROOT}/CoreMesh_Handler.hpp
    ${SRCROOT}/CoreSkeleton_Handler.cpp
    ${INCROOT}/CoreSkeleton_Handler.hpp
    ${SRCROOT}/Loader.cpp
    ${INCROOT}/Loader.hpp
    ${SRCROOT}/Saver.cpp
    ${INCROOT}/Saver.hpp
)

# define the bouge-xmlio target
bouge_add_library(bouge-xmlio
                  SOURCES ${SRC}
                  DEPENDS bouge bouge-xml-common)
