TOPDIR = ../../
include $(DWMAKE)/makedefs

COMPILER_INCLUDE += -I$(THISDIR)/../../include

CXXFILES =  IO_Sprites.cpp\

LINKER_ARGS += -L$(TOPDIR)/lib/$(OSARCH)
LIBS = -losgSprites  

LIB = osgdb_osgsprites.so

include $(DWMAKE)/makerules
