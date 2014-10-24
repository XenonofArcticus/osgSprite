include_directories( 
        ${CMAKE_SOURCE_DIR}/include 
        ${OPENTHREADS_INCLUDE_DIR} 
        ${OSGDB_INCLUDE_DIR} 
        ${OSG_INCLUDE_DIR} )
		
IF(WIN32)
ADD_DEFINITIONS(-DOSGSPRITES_LIBRARY)
ENDIF (WIN32)

SET(TARGET_SRC
    fill.cpp
	)

SET( TARGET_LIBS
    ${OPENTHREADS_LIBRARY} 
    ${OSG_LIBRARY} 
    ${OSGDB_LIBRARY} 
    ${OPENGL_gl_LIBRARY} 
	${OSGSPRITES_LIBRART} 
	)
	

# Make sure the linker can find the Hello library once it is built. 
#LINK_DIRECTORIES (${OSGSPRITES_BINARY_DIR}) 

# Add executable called "helloDemo" that is built from the source files 
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found. 
ADD_EXECUTABLE (DotOsg ${TARGET_SRC})

# Link the executable to the Hello library. 
TARGET_LINK_LIBRARIES (DotOsg ${TARGET_LIBS}) 
