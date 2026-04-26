import os
import re
import matplotlib.pyplot as plt

# Map algorithm IDs to names
ALGO_MAP = {
    0: "ACO",
    1: "A*",
    2: "Dijkstra",
    3: "Bellman-Ford"
}

# New filename pattern: 0_100.txt, 1_250.txt, etc.
FILE_PATTERN = r"(\d+)_(\d+)\.txt"


def parse_metrics(filepath):
    with open(filepath, "r") as f:
        lines = f.readlines()

        # Expect:
        # time_ms,basic_op_count,weight
        # 167,400,276
        if len(lines) < 2:
            raise ValueError(f"Invalid file format: {filepath}")

        values = lines[1].strip().split(",")

        return {
            "time": int(values[0]),
            "ops": int(values[1]),
            "weight": int(values[2])
        }


def collect_data():
    data = {name: [] for name in ALGO_MAP.values()}

    for file in os.listdir():
        match = re.match(FILE_PATTERN, file)
        if match:
            algo_id = int(match.group(1))
            num_points = int(match.group(2))

            if algo_id not in ALGO_MAP:
                continue

            algo_name = ALGO_MAP[algo_id]
            filepath = os.path.join(file)

            try:
                metrics = parse_metrics(filepath)
                data[algo_name].append((num_points, metrics))
            except Exception as e:
                print(f"Skipping {file}: {e}")

    # Sort each algorithm's data by number of points
    for algo in data:
        data[algo].sort(key=lambda x: x[0])

    return data


def plot_metric(data, metric_key, ylabel, filename):
    plt.figure()

    for algo, values in data.items():
        if not values:
            continue

        x = [v[0] for v in values]
        y = [v[1][metric_key] for v in values]

        plt.plot(x, y, marker='o', label=algo)

    plt.xlabel("Number of Points")
    plt.ylabel(ylabel)
    plt.title(f"{ylabel} vs Number of Points")
    plt.legend()
    plt.grid(True)

    plt.savefig(filename)
    plt.close()


def main():
    data = collect_data()

    # Check if all are empty
    if all(len(v) == 0 for v in data.values()):
        print("No data found.")
        return

    plot_metric(data, "time", "Time (ms)", "time_plot.png")
    plot_metric(data, "ops", "Basic Operation Count", "ops_plot.png")
    plot_metric(data, "weight", "Weight", "weight_plot.png")

    print("Graphs saved: time_plot.png, ops_plot.png, weight_plot.png")


if __name__ == "__main__":
    main()import os
import re
import csv
import matplotlib.pyplot as plt

ALGO_DIRS = [
    "ACO_Algorithm",
    "A_Star_Algorithm",
    "Algorithm1Dijkstra",
    "Bellman_Ford_Algorithm"
]

# Example filenames: results_50.csv, results_100.csv
FILE_PATTERN = r"results_(\d+)\.csv"


def parse_metrics(filepath):
    with open(filepath, "r") as f:
        reader = csv.DictReader(f)
        row = next(reader)  # Only one row expected

        return {
            "time": int(row["time_ms"]),
            "ops": int(row["basic_op_count"]),
            "weight": int(row["weight"])
        }


def collect_data():
    data = {}

    for algo in ALGO_DIRS:
        algo_data = []
        if not os.path.exists(algo):
            continue

        for file in os.listdir(algo):
            match = re.match(FILE_PATTERN, file)
            if match:
                num_points = int(match.group(1))
                filepath = os.path.join(algo, file)

                metrics = parse_metrics(filepath)
                algo_data.append((num_points, metrics))

        # Sort by number of points
        algo_data.sort(key=lambda x: x[0])
        data[algo] = algo_data

    return data


def plot_metric(data, metric_key, ylabel, filename):
    plt.figure()

    for algo, values in data.items():
        x = [v[0] for v in values]
        y = [v[1][metric_key] for v in values]

        plt.plot(x, y, marker='o', label=algo)

    plt.xlabel("Number of Points")
    plt.ylabel(ylabel)
    plt.title(f"{ylabel} vs Number of Points")
    plt.legend()
    plt.grid(True)

    plt.savefig(filename)
    plt.close()


def main():
    data = collect_data()

    if not data:
        print("No data found.")
        return

    plot_metric(data, "time", "Time (ms)", "time_plot.png")
    plot_metric(data, "ops", "Basic Operation Count", "ops_plot.png")
    plot_metric(data, "weight", "Weight", "weight_plot.png")

    print("Graphs saved: time_plot.png, ops_plot.png, weight_plot.png")


if __name__ == "__main__":
    main()
