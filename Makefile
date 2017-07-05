export ROOT_DIR := $(shell pwd)

.PHONY: machine pasm test clean

default: machine pasm test

dirs:
		mkdir -p $(ROOT_DIR)/bin
		mkdir -p $(ROOT_DIR)/bin/test_input
		mkdir -p $(ROOT_DIR)/bin/test_output

machine: dirs
	make -C machine

pasm: dirs
	make -C pasm

test: dirs
	make -C test

clean:
	make -C machine clean
	make -C pasm clean
	make -C test clean
