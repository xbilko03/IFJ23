CC=gcc
CFLAGS= -Wall 
DEPS = main.c lex.c syn.c sem.c TRP.c codegen.c functions.h functions.c error.c
PROGNAME = GIGACHAD

$(PROGNAME): $(DEPS)
	$(CC) $(DEPS) $(CFLAGS) -o $(PROGNAME)

run: $(DEPS)
	$(CC) $(DEPS) $(CFLAGS) -o $(PROGNAME)
	./$(PROGNAME) <tests/example1.txt

run2: $(DEPS)
	$(CC) $(DEPS) $(CFLAGS) -o $(PROGNAME)
	./$(PROGNAME) <tests/example2.txt

debug: $(DEPS)
	 $(CC) $(DEPS) $(CFLAGS) -g -o $(PROGNAME)

clean:
	rm $(PROGNAME)
