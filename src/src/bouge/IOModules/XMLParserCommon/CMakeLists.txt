add_subdirectory(XMLParserModules)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/bouge/IOModules/XMLParserCommon)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/bouge/IOModules/XMLParserCommon)

# all source files
set(SRC
    ${SRCROOT}/XMLAttributes.cpp
    ${INCROOT}/XMLAttributes.hpp
    ${SRCROOT}/XMLHandler.cpp
    ${INCROOT}/XMLHandler.hpp
    ${SRCROOT}/XMLParser.cpp
    ${INCROOT}/XMLParser.hpp
    ${INCROOT}/XMLSerializer.hpp
    ${INCROOT}/XMLSerializer.inl
)

# define the bouge-tinyxml target
bouge_add_library(bouge-xml-common
                  SOURCES ${SRC})
