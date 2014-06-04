# Top level makefile, the real shit is at src/Makefile

default: all

.DEFAULT:
	cd deps/leveldb && make
	rm -rf /usr/local/lib/libleveldb.so.1
	rm -rf /usr/local/lib/libleveldb.so
	cp ./deps/leveldb/libleveldb.so.1 /usr/local/lib
	cp ./deps/leveldb/libleveldb.so /usr/local/lib
	cd src && $(MAKE) $@

install:
	cd src && $(MAKE) $@

.PHONY: install
