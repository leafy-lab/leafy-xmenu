CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
LIBS    = -lxcb -lxcb-keysyms

SRC = \
    src/leafy_xmenu.c \
		src/leafy_utils.c \
    src/xcb/xcb_graphic.c \
    src/xcb/xcb_event.c \
    src/xcb/xcb_handler.c

BIN = main

.PHONY: all clean run

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LIBS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
