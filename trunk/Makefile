SOURCEDIR = src
BINDIR = bin
SRC = ooview.c

GCC = /usr/bin/gcc
GCCOPTS = -Werror -lncurses -ansi
RM = rm -f


ooview: gui

gui: $(SOURCEDIR)/$(SRC)
		$(GCC) $(GCCOPTS) -o $(BINDIR)/ooview $+

clean:
	$(RM) $(BINDIR)/*
