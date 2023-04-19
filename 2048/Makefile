COMPILER = gcc
OPTION = -lncurses
PROGRAM = 2048.out

$(PROGRAM): 2048prog.c
	$(COMPILER) 2048prog.c -o $(PROGRAM) $(OPTION)

all: $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM)

clean: 
	rm *.out
