CFLAGS  := -I../include -I/usr/local/include -Wall
LDFLAGS := -L/usr/local/lib
LIBS    := -ltcod

ifndef RELEASE
  CFLAGS += -g
  LIBS   += -lefence
else
  CFLAGS += -O3
endif

SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(shell echo $(SRC_FILES) |sed 's/\.c/.o/g')

all: libnetcrack.a

libnetcrack.a: libnetcrack.a($(OBJ_FILES))
	ranlib libnetcrack.a

clean:
	rm -f libnetcrack.a
	rm -f $(OBJ_FILES)

mrclean:
	rm -f *~

.PHONY: all
