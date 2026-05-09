#!/usr/bin/env python3
"""
run_trial.py  –  run hw5.bin and print the average time over REPS runs.

Usage:
    python3 run_trial.py <reps> <arg1> <arg2> ...

The script forwards all arguments after <reps> directly to ./hw5.bin,
runs it <reps> times, and prints the average wall-clock time to stdout.

Examples:
    python3 run_trial.py 5 10000 s 0
    python3 run_trial.py 5 7000 p 1 4 0
"""

import subprocess
import sys

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <reps> <hw5.bin args...>", file=sys.stderr)
        sys.exit(1)

    reps = int(sys.argv[1])
    cmd  = ["./hw5.bin"] + sys.argv[2:]

    times = []
    for _ in range(reps):
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"[ERROR] Command failed: {' '.join(cmd)}", file=sys.stderr)
            print(result.stderr, file=sys.stderr)
            sys.exit(1)
        # last line of stdout is the timing value
        last_line = result.stdout.strip().split("\n")[-1]
        times.append(float(last_line))

    print(sum(times) / len(times))

if __name__ == "__main__":
    main()