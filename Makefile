UW_INSTALL_PREFIX ?= /usr/local

.PHONY: all
all:	build

.PHONY:	config
config:
	cmake -S . -B build

.PHONY:	build
build:	config
	cmake --build build

.PHONY: test
test:	build
	cd build && ctest --output-on-failure

.PHONY: clean
clean:
	rm -rf build

.PHONY: install
install:	build
	cmake --install build --prefix $(UW_INSTALL_PREFIX)
