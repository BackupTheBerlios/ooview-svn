SOURCEFILES = src/$(wildcard *.c) src/$(wildcard *.h)
BINDIR=bin

GCC = /usr/bin/gcc
GCCOPTS = -lmenu -lncurses -lxml2
GCCNOLINK = -ansi -I/usr/include/libxml2 -c
RM = rm -f

bin/ooview: $(SOURCEFILES)
	$(GCC) $(GCCNOLINK) src/metaparser.c
	$(GCC) $(GCCNOLINK) src/main.c
	$(GCC) $(GCCOPTS) -o $@ main.o metaparser.o
	$(RM) *.o
clean:
	$(RM) *.o
	$(RM) $(BINDIR)/*
