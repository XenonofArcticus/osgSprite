TOPDIR = ../../
include $(DWMAKE)/makedefs

COMPILER_INCLUDE += -I$(THISDIR)/../../include

CXXFILES =  SpriteLoader.cpp\
	        SpriteSerDes.cpp

LINKER_ARGS += -L$(TOPDIR)/lib/$(OSARCH)
LIBS = -losgSprites -liiXML 

LIB = osgdb_sprites.so

include $(DWMAKE)/makerules
