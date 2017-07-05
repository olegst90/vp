.PHONY: clean $(ROOT_DIR)/bin/test_input

default: \
	$(ROOT_DIR)/bin/test_list \
	$(ROOT_DIR)/bin/test_input \
	$(ROOT_DIR)/bin/test_pasm.sh

$(ROOT_DIR)/bin/test_list: list.o
	gcc $^ -o $@

$(ROOT_DIR)/bin/test_input: input/*
	cp $^ $@/

$(ROOT_DIR)/bin/test_pasm.sh: test_pasm.sh
		cp $^ $@
		chmod +x $@

clean:
	rm -f *.o
	rm -rf $(ROOT_DIR)/bin/test_* \
	       $(ROOT_DIR)/bin/test_input \
				 $(ROOT_DIR)/bin/test_output \
