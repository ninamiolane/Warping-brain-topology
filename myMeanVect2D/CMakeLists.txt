# Project name
PROJECT (myMeanVect)

cmake_minimum_required(VERSION 2.4)
set(ITK_DIR /home/mhadjham/ITK/build/)
#set(ITK_DIR /user/nmiolane/home/BackupHome/Software/InsightToolkit-4.9.0/build)



#Select useful libraries
SET(ITK_LIBRARIES
ITKCommon
ITKIO
ITKStatistics
ITKNumerics
ITKAlgorithms
ITKLabelMap
)

if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

FIND_PACKAGE(ITK)
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK and ITK was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE(${ITK_USE_FILE})


Add_Executable(myMeanVect ${PROJECT_SOURCE_DIR}/myMeanVect.cxx)
TARGET_LINK_LIBRARIES ( myMeanVect ${ITK_LIBRARIES})
