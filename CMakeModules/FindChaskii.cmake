
find_path( II_INCLUDE_DIR Chaskii/Chaskii.h
        HINTS
        "$ENV{CHASKII_DIR}"
        PATH_SUFFIXES include
        PATHS
        /usr
        /usr/local )

find_library( 
        IIXML_LIBRARY
        NAMES iiXML
        HINTS
        "$ENV{CHASKII_DIR}" 
        PATH_SUFFIXES lib64 lib
        /usr
        /usr/local
        )

find_library( 
        IIMEM_LIBRARY
        NAMES iiMem
        HINTS
        "$ENV{CHASKII_DIR}" 
        PATH_SUFFIXES lib64 lib
        /usr
        /usr/local
        )
