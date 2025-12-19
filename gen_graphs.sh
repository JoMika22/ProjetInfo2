#!/bin/bash
# Script pour générer les histogrammes PNG à partir d'un fichier de données usine (ex: vol_max.dat)
# Usage: ./gen_graphs.sh vol_max.dat

if [ $# -ne 1 ]; then
    echo "Usage: $0 <fichier_donnees>"
    exit 1
fi

DATAFILE="$1"
BASENAME="${DATAFILE%.*}"

# Vérification du fichier
if [ ! -f "$DATAFILE" ]; then
    echo "Fichier $DATAFILE introuvable."
    exit 2
fi

# Générer les 50 plus petites usines
head -1 "$DATAFILE" > "${BASENAME}_50.dat"
tail -n +2 "$DATAFILE" | sort -t';' -k2 -n | head -50 >> "${BASENAME}_50.dat"

# Générer les 10 plus grandes usines
head -1 "$DATAFILE" > "${BASENAME}_10.dat"
tail -n +2 "$DATAFILE" | sort -t';' -k2 -n | tail -10 >> "${BASENAME}_10.dat"

# Générer l'histogramme des 50 plus petites usines
gnuplot <<EOF
set terminal png size 1200,600
set output '${BASENAME}_50.png'
set datafile separator ';'
set style data histograms
set style fill solid 1.00 border -1
set boxwidth 0.7
set xtics rotate by -45
set title '50 plus petites usines - Capacité maximale'
set xlabel 'Usine'
set ylabel 'Volume (k.m3/an)'
plot '${BASENAME}_50.dat' using 2:xtic(1) title 'Capacité max'
EOF

# Générer l'histogramme des 10 plus grandes usines
gnuplot <<EOF
set terminal png size 1200,600
set output '${BASENAME}_10.png'
set datafile separator ';'
set style data histograms
set style fill solid 1.00 border -1
set boxwidth 0.7
set xtics rotate by -45
set title '10 plus grandes usines - Capacité maximale'
set xlabel 'Usine'
set ylabel 'Volume (k.m3/an)'
plot '${BASENAME}_10.dat' using 2:xtic(1) title 'Capacité max'
EOF

echo "Graphiques générés : ${BASENAME}_50.png et ${BASENAME}_10.png"
