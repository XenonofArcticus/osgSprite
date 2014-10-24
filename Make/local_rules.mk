CMAKE_BUILD_DIR = CMakeBuild.opt
CMAKE_DEBUG_BUILD_DIR = CMakeBuild.dbg

cmake:
	[ -d $(CMAKE_BUILD_DIR) ] || (mkdir -p $(CMAKE_BUILD_DIR); cd $(CMAKE_BUILD_DIR); cmake ../)
	cd $(CMAKE_BUILD_DIR); make
	[ -d lib/$(OSARCH) ] || mkdir -p lib/$(OSARCH)
	cd lib/$(OSARCH); for f in ../../$(CMAKE_BUILD_DIR)/lib/*.so; do ln -sf $$f .; done

cmake_debug:
	[ -d $(CMAKE_DEBUG_BUILD_DIR) ] || (mkdir -p $(CMAKE_DEBUG_BUILD_DIR); cd $(CMAKE_DEBUG_BUILD_DIR); cmake -DBUILD_TYPE=Debug ../)
	cd $(CMAKE_DEBUG_BUILD_DIR); make
	[ -d lib/$(OSARCH) ] || mkdir -p lib/$(OSARCH)
	cd lib/$(OSARCH); for f in ../../$(CMAKE_DEBUG_BUILD_DIR)/lib/*.so; do ln -sf $$f .; done


