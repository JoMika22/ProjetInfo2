#!/bin/bash
# Script principal pour C-WildWater


usage() {
  echo "Usage: $0 <fichier.csv> <commande> <option>"
  echo "Commandes:"
  echo "  histo [max|src|real]"
  echo "  leaks <PlantID>"
  exit 1
}


if [ $# -lt 3 ]; then
  usage
fi

CSV="$1"
CMD="$2"
OPT="$3"
EXEC="wildwater"
LEAKS_LOG="leaks_history.dat"



if [ ! -f "$EXEC" ]; then
  make || { echo "Compilation échouée."; exit 2; }
fi

START=$(date +%s%3N)

case "$CMD" in
  histo)
    # Lancer le C, qui génère le bon fichier selon l'option
    ./$EXEC "$CSV" histo "$OPT"
    RET=$?
    if [ $RET -ne 0 ]; then exit $RET; fi
    # Déterminer le nom du fichier de sortie
    if [ "$OPT" = "max" ]; then
      HISTO_OUT="vol_max.dat"
      TITLE="Capacité maximale de traitement des usines"
      YLABEL="Volume max (M.m3.an-1)"
    elif [ "$OPT" = "src" ]; then
      HISTO_OUT="vol_captation.txt"
      TITLE="Volume total capté par les sources"
      YLABEL="Volume capté (M.m3.an-1)"
    elif [ "$OPT" = "real" ]; then
      HISTO_OUT="vol_traitement.tmp"
      TITLE="Volume total réellement traité"
      YLABEL="Volume réel (M.m3.an-1)"
    else
      echo "Option histo inconnue"; exit 3
    fi
    # Générer top 10 (plus grandes valeurs)
    OUT_TOP="${HISTO_OUT%.dat}_top10.png"
    OUT_BOT="${HISTO_OUT%.dat}_bottom50.png"
    # Extraire les 10 plus grandes (hors header)
    (head -n 1 "$HISTO_OUT" && tail -n +2 "$HISTO_OUT" | sort -t';' -k2,2nr | head -n 10) > __top10.tmp
    # Extraire les 50 plus petites (hors header)
    (head -n 1 "$HISTO_OUT" && tail -n +2 "$HISTO_OUT" | sort -t';' -k2,2n | head -n 50) > __bottom50.tmp
    # Gnuplot les 10 plus grandes
    gnuplot -persist <<EOF
set terminal pngcairo size 1366,768 enhanced font 'Arial,14'
set output "$OUT_TOP"
set datafile separator ";"
set style data histograms
set style fill solid 0.7 border -1
set boxwidth 0.7
set xtics rotate by -45
set grid ytics
set title "$TITLE (10 plus grandes)"
set ylabel "$YLABEL"
set xlabel "Identifiants usines"
plot "__top10.tmp" using 2:xtic(1) title columnheader(2) lc rgb "#4F81BD"
EOF
    # Gnuplot les 50 plus petites
    gnuplot -persist <<EOF
set terminal pngcairo size 1366,768 enhanced font 'Arial,14'
set output "$OUT_BOT"
set datafile separator ";"
set style data histograms
set style fill solid 0.7 border -1
set boxwidth 0.7
set xtics rotate by -45
set grid ytics
set title "$TITLE (50 plus petites)"
set ylabel "$YLABEL"
set xlabel "Identifiants usines"
plot "__bottom50.tmp" using 2:xtic(1) title columnheader(2) lc rgb "#4F81BD"
EOF
    rm -f __top10.tmp __bottom50.tmp
    ;;
  leaks)
    ./$EXEC "$CSV" leaks "$OPT" > tmp_leak.txt || exit 4
    cat tmp_leak.txt >> "$LEAKS_LOG"
    rm -f tmp_leak.txt
    ;;
  *)
    usage
    ;;
esac

END=$(date +%s%3N)
DURATION=$((END-START))
echo "Durée totale : ${DURATION} ms"
