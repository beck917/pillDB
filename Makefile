# Top level makefile, the real shit is at src/Makefile

default: all

.DEFAULT:
	ldconfig
	cd deps/leveldb && make
	rm -rf /usr/local/lib/libleveldb.so.1
	cp ./deps/leveldb/libleveldb.so.1 /usr/local/lib
	ln -s libleveldb.so.1 libleveldb.so
	ldconfig
	cd src && $(MAKE) $@

install:
	cd src && $(MAKE) $@

.PHONY: install
