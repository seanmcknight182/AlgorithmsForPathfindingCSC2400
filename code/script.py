import os
import re
import matplotlib.pyplot as plt
 
# Subdirectory name -> algorithm display name
RESULT_DIRS = {
    "../results/1Dijkstra_Results":    "Dijkstra",
    "../results/A_Star_Results":       "A*",
    "../results/ACO_Results":          "ACO",
    "../results/Bellman_Ford_Results": "Bellman-Ford",
}
 
# Files named like 0_300.txt inside each subdirectory
TXT_FILE_PATTERN = r"(\d+)_(\d+)\.txt"
 
 
# Parser
def parse_txt(filepath):
    """Parse a two-line TXT file:
        time_ms,basic_op_count,weight
        167.234,400,276
    """
    with open(filepath, "r") as f:
        lines = f.readlines()
 
    if len(lines) < 2:
        raise ValueError(f"Expected at least 2 lines in {filepath}")
 
    values = lines[1].strip().split(",")
    if len(values) < 3:
        raise ValueError(f"Expected 3 comma-separated values on line 2 of {filepath}")
 
    return {
        "time": float(values[0]),
        "ops": int(values[1]),
        "weight": int(float(values[2])),
    }
 
 
# Data collection
 
def collect_data():
    """Scan each results directory for TXT result files."""
    data = {}
 
    for result_dir, algo_name in RESULT_DIRS.items():
        if not os.path.isdir(result_dir):
            print(f"Warning: directory '{result_dir}' not found, skipping.")
            continue
 
        algo_data = []
        for filename in os.listdir(result_dir):
            match = re.match(TXT_FILE_PATTERN, filename)
            if not match:
                continue
 
            num_points = int(match.group(2))
            filepath   = os.path.join(result_dir, filename)
 
            try:
                metrics = parse_txt(filepath)
                algo_data.append((num_points, metrics))
            except Exception as e:
                print(f"Skipping {filepath}: {e}")
 
        algo_data.sort(key=lambda x: x[0])
        data[algo_name] = algo_data
 
    return {algo: pts for algo, pts in data.items() if pts}
 
 
# Plotting
 
def plot_metric(data, metric_key, ylabel, filename):
    plt.figure()
 
    for algo, values in data.items():
        x = [v[0] for v in values]
        y = [v[1][metric_key] for v in values]
        plt.plot(x, y, marker="o", label=algo)
 
    plt.xlabel("Number of Points")
    plt.ylabel(ylabel)
    plt.title(f"{ylabel} vs Number of Points")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(filename)
    plt.close()
    print(f"Saved {filename}")
 
 
# Entry point
 
def main():
    data = collect_data()
 
    if not data:
        print("No data found. Make sure the algorithm subdirectories contain result .txt files.")
        return
 
    os.makedirs("../graphs", exist_ok=True)
 
    plot_metric(data, "time",   "Time (ms)",            "../graphs/time_plot.png")
    plot_metric(data, "ops",    "Basic Operation Count", "../graphs/ops_plot.png")
    plot_metric(data, "weight", "Weight",                "../graphs/weight_plot.png")
 
 
if __name__ == "__main__":
    main()