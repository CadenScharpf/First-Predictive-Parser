IDIR=include
ODIR=onj

CC=gcc
CFLAGS= -std=c++2a -I$(IDIR) 

_DEPS = inputbuf.h lexer.h parser.h REG.h symbol_table.h unixcolors.h my_LexicalAnalyzer.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = inputbuf.o lexer.o REG.o parser.o symbol_table.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bin/Parser: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: all
all:
	bin/parser

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 