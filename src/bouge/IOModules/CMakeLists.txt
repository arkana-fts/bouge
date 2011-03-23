# add an option for choosing the build type (shared or static)

if(NOT DEFINED BOUGE_BUILD_XMLIO)
    set(BOUGE_BUILD_XMLIO TRUE)
endif()
set(BUILD_BOUGEIO_XML ${BOUGE_BUILD_XMLIO} CACHE BOOL "TRUE to build the bouge-xml i/o modules (necessary to read .bx* files)")

if(NOT DEFINED BOUGE_BUILD_CAL3DXMLIO)
    set(BOUGE_BUILD_CAL3DXMLIO TRUE)
endif()
set(BUILD_BOUGEIO_CAL3DX ${BOUGE_BUILD_CAL3DXMLIO} CACHE BOOL "TRUE to build the bouge Cal3D XML i/o modules (necessary to read Cal3D's .xmf, .xrf, .xsf, ... files)")

if(BUILD_BOUGEIO_XML OR BUILD_BOUGEIO_CAL3DX)
    add_subdirectory(XMLParserCommon)
endif()

if(BUILD_BOUGEIO_XML)
    add_subdirectory(XML)
endif()

if(BUILD_BOUGEIO_CAL3DX)
    add_subdirectory(Cal3dX)
endif()