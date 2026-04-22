NAME = hw3
#CCOPTS = -Wall -pthread
CCOPTS = -Wall -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp
CCLIBS = -lm -L/opt/homebrew/opt/libomp/lib -lomp
SRC = main.c util.c primes.c
BIN = $(NAME).bin


.PHONY: build clean test
build: $(BIN)
$(BIN): $(SRC)
	gcc $(CCOPTS) -o $@ $^ $(CCLIBS) 


clean:
	rm -rf $(BIN)

MAX = 10000000
THR = 1
VERB = 1
test: 
	@echo "Running sequential version..."
	@./$(BIN) $(MAX) s 1 $(VERB) > /dev/null 2>&1; \
	SEQ_TIME=$$(./$(BIN) $(MAX) s 1 $(VERB) 2>/dev/null | tail -1); \
	echo "Sequential time: $$SEQ_TIME seconds"

	@echo "Running parallel versions with different thread counts..."
	@for thr in 1 2 4 8 16 32 64 128; do \
		echo "Testing with $$thr threads... "; \
		PAR_TIME=$$(./$(BIN) $(MAX) p $$thr $(VERB) 2>/dev/null | tail -1); \
		echo "Time: $$PAR_TIME seconds"; \
	done
