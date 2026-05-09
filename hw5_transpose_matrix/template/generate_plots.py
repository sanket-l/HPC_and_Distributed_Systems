#!/usr/bin/env python3
"""
generate_plots.py
=================
Reads experiment CSVs from the `results/` directory and saves publication-ready
figures to `figures/`. Run after `make experiments`.

Usage:
    python3 generate_plots.py
"""

import os, sys
import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

RESULTS_DIR = "results"
FIGURES_DIR = "figures"
os.makedirs(FIGURES_DIR, exist_ok=True)

# ── Shared style ───────────────────────────────────────────────────────────────
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 11,
    "axes.titlesize": 12,
    "axes.labelsize": 11,
    "legend.fontsize": 10,
    "figure.dpi": 150,
    "axes.grid": True,
    "grid.linestyle": "--",
    "grid.alpha": 0.5,
})

def read_csv(filename):
    path = os.path.join(RESULTS_DIR, filename)
    if not os.path.exists(path):
        print(f"[WARN] {path} not found – skipping.")
        return None
    with open(path) as f:
        reader = csv.DictReader(f)
        rows = list(reader)
    return rows

def col(rows, key):
    return [float(r[key]) for r in rows]

# ── Figure 1: Sequential time vs matrix size ───────────────────────────────────
def plot_exp1():
    rows = read_csv("exp1_sequential.csv")
    if rows is None:
        return
    ns = col(rows, "n")
    ts = col(rows, "time")

    fig, axes = plt.subplots(1, 2, figsize=(10, 4))

    # Left: linear scale
    axes[0].plot(ns, ts, "o-", color="steelblue", linewidth=1.8, markersize=5)
    axes[0].set_xlabel("Matrix size n")
    axes[0].set_ylabel("Execution time (s)")
    axes[0].set_title("Sequential transposition time vs. n")

    # Right: log-log
    axes[1].loglog(ns, ts, "o-", color="steelblue", linewidth=1.8, markersize=5)
    # Overlay n^2 reference line
    ns_arr = np.array(ns)
    ref = ns_arr**2 / ns_arr[-1]**2 * ts[-1]
    axes[1].loglog(ns_arr, ref, "k--", linewidth=1, label=r"$O(n^2)$ reference")
    axes[1].set_xlabel("Matrix size n (log scale)")
    axes[1].set_ylabel("Execution time (s, log scale)")
    axes[1].set_title("Log-log: time vs. n")
    axes[1].legend()

    fig.tight_layout()
    out = os.path.join(FIGURES_DIR, "fig1_seq_time_vs_n.png")
    fig.savefig(out, bbox_inches="tight")
    print(f"Saved {out}")
    plt.close(fig)

# ── Figure 2: Fine-grain execution time vs threads (n=7000, grain=1) ──────────
def plot_exp2():
    rows = read_csv("exp2_fine.csv")
    if rows is None:
        return
    ts   = col(rows, "threads")
    time = col(rows, "time")

    fig, ax = plt.subplots(figsize=(6, 4))
    ax.semilogx(ts, time, "s-", color="firebrick", linewidth=1.8, markersize=5,
                base=2)
    ax.set_xticks(ts)
    ax.get_xaxis().set_major_formatter(ticker.ScalarFormatter())
    ax.set_xlabel("Number of threads")
    ax.set_ylabel("Execution time (s)")
    ax.set_title("Fine-grain (grain=1) execution time vs. threads\n(n = 7,000)")
    fig.tight_layout()
    out = os.path.join(FIGURES_DIR, "fig2_fine_time_vs_threads.png")
    fig.savefig(out, bbox_inches="tight")
    print(f"Saved {out}")
    plt.close(fig)

