import os
import pandas as pd
import matplotlib.pyplot as plt

RESULTS_DIR = "results"

STRONG_CSV = os.path.join(RESULTS_DIR, "strong_scalability.csv")
WEAK_CSV = os.path.join(RESULTS_DIR, "weak_scalability.csv")


def load_csv(path):

    if not os.path.exists(path):
        print(f"[ERREUR] {path} introuvable.")
        return None

    return pd.read_csv(path, skipinitialspace=True)


def execution_plot(df, title, output):

    plt.figure(figsize=(9,6))

    plt.plot(
        df["Threads"],
        df["SequentialTime"],
        marker="o",
        linewidth=2,
        linestyle="--",
        label="Séquentiel"
    )

    plt.plot(
        df["Threads"],
        df["ParallelTime"],
        marker="s",
        linewidth=2,
        label="Parallèle"
    )

    plt.title(title)

    plt.xlabel("Nombre de Threads")

    plt.ylabel("Temps (secondes)")

    plt.grid(True)

    plt.legend()

    for x, y in zip(df["Threads"], df["ParallelTime"]):
        plt.annotate(f"{y:.3f}",
                     (x,y),
                     textcoords="offset points",
                     xytext=(0,8),
                     ha="center")

    plt.savefig(output, dpi=300)

    plt.close()

    print(f"[OK] {output}")


def speedup_plot(df, title, output):

    plt.figure(figsize=(9,6))

    plt.plot(
        df["Threads"],
        df["Threads"],
        "--",
        linewidth=2,
        label="Idéal"
    )

    plt.plot(
        df["Threads"],
        df["Speedup"],
        marker="o",
        linewidth=2,
        label="Observé"
    )

    plt.fill_between(
        df["Threads"],
        df["Threads"],
        df["Speedup"],
        where=(df["Speedup"] > df["Threads"]),
        alpha=0.20,
        label="Super-linéaire"
    )

    plt.title(title)

    plt.xlabel("Nombre de Threads")

    plt.ylabel("Speedup")

    plt.grid(True)

    plt.legend()

    for x,y in zip(df["Threads"],df["Speedup"]):

        plt.annotate(f"{y:.2f}",
                     (x,y),
                     textcoords="offset points",
                     xytext=(0,8),
                     ha="center")

    plt.savefig(output,dpi=300)

    plt.close()

    print(f"[OK] {output}")


def efficiency_plot(df,title,output):

    plt.figure(figsize=(9,6))

    plt.plot(
        df["Threads"],
        df["Efficiency"],
        marker="o",
        linewidth=2
    )

    plt.axhline(
        1,
        linestyle="--",
        linewidth=1
    )

    plt.title(title)

    plt.xlabel("Nombre de Threads")

    plt.ylabel("Efficacité")

    plt.grid(True)

    for x,y in zip(df["Threads"],df["Efficiency"]):

        plt.annotate(f"{y:.2f}",
                     (x,y),
                     textcoords="offset points",
                     xytext=(0,8),
                     ha="center")

    plt.savefig(output,dpi=300)

    plt.close()

    print(f"[OK] {output}")


def strong_scalability():

    df = load_csv(STRONG_CSV)

    if df is None:
        return

    execution_plot(
        df,
        "Temps d'exécution - Scalabilité Forte",
        os.path.join(
            RESULTS_DIR,
            "execution_time_strong.png"
        )
    )

    speedup_plot(
        df,
        "Speedup - Scalabilité Forte",
        os.path.join(
            RESULTS_DIR,
            "speedup_strong.png"
        )
    )

    efficiency_plot(
        df,
        "Efficacité - Scalabilité Forte",
        os.path.join(
            RESULTS_DIR,
            "efficiency_strong.png"
        )
    )


def weak_scalability():

    df = load_csv(WEAK_CSV)

    if df is None:
        return

    execution_plot(
        df,
        "Temps d'exécution - Scalabilité Faible",
        os.path.join(
            RESULTS_DIR,
            "execution_time_weak.png"
        )
    )

    speedup_plot(
        df,
        "Speedup - Scalabilité Faible",
        os.path.join(
            RESULTS_DIR,
            "speedup_weak.png"
        )
    )

    efficiency_plot(
        df,
        "Efficacité - Scalabilité Faible",
        os.path.join(
            RESULTS_DIR,
            "efficiency_weak.png"
        )
    )


def main():

    os.makedirs(RESULTS_DIR,exist_ok=True)

    print()

    print("========================================")

    print(" Génération automatique des graphiques")

    print("========================================")

    print()

    strong_scalability()

    weak_scalability()

    print()

    print("Tous les graphiques ont été générés.")

    print()


if __name__ == "__main__":
    main()