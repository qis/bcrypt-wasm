MAKEFLAGS += --no-print-directory

CC	:= wasm-clang
CXX	:= wasm-clang++

PROJECT	!= grep "^project" CMakeLists.txt | cut -c9- | cut -d " " -f1 | tr "[:upper:]" "[:lower:]"
SOURCES	!= find include src -type f -name "*.h" -or -name "*.c" -or -name "*.cpp" -or -name "*.syms"

all: build/wasm/CMakeCache.txt $(SOURCES)
	@cmake --build build/wasm --target install

build/wasm/CMakeCache.txt: CMakeLists.txt build/wasm
	@cd build/wasm && CC=$(CC) CXX=$(CXX) cmake -GNinja -DCMAKE_BUILD_TYPE=MinSizeRel \
	  -DCMAKE_INSTALL_PREFIX=$(PWD) -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON -DBUILD_WASM=ON $(PWD)

build/wasm:
	@mkdir -p build/wasm

format:
	@clang-format -i $(SOURCES)

clean:
	@rm -rf build/wasm

.PHONY: all format clean
