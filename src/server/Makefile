CFLAGS  := -I../../include -I/usr/local/include -Wall
LDFLAGS := -L/usr/local/lib -L../../lib
LIBS    := -ltcod

ifndef RELEASE
  CFLAGS += -g
  LIBS   += -lefence
else
  CFLAGS += -O3
endif

SRC_FILES := $(wildcard *.c)                      # foo.c
OBJ_FILES := $(patsubst %.c,%.o,$(wildcard *.c))  # foo.c -> foo.o

all: $(OBJ_FILES) netcrackd

$(OBJ_FILES): %.o: %.c ../../lib/libnetcrack.a
	$(CC) $(CFLAGS) -c -o $@ $<

netcrackd: $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $(OBJ_FILES) ../../lib/libnetcrack.a

clean:
	rm -f $(OBJ_FILES)
	rm -f netcrackd

mrclean: clean
	rm -f *~

.PHONY: all run clean mrclean
