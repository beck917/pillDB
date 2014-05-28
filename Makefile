# Top level makefile, the real shit is at src/Makefile

default: all

.DEFAULT:
	cd deps/leveldb && make
	rm -rf /usr/local/lib/libleveldb.so.1
	cp ./deps/leveldb/libleveldb.so.1 /usr/local/lib
	ldconfig
	cd src && $(MAKE) $@

install:
	cd src && $(MAKE) $@

.PHONY: install
