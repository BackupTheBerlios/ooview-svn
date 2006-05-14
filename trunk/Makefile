SOURCEFILES = src/$(wildcard *.c) src/$(wildcard *.h)
BINDIR=bin

GCC = /usr/bin/gcc
GCCOPTS = -Werror -lmenu -lncurses
GCCNOLINK = -ansi -Werror -c
RM = rm -f

bin/ooview: $(SOURCEFILES)
	$(GCC) $(GCCNOLINK) src/dialog.c
	$(GCC) $(GCCNOLINK) src/widget.c
	$(GCC) $(GCCNOLINK) src/layout.c
	$(GCC) $(GCCNOLINK) src/main.c
	$(GCC) $(GCCNOLINK) src/boxes.c
	$(GCC) $(GCCNOLINK) src/cmd.c
	$(GCC) $(GCCNOLINK) src/menu.c	
	$(GCC) $(GCCOPTS) -o $@ main.o menu.o boxes.o cmd.o layout.o dialog.o widget.o 
	$(RM) *.o

clean:
	$(RM) $(BINDIR)/*
