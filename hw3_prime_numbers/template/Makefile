NAME = hw3
#CCOPTS = -Wall -pthread
CCOPTS = -Wall -fopenmp
CCLIBS = -lm
SRC = main.c util.c primes.c
BIN = $(NAME).bin


.PHONY: build clean test
build: $(BIN)
$(BIN): $(SRC)
	gcc $(CCOPTS) -o $@ $^ $(CCLIBS) 


clean:
	rm -rf $(BIN) sequential.txt parallel.txt


MAX = 10000
THR = 4
VERB = 1
test: 
	./$(BIN) $(MAX) s 1 $(VERB) | head -n -1 | sort -n > sequential.txt
	./$(BIN) $(MAX) p $(THR) $(VERB) | head -n -1 | sort -n > parallel.txt
	diff sequential.txt parallel.txt
