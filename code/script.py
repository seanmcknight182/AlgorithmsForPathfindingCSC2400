import os
import re

ALGO_DIRS = [
    "ACO_Algorithm",
    "A_Star_Algorithm",
    "Algorithm1Dijkstra",
    "Bellman_Ford_Algorithm"
]

RESULT_FILE = "results.txt"


def parse_results(filepath):
    if not os.path.exists(filepath):
        print(f"Missing: {filepath}")
        return None

    with open(filepath, "r") as f:
        content = f.read()

    # Extract path line
    path_match = re.search(r"Path:\s*([\d\s]+)", content)
    path = None
    if path_match:
        path = list(map(int, path_match.group(1).strip().split()))

    # Extract total cost
    cost_match = re.search(r"Total cost:\s*(\d+)", content)
    cost = int(cost_match.group(1)) if cost_match else None

    return {
        "path": path,
        "cost": cost
    }


def main():
    results = {}

    for algo in ALGO_DIRS:
        path = os.path.join(algo, RESULT_FILE)
        parsed = parse_results(path)

        if parsed:
            results[algo] = parsed

    print("\n=== Algorithm Comparison ===\n")
    print(f"{'Algorithm':25} {'Cost':10} {'Path Length':12}")
    print("-" * 55)

    for algo, data in results.items():
        cost = data["cost"] if data["cost"] is not None else "N/A"
        length = len(data["path"]) if data["path"] else "N/A"

        print(f"{algo:25} {cost:<10} {length:<12}")

    # Rank by cost (lower is better)
    valid = [(a, d["cost"]) for a, d in results.items() if d["cost"] is not None]

    if valid:
        sorted_algos = sorted(valid, key=lambda x: x[1])

        print("\n=== Best to Worst (by cost) ===")
        for i, (algo, cost) in enumerate(sorted_algos, 1):
            print(f"{i}. {algo} ({cost})")

        print(f"\nBest algorithm: {sorted_algos[0][0]}")

    else:
        print("\nNo valid cost data found.")


if __name__ == "__main__":
    main()
