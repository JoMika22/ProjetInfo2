#!/bin/bash

################################################################################
# SCRIPT SHELL - WildWater
# Point d'entrée du projet
# Gère les arguments, la compilation C, et l'appel au programme
################################################################################

# Couleurs pour les messages
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Variables globales
START_TIME=$(date +%s%N)
CSV_FILE="$1"
COMMAND="$2"
PARAM="$3"

################################################################################
# FONCTIONS UTILITAIRES
################################################################################

print_error() {
    echo -e "${RED}❌ ERREUR: $1${NC}" >&2
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ️  $1${NC}"
}

print_usage() {
    cat << EOF
Utilisation : $0 <fichier_csv> <commande> [paramètre]

Commandes disponibles:
  histo max    - Affiche l'histogramme des capacités maximales
  histo src    - Affiche l'histogramme des volumes captés
  histo real   - Affiche l'histogramme des volumes traités
  histo all    - Affiche l'histogramme cumulé (BONUS)
  leaks ID     - Calcule les fuites pour une usine spécifique

Exemples:
  $0 data.csv histo max
  $0 data.csv leaks "Facility complex #RH400057F"
EOF
}

elapsed_time() {
    END_TIME=$(date +%s%N)
    ELAPSED=$(( (END_TIME - START_TIME) / 1000000 ))
    echo "Temps écoulé : ${ELAPSED} ms"
}

cleanup_on_exit() {
    elapsed_time
}

trap cleanup_on_exit EXIT

################################################################################
# VALIDATION DES ARGUMENTS
################################################################################

if [ $# -lt 2 ]; then
    print_error "Nombre d'arguments insuffisant"
    print_usage
    exit 1
fi

if [ ! -f "$CSV_FILE" ]; then
    print_error "Fichier CSV introuvable: $CSV_FILE"
    exit 1
fi

case "$COMMAND" in
    histo)
        if [ -z "$PARAM" ]; then
            print_error "Commande 'histo' requiert un paramètre (max|src|real|all)"
            print_usage
            exit 1
        fi
        case "$PARAM" in
            max|src|real|all)
                ;;
            *)
                print_error "Paramètre invalide pour 'histo': $PARAM"
                print_usage
                exit 1
                ;;
        esac
        ;;
    leaks)
        if [ -z "$PARAM" ]; then
            print_error "Commande 'leaks' requiert un identifiant d'usine"
            print_usage
            exit 1
        fi
        ;;
    *)
        print_error "Commande inconnue: $COMMAND"
        print_usage
        exit 1
        ;;
esac

# Vérifier les arguments supplémentaires non attendus
if [ $# -gt 3 ]; then
    print_error "Arguments supplémentaires inattendus"
    exit 1
fi

################################################################################
# COMPILATION
################################################################################

print_info "Vérification de la compilation..."

if [ ! -f "Makefile" ]; then
    print_error "Fichier Makefile introuvable"
    exit 1
fi

# Compiler le projet
make > /dev/null 2>&1
MAKE_RESULT=$?

if [ $MAKE_RESULT -ne 0 ]; then
    print_error "Compilation échouée"
    make
    exit 1
fi

if [ ! -f "wildwater" ]; then
    print_error "L'exécutable 'wildwater' n'a pas été généré"
    exit 1
fi

print_success "Compilation réussie"

################################################################################
# EXÉCUTION DU PROGRAMME C
################################################################################

print_info "Exécution du programme..."

if [ "$COMMAND" = "histo" ]; then
    ./wildwater "$CSV_FILE" "$COMMAND" "$PARAM"
    RESULT=$?
else
    ./wildwater "$CSV_FILE" "$COMMAND" "$PARAM"
    RESULT=$?
fi

if [ $RESULT -ne 0 ]; then
    print_error "Le programme a retourné un code d'erreur: $RESULT"
    exit $RESULT
fi

print_success "Traitement terminé"

exit 0
