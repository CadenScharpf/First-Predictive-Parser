IDIR=include
ODIR=obj

CC=g++
CFLAGS=-I$(IDIR) -g

_DEPS = inputbuf.h lexer.h parser.h REG.h symbol_table.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = inputbuf.o lexer.o REG.o parser.o symbol_table.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bin/Project2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: all
all:
	bin/Project2

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 