BUILDDIR = bin
SRCDIR = src

DESTDIR ?= /usr

CFLAGS = -Os -Wall -Wextra

build:
	gcc $(SRCDIR)/main.c $(CFLAGS) -o $(BUILDDIR)/main.out

install:
	sudo cp $(BUILDDIR)/main.out $(PREFIX)/bin/rcv

clean:
	find $(BUILDDIR) -type f ! -name '.gitignore' -delete

uninstall:
	sudo rm $(DESTDIR)/bin/rcv
