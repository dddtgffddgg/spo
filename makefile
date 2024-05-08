CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O3
OMP_FLAGS=-fopenmp

EXEC_1 = program_1
SRC_1 = $(EXEC_1).c

EXEC_2 = program_2
SRC_2 = $(EXEC_2).c

EXEC_3 = program_3
SRC_3 = $(EXEC_3).c

.PHONY: all clean

all: $(EXEC_1) $(EXEC_2) $(EXEC_3)
	@echo "Running $(EXEC_1)"
	time ./$(EXEC_1)
	@echo "Running $(EXEC_2)"
	time ./$(EXEC_2)
	@echo "Running $(EXEC_3)"
	time ./$(EXEC_3)

$(EXEC_1): $(SRC_1)
	$(CC) $(CFLAGS) -o $@ $< -lm

$(EXEC_2): $(SRC_2)
	$(CC) $(CFLAGS) -o $@ $< -pthread

$(EXEC_3): $(SRC_3)
	$(CC) $(CFLAGS) $(OMP_FLAGS) -o $@ $< -lm

clean:
	rm -f $(EXEC_1) $(EXEC_2) $(EXEC_3)
