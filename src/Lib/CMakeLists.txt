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
    Sprites.cpp
    TexturePalette.cpp
	)


SET(TARGET_H
        ${HEADER_PATH}/Sprites.h
        ${HEADER_PATH}/TexturePalette.h
	)

set( TARGET_LIBS
    ${OPENTHREADS_LIBRARY} 
    ${OSG_LIBRARY} 
    ${OSGDB_LIBRARY} 
    ${OPENGL_gl_LIBRARY} )

ADD_LIBRARY(osgSprites SHARED ${TARGET_SRC} ${TARGET_H})
TARGET_LINK_LIBRARIES(osgSprites  ${TARGET_LIBS} )


