UW_INSTALL_PREFIX ?= /usr/local
UW_DOCTESTFLAGS ?=

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
	./build/tests/uw_tests $(UW_DOCTESTFLAGS)

.PHONY: clean
clean:
	rm -rf build

.PHONY: install
install:	build
	cmake --install build --prefix $(UW_INSTALL_PREFIX)


.PHONY: header
header:
	./scripts/amalgamate_header.sh

.PHONY: test-single-header
test-single-header:	build header
	cmake -S . -B build_single_header -DUW_USE_SINGLE_HEADER=ON
	cmake --build build_single_header
	./build_single_header/tests/uw_tests $(UW_DOCTESTFLAGS)

.PHONY: test-all
test-all: test test-single-header



