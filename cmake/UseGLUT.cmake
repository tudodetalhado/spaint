###################
# UseGLUT.cmake #
###################

OPTION(WITH_GLUT "Build with Glut support?" OFF)

IF(WITH_GLUT)
  IF(MSVC_IDE)
    FIND_PATH(GLUT_ROOT Readme.txt HINTS "$ENV{HOMEPATH}/Downloads/freeglut")
    FIND_LIBRARY(GLUT_LIBRARY freeglut HINTS "${GLUT_ROOT}/lib/x64")
    FIND_PATH(GLUT_INCLUDE_DIR GL/glut.h HINTS "${GLUT_ROOT}/include")
  ELSEIF(APPLE)
    FIND_PACKAGE(GLUT REQUIRED)
  ELSEIF("${CMAKE_SYSTEM}" MATCHES "Linux")
    FIND_LIBRARY(GLUT_LIBRARY glut HINTS "/usr/lib/x86_64-linux-gnu")
    FIND_PATH(GLUT_INCLUDE_DIR glut.h HINTS "/usr/include/GL")
  ENDIF()

  INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
  ADD_DEFINITIONS(-DWITH_GLUT)
ENDIF()