# ── Figure 3: Coarse-grain execution time vs threads ─────────────────────────
def plot_exp3():
    rows_n   = read_csv("exp3_coarse_n.csv")
    rows_div = read_csv("exp3_coarse_div.csv")

    fig, ax = plt.subplots(figsize=(6, 4))

    if rows_n:
        ts_n   = col(rows_n, "threads")
        time_n = col(rows_n, "time")
        ax.semilogx(ts_n, time_n, "o-", color="darkorange", linewidth=1.8,
                    markersize=5, label=r"grain $= n$", base=2)

    if rows_div:
        ts_d   = col(rows_div, "threads")
        time_d = col(rows_div, "time")
        ax.semilogx(ts_d, time_d, "^--", color="green", linewidth=1.8,
                    markersize=5, label=r"grain $= n(n-1)/(2t)$", base=2)

    if rows_n or rows_div:
        ref_ts = ts_n if rows_n else ts_d
        ax.set_xticks(ref_ts)
        ax.get_xaxis().set_major_formatter(ticker.ScalarFormatter())

    ax.set_xlabel("Number of threads")
    ax.set_ylabel("Execution time (s)")
    ax.set_title("Coarse-grain execution time vs. threads\n(n = 7,000)")
    ax.legend()
    fig.tight_layout()
    out = os.path.join(FIGURES_DIR, "fig3_coarse_time_vs_threads.png")
    fig.savefig(out, bbox_inches="tight")
    print(f"Saved {out}")
    plt.close(fig)

# ── Figure 4: Speedup – fine vs coarse, n=7000 ───────────────────────────────
def plot_exp4():
    rows_seq    = read_csv("exp4_seq.csv")
    rows_fine   = read_csv("exp4_fine.csv")
    rows_coarse = read_csv("exp4_coarse.csv")
    if rows_seq is None:
        return

    t_seq = float(rows_seq[0]["time"])

    fig, ax = plt.subplots(figsize=(6, 4))

    if rows_fine:
        ts_f = col(rows_fine, "threads")
        sp_f = [t_seq / t for t in col(rows_fine, "time")]
        ax.semilogx(ts_f, sp_f, "s-", color="firebrick", linewidth=1.8,
                    markersize=5, label="Fine grain (grain=1)", base=2)

    if rows_coarse:
        ts_c = col(rows_coarse, "threads")
        sp_c = [t_seq / t for t in col(rows_coarse, "time")]
        ax.semilogx(ts_c, sp_c, "o--", color="darkorange", linewidth=1.8,
                    markersize=5, label=r"Coarse grain (grain=$n$)", base=2)

    # Ideal speedup reference
    ref_ts = ts_f if rows_fine else ts_c
    ax.semilogx(ref_ts, ref_ts, "k:", linewidth=1, label="Ideal speedup", base=2)

    ax.set_xticks(ref_ts)
    ax.get_xaxis().set_major_formatter(ticker.ScalarFormatter())
    ax.set_xlabel("Number of threads")
    ax.set_ylabel("Speedup over sequential")
    ax.set_title("Speedup: fine-grain vs. coarse-grain\n(n = 7,000)")
    ax.legend()
    ax.set_ylim(0, 10)   # ← add this
    fig.tight_layout()
    out = os.path.join(FIGURES_DIR, "fig4_speedup_fine_vs_coarse.png")
    fig.savefig(out, bbox_inches="tight")
    print(f"Saved {out}")
    plt.close(fig)

# ── Figure 5: Speedup of best coarse-grain vs sequential, varying n ───────────
def plot_exp5():
    rows = read_csv("exp5_speedup.csv")
    if rows is None:
        return
    ns      = col(rows, "n")
    speedup = col(rows, "speedup")

    fig, ax = plt.subplots(figsize=(6, 4))
    ax.semilogx(ns, speedup, "D-", color="steelblue", linewidth=1.8, markersize=5)
    ax.axhline(1.0, color="gray", linestyle="--", linewidth=1, label="Speedup = 1")
    ax.set_xlabel("Matrix size n (log scale)")
    ax.set_ylabel("Speedup over sequential")
    ax.set_title("Best coarse-grain speedup vs. matrix size")
    ax.legend()
    fig.tight_layout()
    out = os.path.join(FIGURES_DIR, "fig5_speedup_vs_n.png")
    fig.savefig(out, bbox_inches="tight")
    print(f"Saved {out}")
    plt.close(fig)


if __name__ == "__main__":
    missing_deps = []
    try:
        import matplotlib
    except ImportError:
        missing_deps.append("matplotlib")
    try:
        import numpy
    except ImportError:
        missing_deps.append("numpy")

    if missing_deps:
        print(f"[ERROR] Missing packages: {', '.join(missing_deps)}")
        print("Install with: pip3 install " + " ".join(missing_deps))
        sys.exit(1)

    print("Generating figures from experiment results...")
    plot_exp1()
    plot_exp2()
    plot_exp3()
    plot_exp4()
    plot_exp5()
    print(f"\nAll figures saved to ./{FIGURES_DIR}/")
    print("Include them in your report with \\includegraphics{figures/<name>.png}")
