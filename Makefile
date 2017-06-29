export ROOT_DIR := $(shell pwd)

.PHONY: machine test clean

default: machine test

dirs:
		mkdir -p $(ROOT_DIR)/bin

machine: dirs
	make -C machine

test: dirs
	make -C test

clean:
	make -C machine clean
	make -C test clean
