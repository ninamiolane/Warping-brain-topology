# Project name
PROJECT (myMean)

#set(ITK_DIR /user/nmiolane/home/BackupHome/Software/InsightToolkit-4.9.0/build)



cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

FIND_PACKAGE(ITK)
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK and ITK was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE( ${ITK_USE_FILE} )


Add_Executable(myMean ${PROJECT_SOURCE_DIR}/myMean.cxx)
TARGET_LINK_LIBRARIES (myMean ${ITK_LIBRARIES})
