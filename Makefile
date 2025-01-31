BUILD_DIR = bin
SRC_DIR = src

PREFIX ?= /usr/local

CFLAGS = -Os -Wall -Wextra

build:
	gcc $(SRC_DIR)/main.c $(CFLAGS) -o $(BUILD_DIR)/main.out

install:
	sudo cp $(BUILD_DIR)/main.out $(PREFIX)/bin/rcv

clean:
	find $(BUILD_DIR) -type f ! -name '.gitignore' -delete

uninstall:
	sudo rm $(PREFIX)/bin/rcv
