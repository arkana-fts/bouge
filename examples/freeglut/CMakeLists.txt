# include the bouge specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# add the freeglut sourcecode
include_directories(${PROJECT_SOURCE_DIR}/examples/freeglut/include)

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# set the output directory for bouge libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# define the export symbol
if(BUILD_SHARED_LIBS)
    add_definitions(-DFREEGLUT_EXPORTS)
else()
    add_definitions(-DFREEGLUT_STATIC)
endif()
add_definitions(-DFREEGLUT_LIB_PRAGMAS=0)

# add the modules subdirectories
#add_subdirectory(Math)

set(INCROOT ${PROJECT_SOURCE_DIR}/examples/freeglut/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/freeglut/src)

# all source files
set(SRC
    ${SRCROOT}/freeglut_callbacks.c
    ${SRCROOT}/freeglut_cursor.c
    ${SRCROOT}/freeglut_display.c
    ${SRCROOT}/freeglut_ext.c
    ${SRCROOT}/freeglut_font.c
    ${SRCROOT}/freeglut_font_data.c
    ${SRCROOT}/freeglut_gamemode.c
    ${SRCROOT}/freeglut_geometry.c
    ${SRCROOT}/freeglut_glutfont_definitions.c
    ${SRCROOT}/freeglut_init.c
    ${SRCROOT}/freeglut_input_devices.c
    ${SRCROOT}/freeglut_internal.h
    ${SRCROOT}/freeglut_joystick.c
    ${SRCROOT}/freeglut_main.c
    ${SRCROOT}/freeglut_menu.c
    ${SRCROOT}/freeglut_misc.c
    ${SRCROOT}/freeglut_overlay.c
    ${SRCROOT}/freeglut_spaceball.c
    ${SRCROOT}/freeglut_state.c
    ${SRCROOT}/freeglut_stroke_mono_roman.c
    ${SRCROOT}/freeglut_stroke_roman.c
    ${SRCROOT}/freeglut_structure.c
    ${SRCROOT}/freeglut_teapot.c
    ${SRCROOT}/freeglut_teapot_data.h
    ${SRCROOT}/freeglut_videoresize.c
    ${SRCROOT}/freeglut_window.c
    ${INCROOT}/GL/freeglut.h
    ${INCROOT}/GL/freeglut_ext.h
    ${INCROOT}/GL/freeglut_std.h
    ${INCROOT}/GL/glut.h
)

set(LIBS DEPENDS "")
if(${MSVC})
    set(LIBS DEPENDS opengl32.lib winmm.lib)
endif()

bouge_add_library(bouge-freeglut SOURCES ${SRC} ${LIBS})

# create the target
#add_library(freeglut ${SRC})

#if(BUILD_SHARED_LIBS)
#else()
#    set_target_properties(freeglut PROPERTIES DEBUG_POSTFIX _static)
#    set_target_properties(freeglut PROPERTIES RELEASE_POSTFIX _static)
#endif()

# for gcc 4.x on Windows, we add the -static-libgcc linker flag to get rid of an extra gcc DLL
#if(WINDOWS AND COMPILER_GCC)
#    if(${GCC_VERSION} MATCHES "4\\..*")
#        set_target_properties(freeglut PROPERTIES LINK_FLAGS -static-libgcc)
#    endif()
#endif()

# add the install rule
#install(TARGETS freeglut
#        RUNTIME DESTINATION bin COMPONENT bin
#        LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT bin
#        ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel)
