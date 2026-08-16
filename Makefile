UW_INSTALL_PREFIX ?= /usr/local
CTESTFLAGS = --output-on-failure

.PHONY: all
all:	build

.PHONY:	config
config:
	cmake -S . -B build

.PHONY:	build
build:	config
	cmake --build build

.PHONY: test
test:	build check-tests
	cd build && ctest ${CTESTFLAGS}

.PHONY: clean
clean:
	rm -rf build

.PHONY: install
install:	build
	cmake --install build --prefix $(UW_INSTALL_PREFIX)

.PHONY: check-tests
check-tests:
	@if [[ -x "./scripts/check_tests.sh" ]]; then \
		./scripts/check_tests.sh; \
	else \
		echo "./scripts/check_tests.sh is not executable"; \
	fi \

.PHONY: header
header:
	./scripts/amalgamate_header.sh

.PHONY: test-single-header
test-single-header:	build header
	cmake -S . -B build_single_header -DUW_USE_SINGLE_HEADER=ON
	cmake --build build_single_header
	cd build_single_header && ctest $(CTESTFLAGS)

.PHONY: test-all
test-all: check-tests test test-single-header



