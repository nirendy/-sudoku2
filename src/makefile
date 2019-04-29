CC = gcc
OBJS = main.o main_aux.o parser.o game_aux.o game.o solver.o file_handler.o linked_list.o lp_solver.o
PROJ_FILES = main.c main.h main_aux.c main_aux.h parser.c parser.h game_aux.c game_aux.h game.c game.h solver.c solver.h file_handler.c file_handler.h linked_list.c linked_list.h lp_solver.c lp_solver.h SPBufferset.h
EXEC = sudoku-console
LOGS = gur.log gur.lp
MAKE_FILE = makefile
ZIP_NAME = 205686397_317624278_finalproject.zip
COMP_FLAG = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c main_aux.h parser.h game_aux.h game.h solver.h file_handler.h linked_list.h lp_solver.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
main_aux.o: main_aux.c main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h
	$(CC) $(COMP_FLAG) -c $*.c
game_aux.o: game_aux.c game_aux.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c solver.h
	$(CC) $(COMP_FLAG) -c $*.c
file_handler.o: file_handler.c file_handler.h
	$(CC) $(COMP_FLAG) -c $*.c
linked_list.o: linked_list.c linked_list.h
	$(CC) $(COMP_FLAG) -c $*.c
lp_solver.o: lp_solver.c lp_solver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
zip:
	zip ${ZIP_NAME} ${PROJ_FILES} ${MAKE_FILE}
clean:
	rm -f $(OBJS) $(EXEC) ${LOGS} ${ZIP_NAME}
