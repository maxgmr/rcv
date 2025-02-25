BUILDDIR = bin
SRCDIR = src

DESTDIR ?= /usr/local

CFLAGS = -Os -Wall -Wextra

build:
	gcc $(SRCDIR)/main.c $(CFLAGS) -o $(BUILDDIR)/main.out

install:
	install -Dm755 $(BUILDDIR)/main.out $(DESTDIR)/bin/rcv

clean:
	find $(BUILDDIR) -type f ! -name '.gitignore' -delete

uninstall:
	rm $(DESTDIR)/bin/rcv
