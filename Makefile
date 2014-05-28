# Top level makefile, the real shit is at src/Makefile

default: all

.DEFAULT:
	cd deps/leveldb && make
	cd src && $(MAKE) $@

install:
	cd src && $(MAKE) $@

.PHONY: install
