CFLAGS := -I $(ROOT_DIR)/aux -I $(ROOT_DIR)/machine

default: $(ROOT_DIR)/bin/pasm $(ROOT_DIR)/bin/pdump

$(ROOT_DIR)/bin/pasm: pasm.o parser.o generator.o pobj.o
	gcc $^ -o $@

$(ROOT_DIR)/bin/pdump: pdump.o pobj.o
		gcc $^ -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(ROOT_DIR)/bin/pasm
	rm -f $(ROOT_DIR)/bin/pdump
