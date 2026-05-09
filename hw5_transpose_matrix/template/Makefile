NAME = hw5
# Detect OS and set compiler flags accordingly
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CCOPTS = -Wall -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp
    CCLIBS = -lm -L/opt/homebrew/opt/libomp/lib -lomp
else
    # Linux (GCC with native OpenMP)
    CCOPTS = -Wall -fopenmp
    CCLIBS = -lm
endif

SRC = main.c util.c transpose.c
BIN = $(NAME).bin

# Number of repetitions per measurement
REPS    = 5
# Fixed matrix size for threading experiments (Sections 2-4)
N_FIXED = 7000
# Thread counts for Sections 2, 3, 4
THREADS = 1 2 3 4 5 6 8 16 24 32 64 128
# Thread counts for Section 4 speedup plot
SPEEDUP_THREADS = 1 2 3 4 6 8 16 32 64 128
# Physical core count for Section 5 – SET THIS TO YOUR MACHINE'S CORE COUNT
EXP5_THREADS = 4

.PHONY: build clean test \
        exp1 exp2_fine exp3_coarse_n exp3_coarse_div exp4 exp5 \
        experiments

# ── Build ─────────────────────────────────────────────────────────────────────
build: $(BIN)

$(BIN): $(SRC)
	gcc $(CCOPTS) -o $@ $^ $(CCLIBS)

# ── Correctness test ──────────────────────────────────────────────────────────
MAT_SIZE = 50
GRA      = 4
THR      = 4
VERB     = 1
test:
	./$(BIN) $(MAT_SIZE) s $(VERB) > sequential.txt
	./$(BIN) $(MAT_SIZE) p $(GRA) $(THR) $(VERB) > parallel.txt
	diff sequential.txt parallel.txt
	@echo "Test passed: sequential and parallel outputs match."

# ── Experiment 1: Sequential time vs matrix size ──────────────────────────────
EXP1_OUT = results/exp1_sequential.csv
exp1: build
	mkdir -p results
	echo "n,time" > $(EXP1_OUT)
	for n in 100 500 1000 2000 4000 6000 8000 10000; do \
	    printf "$$n," >> $(EXP1_OUT); \
	    python3 run_trial.py $(REPS) $$n s 0 >> $(EXP1_OUT); \
	done
	@echo "Experiment 1 done -> $(EXP1_OUT)"

# ── Experiment 2: Fine-grain (grain=1), threads vary, n=N_FIXED ───────────────
EXP2_FINE_OUT = results/exp2_fine.csv
exp2_fine: build
	mkdir -p results
	echo "threads,time" > $(EXP2_FINE_OUT)
	for t in $(THREADS); do \
	    printf "$$t," >> $(EXP2_FINE_OUT); \
	    python3 run_trial.py $(REPS) $(N_FIXED) p 1 $$t 0 >> $(EXP2_FINE_OUT); \
	done
	@echo "Experiment 2 (fine) done -> $(EXP2_FINE_OUT)"

# ── Experiment 3a: Coarse-grain (grain=n), threads vary, n=N_FIXED ────────────
EXP3_COARSE_N_OUT = results/exp3_coarse_n.csv
exp3_coarse_n: build
	mkdir -p results
	echo "threads,time" > $(EXP3_COARSE_N_OUT)
	for t in $(THREADS); do \
	    printf "$$t," >> $(EXP3_COARSE_N_OUT); \
	    python3 run_trial.py $(REPS) $(N_FIXED) p $(N_FIXED) $$t 0 >> $(EXP3_COARSE_N_OUT); \
	done
	@echo "Experiment 3 (grain=n) done -> $(EXP3_COARSE_N_OUT)"

# ── Experiment 3b: Coarse-grain (grain=total/t), threads vary, n=N_FIXED ──────
EXP3_COARSE_DIV_OUT = results/exp3_coarse_div.csv
exp3_coarse_div: build
	mkdir -p results
	echo "threads,grain,time" > $(EXP3_COARSE_DIV_OUT)
	for t in $(THREADS); do \
	    grain=$$(python3 -c "n=$(N_FIXED); t=$$t; print(max(1, n*(n-1)//2//t))"); \
	    printf "$$t,$$grain," >> $(EXP3_COARSE_DIV_OUT); \
	    python3 run_trial.py $(REPS) $(N_FIXED) p $$grain $$t 0 >> $(EXP3_COARSE_DIV_OUT); \
	done
	@echo "Experiment 3 (grain=total/t) done -> $(EXP3_COARSE_DIV_OUT)"

# ── Experiment 4: Speedup – fine vs coarse, n=N_FIXED ────────────────────────
EXP4_SEQ    = results/exp4_seq.csv
EXP4_FINE   = results/exp4_fine.csv
EXP4_COARSE = results/exp4_coarse.csv
exp4: build
	mkdir -p results
	echo "time" > $(EXP4_SEQ)
	python3 run_trial.py $(REPS) $(N_FIXED) s 0 >> $(EXP4_SEQ)
	echo "threads,time" > $(EXP4_FINE)
	for t in $(SPEEDUP_THREADS); do \
	    printf "$$t," >> $(EXP4_FINE); \
	    python3 run_trial.py $(REPS) $(N_FIXED) p 1 $$t 0 >> $(EXP4_FINE); \
	done
	echo "threads,time" > $(EXP4_COARSE)
	for t in $(SPEEDUP_THREADS); do \
	    printf "$$t," >> $(EXP4_COARSE); \
	    python3 run_trial.py $(REPS) $(N_FIXED) p $(N_FIXED) $$t 0 >> $(EXP4_COARSE); \
	done
	@echo "Experiment 4 done."

# ── Experiment 5: Speedup vs n, best coarse config ────────────────────────────
EXP5_OUT = results/exp5_speedup.csv
exp5: build
	mkdir -p results
	echo "n,seq_time,par_time,speedup" > $(EXP5_OUT)
	for n in 10 50 100 250 500 1000 2000 3000 5000 7000 10000; do \
	    grain=$$(python3 -c "n=$$n; t=$(EXP5_THREADS); print(max(1, n*(n-1)//2//t))"); \
	    seq_t=$$(python3 run_trial.py $(REPS) $$n s 0); \
	    par_t=$$(python3 run_trial.py $(REPS) $$n p $$grain $(EXP5_THREADS) 0); \
	    speedup=$$(python3 -c "print($$seq_t / $$par_t)"); \
	    echo "$$n,$$seq_t,$$par_t,$$speedup" >> $(EXP5_OUT); \
	done
	@echo "Experiment 5 done -> $(EXP5_OUT)"

# ── Run all experiments ───────────────────────────────────────────────────────
experiments: exp1 exp2_fine exp3_coarse_n exp3_coarse_div exp4 exp5
	@echo "All experiments complete. Results in results/"

# ── Clean ─────────────────────────────────────────────────────────────────────
clean:
	rm -rf $(BIN) sequential.txt parallel.txt results/
