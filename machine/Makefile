CFLAGS := -I $(ROOT_DIR)/aux
$(ROOT_DIR)/bin/machine: machine.o cpu_default.o iosoc.o memctrl.o ram.o
	gcc $^ -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(ROOT_DIR)/bin/machine
