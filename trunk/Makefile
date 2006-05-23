SOURCEFILES = src/$(wildcard *.c) src/$(wildcard *.h)
BINDIR=bin

GCC = /usr/bin/gcc
GCCOPTS = -lmenu -lncurses
GCCNOLINK = -ansi -Werror -c
RM = rm -f

bin/ooview: $(SOURCEFILES)
	$(GCC) $(GCCOPTS) -o $@ src/main.c 

clean:
	$(RM) *.o
	$(RM) $(BINDIR)/*
