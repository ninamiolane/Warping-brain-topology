PROJECT(SVFExponential)

cmake_minimum_required(VERSION 2.4)
#set(ITK_DIR /home/mhadjham/ITK/build/)
#set(ITK_DIR /user/nmiolane/home/BackupHome/Software/InsightToolkit-4.9.0/build)

if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

FIND_PACKAGE(ITK)
IF( NOT ITK_FOUND )
    MESSAGE( "Project ${PROJECT_NAME} requires ITK and ITK was not found. ${PROJECT_NAME} will not be built." )
    RETURN()
ENDIF()
INCLUDE( ${ITK_USE_FILE} )


#SET(MODULES_DIR )
INCLUDE_DIRECTORIES(	${PROJECT_SOURCE_DIR}/ ${MODULES_DIR}/)


ADD_EXECUTABLE(SVFExponential ${PROJECT_SOURCE_DIR}/SVFExponential.cxx ${MODULES_DIR})
#TARGET_LINK_LIBRARIES(SVFExponential ${ITK_LIBRARIES} ${MODULES_DIR}/itkDivideByConstantImageFilter.h)
TARGET_LINK_LIBRARIES(SVFExponential ${ITK_LIBRARIES} )
