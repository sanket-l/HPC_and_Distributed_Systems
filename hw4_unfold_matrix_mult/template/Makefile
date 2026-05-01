NAME = hw4
#CCOPTS = -Wall -pthread
CCOPTS = -Wall -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp
CCLIBS = -lm -L/opt/homebrew/opt/libomp/lib -lomp
SRC = main.c util.c multiply.c
BIN = $(NAME).bin


.PHONY: build clean test
build: $(BIN)
$(BIN): $(SRC)
	gcc $(CCOPTS) -o $@ $^ $(CCLIBS) 


clean:
	rm -rf $(BIN) sequential.txt parallel.txt


MAT_SIZE = 16
VERB = 1
test: 
	./$(BIN) $(MAT_SIZE) 1 $(VERB)  > sequential.txt
	for THR in 2 4 8 16; do \
		./$(BIN) $(MAT_SIZE) $$THR $(VERB)  > parallel_$$THR.txt; \
	done
	
# 	diff sequential.txt parallel.txt