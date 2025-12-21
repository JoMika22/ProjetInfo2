# Projet C-WildWater

Ce projet permet de réaliser la synthèse de données d’un système de distribution d’eau potable à partir d'un fichier CSV volumineux. Il combine un script Shell pour le **pilotage** des étapes et un programme en C optimisé pour le traitement de millions de lignes.

---

## Contenu du projet

Le dépôt contient les fichiers suivants :

- **README.md** : Documentation complète du projet.
- **myScript.sh** : Point d'entrée principal qui coordonne les calculs et la génération des graphiques.
- **gengraph.sh** : Script utilitaire pour générer les histogrammes PNG à partir d'un fichier de données `.dat`.
- **Makefile** : Outil permettant la compilation automatisée du programme C.
- **Fichiers source en C** :
  - `main.c` : Point d'entrée du programme et gestion des arguments.
  - `parser.c` / `parser.h` : Analyse du fichier CSV et conversion des données textuelles en structures utilisables.
  - `avl.c` / `avl.h` : Implémentation des arbres équilibrés (AVL) pour garantir une recherche rapide en O(log n).
  - `network.c` / `network.h` : Structures de données définissant les composants du réseau (Tronçons, Usines, Nœuds).
  - `utils.c` / `utils.h` : Fonctions utilitaires (comparaison de chaînes, gestion mémoire sécurisée).
- **tests/** : Dossier regroupant les résultats d'exécution, incluant les images PNG et les fichiers de données.

---

## Installation & Compilation

1. **Cloner le dépôt** :
   ```bash
   git clone [https://github.com/JoMika22/ProjetInfo2.git](https://github.com/JoMika22/ProjetInfo2.git)
   cd ProjetInfo2
   ```

2. **Compilation** :
   La compilation est gérée par le `Makefile`. Le script Shell vérifie automatiquement la présence de l'exécutable (`wildwater`) et lance la compilation si nécessaire via l'appel à `make` :
   ```bash
   make
   ```

---

## Utilisation

Le script `myScript.sh` accepte deux types de traitements principaux :

### 1. Histogrammes des usines (`histo`)
Génère des graphiques PNG (Top 10 et Bottom 50) sur les capacités ou les volumes.

```bash
# Options : max (capacité), src (captage), real (réellement traité)
./myScript.sh <fichier.csv> histo <option>
```

### 2. Rendement de distribution (`leaks`)
Calcule le volume total d'eau perdu sur l'ensemble du réseau en aval d'une usine spécifique.

```bash
./myScript.sh <fichier.csv> leaks "<Identifiant_Usine>"
```

### 3. Génération manuelle de graphiques
Si vous disposez déjà d'un fichier de données extrait, vous pouvez utiliser le script dédié :

```bash
./gengraph.sh <fichier_donnees.dat>
```

---

## Format des données

Le programme traite des fichiers CSV structurés en 5 colonnes séparées par des points-virgules :

1. **Identifiant Usine** : Utilisé pour identifier l'usine liée au tronçon.
2. **Identifiant Amont** : Point de départ du tronçon (parent).
3. **Identifiant Aval** : Point d'arrivée du tronçon (enfant).
4. **Volume/Capacité** : Volume capté ou capacité maximale de l'usine.
5. **Pourcentage de fuites** : Taux de perte dans le tronçon.

> **Note** : Les valeurs manquantes sont systématiquement indiquées par un tiret `-`.

---

## Structure du projet

```text
ProjetInfo2/
├── src/                # Code source (.c)
│   ├── main.c
│   ├── parser.c
│   ├── avl.c
│   ├── network.c
│   └── utils.c
├── include/            # En-têtes (.h)
│   ├── parser.h
│   ├── avl.h
│   ├── network.h
│   └── utils.h
├── tests/              # Résultats d'exécution (PNG, .dat)
├── Makefile            # Script de compilation
├── myScript.sh         # Script de pilotage Shell
├── gengraph.sh         # Générateur de graphiques
└── README.md           # Documentation
```

---

##  Auteurs

- BOTNARI Alexandru
- TAKHIM Rayan
- TEIXEIRA COSTA Joao
