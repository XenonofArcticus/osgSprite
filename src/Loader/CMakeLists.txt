

include_directories( 
        ${CMAKE_SOURCE_DIR}/include 
        ${OPENTHREADS_INCLUDE_DIR} 
        ${OSGDB_INCLUDE_DIR} 
        ${OSG_INCLUDE_DIR} )

IF(WIN32)
ADD_DEFINITIONS(-DOSGSPRITES_LIBRARY)
ENDIF (WIN32)


SET(TARGET_LIBS
    osgSprites 
    ${OPENTHREADS_LIBRARY} 
    ${OSG_LIBRARY} 
    ${OSGDB_LIBRARY} 
    ${OPENGL_gl_LIBRARY} )

SET(TARGET_SRC
    SpriteLoader.cpp
    SpriteSerDes.cpp
	)

SET(TARGET_H
    PositionElement.h
    SpritesElement.h
    TexturePaletteElement.h
    SpriteDataElement.h
    SpriteSerDes.h
	)


ADD_LIBRARY(osgdb_sprites MODULE ${TARGET_SRC} ${TARGET_H} )
SET_TARGET_PROPERTIES( osgdb_sprites PROPERTIES PREFIX "" )

TARGET_LINK_LIBRARIES(osgdb_sprites ${TARGET_LIBS} )


