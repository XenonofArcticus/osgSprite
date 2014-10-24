set( HEADER_PATH ${CMAKE_SOURCE_DIR}/include/osgSprites )

include_directories( 
        ${CMAKE_SOURCE_DIR}/include 
        ${OPENTHREADS_INCLUDE_DIR}
        ${OSGDB_INCLUDE_DIR} 
        ${OSG_INCLUDE_DIR} )

IF(WIN32)
ADD_DEFINITIONS(-DOSGSPRITES_LIBRARY)
ENDIF (WIN32)


SET(TARGET_SRC
        IO_Sprites.cpp
	)

SET( TARGET_LIBS
        osgSprites
        ${OSGDB_LIBRARY}
        ${OSG_LIBRARY}
   )


ADD_LIBRARY(osgdb_osgsprites MODULE ${TARGET_SRC} )
SET_TARGET_PROPERTIES( osgdb_osgsprites PROPERTIES PREFIX "" )
TARGET_LINK_LIBRARIES( osgdb_osgsprites ${TARGET_LIBS} )
