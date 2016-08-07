PROJECT(myWarpImage)

cmake_minimum_required(VERSION 2.4)
#set(ITK_DIR /user/nmiolane/home/BackupHome/Software/InsightToolkit-4.9.0/build)

if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "ITK not found. Please set ITK_DIR.")
ENDIF(ITK_FOUND)
INCLUDE( ${ITK_USE_FILE} )

ADD_EXECUTABLE(myWarpImage_BSpline ${PROJECT_SOURCE_DIR}/myWarpImage_BSpline.cxx )
TARGET_LINK_LIBRARIES(myWarpImage_BSpline ${ITK_LIBRARIES})

