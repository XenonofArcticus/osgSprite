# This module defines

# OSGSPRITES_LIBRARY
# OSGSPRITES_FOUND, if false, do not try to link to osg
# OSGSPRITES_INCLUDE_DIRS, where to find the headers
# OSGSPRITES_INCLUDE_DIR, where to find the source headers

# to use this module, set variables to point to the osg build
# directory, and source directory, respectively
# OSGSPRITESDIR or OSGSPRITES_SOURCE_DIR: osg source directory, typically OpenSceneGraph
# OSGSPRITES_DIR or OSGSPRITES_BUILD_DIR: osg build directory, place in which you've
#    built osg via cmake

# Header files are presumed to be included like
# #include <osg/PositionAttitudeTransform>
# #include <osgUtil/SceneView>

###### headers ######

SET(OSGSPRITES_DIR "" CACHE PATH "Set to base osgSprites install path")

MACRO( FIND_OSGSPRITES_INCLUDE THIS_OSGSPRITES_INCLUDE_DIR THIS_OSGSPRITES_INCLUDE_FILE )

FIND_PATH( ${THIS_OSGSPRITES_INCLUDE_DIR} ${THIS_OSGSPRITES_INCLUDE_FILE}
    PATHS
        ${OSGSPRITES_DIR}
        $ENV{OSGSPRITES_SOURCE_DIR}
        $ENV{OSGSPRITESDIR}
        $ENV{OSGSPRITES_DIR}
        /usr/local/
        /usr/
        /sw/ # Fink
        /opt/local/ # DarwinPorts
        /opt/csw/ # Blastwave
        /opt/
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSGSPRITES_ROOT]/
        ~/Library/Frameworks
        /Library/Frameworks
    PATH_SUFFIXES
        /include/
)

ENDMACRO( FIND_OSGSPRITES_INCLUDE THIS_OSGSPRITES_INCLUDE_DIR THIS_OSGSPRITES_INCLUDE_FILE )

FIND_OSGSPRITES_INCLUDE( OSGSPRITES_INCLUDE_DIR       OSGSPRITES/Export.h )

###### libraries ######

MACRO( FIND_OSGSPRITES_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
    PATHS
        ${OSGSPRITES_DIR}
        $ENV{OSGSPRITES_BUILD_DIR}
        $ENV{OSGSPRITES_DIR}
        $ENV{OSGSPRITESDIR}
        $ENV{OSG_ROOT}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSGSPRITES_ROOT]/lib
        /usr/freeware
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
     )

ENDMACRO(FIND_OSGSPRITES_LIBRARY LIBRARY LIBRARYNAME)

FIND_OSGSPRITES_LIBRARY( OSGSPRITES_LIBRARY OSGSPRITES)

SET( OSGSPRITES_FOUND "NO" )
IF( OSGSPRITES_LIBRARY AND OSGSPRITES_INCLUDE_DIR )
    SET( OSGSPRITES_FOUND "YES" )
    SET( OSGSPRITES_INCLUDE_DIRS ${OSGSPRITES_INCLUDE_DIR})
    GET_FILENAME_COMPONENT( OSGSPRITES_LIBRARIES_DIR ${OSGSPRITES_LIBRARY} PATH )
ENDIF( OSGSPRITES_LIBRARY AND OSGSPRITES_INCLUDE_DIR )
