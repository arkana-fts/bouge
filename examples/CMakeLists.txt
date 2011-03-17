# add the examples subdirectories
add_subdirectory(io)
add_subdirectory(cal3dx-to-bougexml)
add_subdirectory(moulin-bouge_glut)
add_subdirectory(skeletonviewer_glut)
add_subdirectory(staticviewer_glut)
add_subdirectory(viewer_glut)
add_subdirectory(plot-tf)

if(${WINDOWS})
    add_subdirectory(freeglut)
endif()
