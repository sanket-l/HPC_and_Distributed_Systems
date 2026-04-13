#!/usr/bin/env python3
import matplotlib.pyplot as plt
import os
from pathlib import Path

# Configuration
PROCESSORS = [2, 4, 8, 16, 32, 64]
DISTRIBUTIONS = ['n', 'u']
OUTPUT_DIR = 'output'
RESULT_PREFIX = 'result_'
PLOT_DIR = 'plots'

# Create plots directory if it doesn't exist
os.makedirs(PLOT_DIR, exist_ok=True)

# Distribution labels
dist_labels = {
    'n': 'Normal',
    'u': 'Uniform'
}

# Process each distribution
for dist in DISTRIBUTIONS:
    print(f"Processing {dist_labels[dist]} distribution...")
    
    fig, ax = plt.subplots(figsize=(12, 7))
    
    # Plot results for each processor count
    for procs in PROCESSORS:
        result_file = os.path.join(OUTPUT_DIR, f'{RESULT_PREFIX}{procs}_{dist}.txt')
        
        if os.path.exists(result_file):
            # Read the data
            with open(result_file, 'r') as f:
                data = [float(line.strip()) for line in f.readlines()]
            
            # X-axis is memory module index (0 to len(data)-1)
            x = list(range(len(data)))
            
            # Plot
            ax.plot(x, data, label=f'{procs} processors', linewidth=2, alpha=0.8)
            print(f"  Plotted: {procs} processors")
        else:
            print(f"  Warning: File not found: {result_file}")
    
    # Configure plot
    ax.set_xlabel('Memory Module Index', fontsize=12)
    ax.set_ylabel('Average Wait Time', fontsize=12)
    ax.set_title(f'Average Wait Time vs Memory Modules ({dist_labels[dist]} Distribution)', 
                 fontsize=14, fontweight='bold')
    ax.legend(fontsize=10, loc='best')
    ax.grid(True, alpha=0.3)
    
    # Save plot
    plot_file = os.path.join(PLOT_DIR, f'plot_{dist_labels[dist].lower()}.png')
    plt.savefig(plot_file, dpi=300, bbox_inches='tight')
    print(f"  Saved plot to: {plot_file}")
    
    plt.close()

print("\nPlot generation complete!")
print(f"Plots saved in '{PLOT_DIR}/' directory")
