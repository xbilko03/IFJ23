CC=gcc
CFLAGS=-w -Werror -Wpedantic
DEPS = main.c lex.c sem.c TRP.c codegen.c
PROGNAME = GIGACHAD

$(PROGNAME): $(DEPS)
	$(CC) $(DEPS) $(CFLAGS) -o $(PROGNAME)

run: $(DEPS)
	$(CC) $(DEPS) $(CFLAGS) -o $(PROGNAME)
	./$(PROGNAME)

clean:
	rm $(PROGNAME)
