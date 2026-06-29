# Projet OpenMP : Parallélisation et Optimisation du Produit Matriciel

## Architecture Parallèle – Projet de Fin de Session

Ce projet a été réalisé individuellement dans le cadre du cours **Architecture Parallèle** du programme de **Master en Ingénierie des Données** de l'Université d'État d'Haïti (UEH).

L'objectif est d'étudier les performances du calcul matriciel carré

[
C = A \times B
]

en comparant trois implémentations :

* une version séquentielle servant de référence ;
* une version parallèle utilisant **OpenMP** ;
* une version parallèle optimisée utilisant la technique du **Blocking (Tiling)** afin d'améliorer l'utilisation du cache processeur.

Le projet comprend également une étude de **scalabilité forte**, une analyse des performances et une comparaison avec la **loi d'Amdahl**.

---

# Structure du projet

## 📑 Structure du Projet

Le projet adopte une architecture modulaire afin de séparer les algorithmes de calcul, les outils de mesure de performances et les scripts d'analyse. Cette organisation facilite la maintenance, les tests et l'extension du projet.

```text
Projet_OpenMP/
│
├── include/
│   └── matrix.h                     # Déclarations des fonctions, constantes et prototypes
│
├── src/
│   ├── matrix.c                     # Gestion des matrices (allocation, initialisation, comparaison, temps)
│   ├── sequential.c                 # Produit matriciel séquentiel de référence
│   ├── openmp_naive.c               # Version parallèle OpenMP naïve
│   ├── openmp_block.c               # Version parallèle optimisée par blocs (Blocking/Tiling)
│   ├── benchmark.c                  # Mesures de performances, Speedup, Efficacité et export CSV
│   └── main.c                       # Point d'entrée du programme
│
├── results/
│   ├── strong_scalability.csv       # Données expérimentales - Scalabilité forte
│   ├── weak_scalability.csv         # Données expérimentales - Scalabilité faible
│   ├── execution_time_strong.png    # Temps d'exécution - Scalabilité forte
│   ├── speedup_strong.png           # Courbe de Speedup - Scalabilité forte
│   ├── execution_time_weak.png      # Temps d'exécution - Scalabilité faible
│   ├── speedup_weak.png             # Courbe de Speedup - Scalabilité faible
│   └── efficiency_weak.png          # Courbe d'efficacité - Scalabilité faible
│
├── generate_plots.py                # Génération automatique des graphiques à partir des fichiers CSV
│
├── Makefile                         # Compilation automatique avec OpenMP (-O3 -fopenmp)
│
└── README.md                        # Documentation complète du projet
```

---

## 📊 Génération automatique des graphiques

Le script **`generate_plots.py`** automatise l'analyse des performances du projet.

Il lit automatiquement les fichiers :

* `results/strong_scalability.csv`
* `results/weak_scalability.csv`

puis génère les graphiques suivants :

### Scalabilité forte

* Temps d'exécution en fonction du nombre de threads.
* Courbe de Speedup.
* Comparaison avec le Speedup théorique.
* Mise en évidence d'une éventuelle zone de Speedup super-linéaire.

### Scalabilité faible

* Temps d'exécution en fonction du nombre de threads.
* Courbe de Speedup.
* Courbe d'efficacité parallèle.
* Analyse de la stabilité du temps d'exécution lorsque la charge par thread reste constante.

Les graphiques sont automatiquement enregistrés dans le dossier **`results/`** au format PNG avec une résolution adaptée à une intégration dans le rapport final.

---

## 📈 Résultats expérimentaux

À l'issue des expérimentations, le dossier `results/` contiendra :

* les données brutes des expériences (CSV) ;
* les graphiques de performances ;
* les courbes de Speedup ;
* les courbes d'efficacité ;
* les figures utilisées dans le rapport d'analyse.

Cette organisation facilite l'importation des résultats dans Microsoft Excel, LibreOffice Calc ou tout autre outil d'analyse scientifique.


---

# Description des modules

## include/matrix.h

Déclarations des fonctions utilisées dans tout le projet :

* allocation des matrices ;
* libération de la mémoire ;
* génération de matrices ;
* comparaison des résultats ;
* chronométrage ;
* prototypes des différentes implémentations.

---

## src/matrix.c

Fonctions utilitaires :

* allocation dynamique des matrices ;
* libération mémoire ;
* initialisation aléatoire ;
* initialisation à zéro ;
* affichage des matrices ;
* comparaison numérique ;
* mesure du temps d'exécution.

---

## src/sequential.c

Implémentation du produit matriciel séquentiel classique.

Cette version sert de référence pour :

* vérifier l'exactitude des versions parallèles ;
* calculer le Speedup ;
* mesurer les gains de performances.

---

## src/openmp_naive.c

Implémentation parallèle utilisant OpenMP.

Caractéristiques :

* distribution des lignes de la matrice résultat ;
* directive `#pragma omp parallel for`;
* politique d'ordonnancement `schedule(static)` ;
* version directement comparable à l'algorithme séquentiel.

---

## src/openmp_block.c

Version optimisée utilisant la technique du **Blocking (Tiling)**.

Objectifs :

* améliorer la localité mémoire ;
* réduire les défauts de cache (cache misses) ;
* améliorer les performances pour les grandes matrices.

---

## src/benchmark.c

Gestion des performances :

* chronométrage ;
* calcul du Speedup ;
* calcul de l'efficacité parallèle ;
* export automatique des résultats au format CSV.

---

## src/main.c

Programme principal.

Il assure :

* la création des matrices ;
* l'exécution des trois versions ;
* la validation des résultats ;
* les mesures de performances ;
* la génération des fichiers de résultats.

---

# Compilation

Le projet utilise **GCC** avec le support OpenMP.

Compilation :

```bash
make
```

Compilation manuelle :

```bash
gcc -O3 -fopenmp src/*.c -o matrix
```

---

# Exécution

```bash
./matrix
```

---

# Résultats produits

Les performances sont enregistrées automatiquement dans :

```text
results/
    strong_scalability.csv
    weak_scalability.csv
```

Ces fichiers peuvent être importés dans :

* Microsoft Excel ;
* LibreOffice Calc ;
* Python (Pandas, Matplotlib).

---

# Concepts étudiés

Le projet met en œuvre plusieurs notions fondamentales de l'architecture parallèle :

* programmation parallèle avec OpenMP ;
* mémoire partagée ;
* ordonnancement statique (`schedule(static)`) ;
* parallélisation des boucles ;
* produit matriciel parallèle ;
* optimisation par blocs (Blocking/Tiling) ;
* localité spatiale et temporelle ;
* hiérarchie mémoire (cache L1, L2, L3) ;
* Speedup ;
* efficacité parallèle ;
* scalabilité forte ;
* scalabilité faible ;
* loi d'Amdahl.

---

# Performances analysées

Les expérimentations permettent de comparer :

* le temps d'exécution séquentiel ;
* le temps d'exécution parallèle ;
* le Speedup ;
* l'efficacité parallèle ;
* les performances de la version naïve ;
* les performances de la version optimisée par blocs.

Les résultats sont analysés en tenant compte :

* des coûts de synchronisation ;
* de la bande passante mémoire ;
* de l'utilisation du cache ;
* de l'impact de la taille des blocs ;
* des limites imposées par la loi d'Amdahl.

---

# Auteur

**Saint-Vil RENÉ**

Certificat en Ingénierie des Données

Université d'État d'Haïti (UEH)

---

# Encadrement

**Cours :** Architecture Parallèle

**Enseignant :** Jean Andris Adam

---

# Année académique

**2025 – 2026**
