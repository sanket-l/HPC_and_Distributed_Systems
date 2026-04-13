all:
	gcc -o simulator.bin -lm main.c simulator.c

run: all
	@bash run_simulations.sh

clean:
	rm -f simulator.bin
	rm -rf output
	rm -rf plots
