#!/bin/bash

# Create output directory
mkdir -p output

echo "Running simulator with all parameter combinations..."

PROCESSORS=(2 4 8 16 32 64)
DISTRIBUTIONS=(n u)

for procs in "${PROCESSORS[@]}"; do
    for dist in "${DISTRIBUTIONS[@]}"; do
        echo "Running: processors=$procs, distribution=$dist"
        ./simulator.bin "$procs" "$dist" > "output/result_${procs}_${dist}.txt"
        if [ $? -ne 0 ]; then
            echo "Error running with procs=$procs, dist=$dist"
            exit 1
        fi
    done
done

echo "All simulations complete. Generated output files:"
ls -lh output/result_*.txt

echo ""
echo "Running plot generation..."
../.venv/bin/python3 plot_results.py
