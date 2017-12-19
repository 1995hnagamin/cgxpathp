.PHONY: clean

GXPATHP = ../gxpathp
CFLAGS = -Wall -Wextra -pedantic $(PKG_CFLAGS)
LIBS = $(PKG_LIBS)
PKG_CFLAGS = $(shell pkg-config --cflags libxml-2.0 2>/dev/null)
PKG_LIBS = $(shell pkg-config --libs libxml-2.0 2>/dev/null)

all: $(GXPATHP)

clean:
	rm -f $(GXPATHP) $(OBJS)

OBJS = gxpathp.o

$(GXPATHP): gxpathp.o
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@
