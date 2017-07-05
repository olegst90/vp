CFLAGS := -I $(ROOT_DIR)/aux -I $(ROOT_DIR)/machine
$(ROOT_DIR)/bin/pasm: pasm.o parser.o generator.o
	gcc $^ -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(ROOT_DIR)/bin/pasm
