set(INCROOT ${PROJECT_SOURCE_DIR}/include/bouge/IOModules/XMLParserCommon/XMLParserModules)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/bouge/IOModules/XMLParserCommon/XMLParserModules/TinyXMLParser)

# all source files
set(SRC
    ${SRCROOT}/bougetinyxml/tinyxml.cpp
    ${SRCROOT}/bougetinyxml/tinyxml.h
    ${SRCROOT}/bougetinyxml/tinyxmlerror.cpp
    ${SRCROOT}/bougetinyxml/tinyxmlparser.cpp
    ${SRCROOT}/TinyXMLParser.cpp
    ${INCROOT}/TinyXMLParser.hpp
)

# define the bouge-tinyxml target
bouge_add_library(bouge-tinyxml
                  SOURCES ${SRC}
                  DEPENDS bouge-xml-common)
