CFLAGS  := -I../include -I/usr/local/include -Wall
LDFLAGS := -L/usr/local/lib -L../lib
LIBS    := -ltcod

ifndef RELEASE
  CFLAGS += -g
  LIBS   += -lefence
else
  CFLAGS += -O3
endif

SRC_FILES := $(wildcard *.c)                    # foo.c
OBJ_FILES := $(patsubst %.c,%,$(wildcard *.c))  # foo.c -> foo
X_TESTS   := disp_test

all: $(OBJ_FILES)

$(OBJ_FILES): %: %.c ../lib/libnetcrack.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $< ../lib/libnetcrack.a

run: $(OBJ_FILES)
	@echo
	@for test in $(OBJ_FILES); do \
		echo ------------------------------------------------------------ $$test; \
		if [ -z "$$DISPLAY" ]; then \
			if echo $(X_TESTS) |grep -q $$test; then \
				echo "Skipping $$test as it requires a GUI"; \
				continue; \
			fi; \
		fi; \
		./$$test; \
		if [ "$$?" != "0" ]; then \
			echo '!!!' Test $$test failed; \
			echo; \
			exit 1; \
		fi; \
		echo; \
	done

clean:
	rm -f $(OBJ_FILES)

mrclean: clean
	rm -f *~

.PHONY: all run clean mrclean
