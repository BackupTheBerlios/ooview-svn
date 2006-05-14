SOURCEFILES = src/$(wildcard *.c) src/$(wildcard *.h)
BINDIR=bin

GCC = /usr/bin/gcc
GCCOPTS = -Werror -lmenu -lncurses
GCCNOLINK = -c
RM = rm -f

bin/ooview: $(SOURCEFILES)
	$(GCC) $(GCCNOLINK) src/main.c
	$(GCC) $(GCCNOLINK) src/menu.c
	$(GCC) $(GCCNOLINK) src/boxes.c
	$(GCC) $(GCCNOLINK) src/cmd.c
	$(GCC) $(GCCNOLINK) src/layout.c
	$(GCC) $(GCCNOLINK) src/widget.c
	$(GCC) $(GCCNOLINK) src/dialog.c
	$(GCC) $(GCCOPTS) -o $@ main.o menu.o boxes.o cmd.o layout.o widget.o dialog.o
	$(RM) *.o

clean:
	$(RM) $(BINDIR)/*
