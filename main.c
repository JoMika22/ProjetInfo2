#include "parser.h"
#include "avl.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes
static int process_histo(const char *csv, const char *option);
static int process_leaks(const char *csv, const char *plant_id);

// Helper : tester si ID est une source
static int is_source(const char *id) {
    if (id == NULL) {
        return 0;
    }
    
    if (strstr(id, "Spring #") == id) return 1;
    if (strstr(id, "Source #") == id) return 1;
    if (strstr(id, "Well #") == id) return 1;
    if (strstr(id, "Resurgence #") == id) return 1;
    if (strstr(id, "Fountain #") == id) return 1;
    if (strstr(id, "Catchment basin #") == id) return 1;
    if (strstr(id, "Pond #") == id) return 1;
    if (strstr(id, "Intake #") == id) return 1;
    if (strstr(id, "Borehole #") == id) return 1;
    
    return 0;
}

// Helper : tester si ID est une usine
static int is_plant(const char *id) {
    if (id == NULL) {
        return 0;
    }
    
    if (strstr(id, "Facility complex #") == id) return 1;
    if (strstr(id, "Plant #") == id) return 1;
    if (strstr(id, "Module #") == id) return 1;
    if (strstr(id, "Unit #") == id) return 1;
    
    return 0;
}

// Structure pour stocker les volumes
typedef struct {
    double max_capacity;
    double source_volume;
    double real_volume;
} PlantData;

// Callback pour histo
int collect_histo(Segment *seg, void *userdata) {
    AVLNode **avl = (AVLNode**)userdata;
    
    // Ligne usine
    if (is_plant(seg->upstream_id) && seg->downstream_id == NULL && seg->volume > 0) {
        AVLNode *found = avl_find(*avl, seg->upstream_id, cmp_str);
        PlantData *pd;
        
        if (found != NULL) {
            pd = (PlantData*)found->value;
        } else {
            pd = calloc(1, sizeof(PlantData));
        }
        
        if (pd == NULL) {
            return 1;
        }
        
        pd->max_capacity = seg->volume;
        
        if (found == NULL) {
            *avl = avl_insert(*avl, seg->upstream_id, pd, cmp_str);
        }
        return 0;
    }
    
    // Ligne source -> usine (captage)
    if (is_source(seg->upstream_id) && is_plant(seg->downstream_id) && seg->volume > 0) {
        AVLNode *found = avl_find(*avl, seg->downstream_id, cmp_str);
        PlantData *pd;
        
        if (found != NULL) {
            pd = (PlantData*)found->value;
        } else {
            pd = calloc(1, sizeof(PlantData));
        }
        
        if (pd == NULL) {
            return 1;
        }
        
        pd->source_volume = pd->source_volume + seg->volume;
        double leak_pct = seg->leakage / 100.0;
        pd->real_volume = pd->real_volume + (seg->volume * (1.0 - leak_pct));
        
        if (found == NULL) {
            *avl = avl_insert(*avl, seg->downstream_id, pd, cmp_str);
        }
        return 0;
    }
    
    return 0;
}

// Visiteur pour écrire les usines
void write_histo_max(AVLNode *node, void *arg) {
    PlantData *pd = (PlantData*)node->value;
    double vmax = pd->max_capacity / 1000.0;
    fprintf((FILE*)arg, "%s;%.3f\n", node->key, vmax);
}

void write_histo_src(AVLNode *node, void *arg) {
    PlantData *pd = (PlantData*)node->value;
    double vsrc = pd->source_volume / 1000.0;
    fprintf((FILE*)arg, "%s;%.3f\n", node->key, vsrc);
}

void write_histo_real(AVLNode *node, void *arg) {
    PlantData *pd = (PlantData*)node->value;
    double vreal = pd->real_volume / 1000.0;
    fprintf((FILE*)arg, "%s;%.3f\n", node->key, vreal);
}

void write_histo_all(AVLNode *node, void *arg) {
    PlantData *pd = (PlantData*)node->value;
    double vmax = pd->max_capacity / 1000.0;
    double vsrc = pd->source_volume / 1000.0;
    double vreal = pd->real_volume / 1000.0;
    fprintf((FILE*)arg, "%s;%.3f;%.3f;%.3f\n", node->key, vmax, vsrc, vreal);
}

// Traiter histogramme
static int process_histo(const char *csv, const char *option) {
    AVLNode *plants_avl = NULL;
    
    // Parser le CSV
    int res = parse_csv_file(csv, collect_histo, &plants_avl);
    if (res != 0) {
        fprintf(stderr, "Erreur parsing CSV\n");
        avl_free(plants_avl, free);
        return res;
    }
    
    // Déterminer fichier sortie
    char outname[256];
    char header[256];
    void (*writer)(AVLNode*, void*) = NULL;
    
    if (strcmp(option, "max") == 0) {
        snprintf(outname, sizeof(outname), "vol_max.dat");
        snprintf(header, sizeof(header), "identifier;max volume (M.m3)\n");
        writer = write_histo_max;
    } else if (strcmp(option, "src") == 0) {
        snprintf(outname, sizeof(outname), "vol_captation.txt");
        snprintf(header, sizeof(header), "identifier;source volume (M.m3)\n");
        writer = write_histo_src;
    } else if (strcmp(option, "real") == 0) {
        snprintf(outname, sizeof(outname), "vol_traitement.tmp");
        snprintf(header, sizeof(header), "identifier;real volume (M.m3)\n");
        writer = write_histo_real;
    } else if (strcmp(option, "all") == 0) {
        snprintf(outname, sizeof(outname), "vol_all.dat");
        snprintf(header, sizeof(header), "identifier;max volume (M.m3);source volume (M.m3);real volume (M.m3)\n");
        writer = write_histo_all;
    } else {
        fprintf(stderr, "Option histo inconnue: %s\n", option);
        avl_free(plants_avl, free);
        return 3;
    }
    
    // Écrire fichier
    FILE *fout = fopen(outname, "w");
    if (fout == NULL) {
        fprintf(stderr, "Erreur: impossible d'ouvrir %s\n", outname);
        avl_free(plants_avl, free);
        return 2;
    }
    
    fprintf(fout, "%s", header);
    avl_inorder_reverse(plants_avl, writer, fout);
    fclose(fout);
    
    avl_free(plants_avl, free);
    printf("[INFO] Fichier généré: %s\n", outname);
    return 0;
}

// Traiter fuites (stub pour maintenant)
static int process_leaks(const char *csv, const char *plant_id) {
    printf("[INFO] Calcul des fuites pour %s\n", plant_id);
    // TODO: implémenter calcul fuites complet
    return 0;
}

// Main
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <csv_file> <command> [options]\n", argv[0]);
        fprintf(stderr, "  histo [max|src|real|all]\n");
        fprintf(stderr, "  leaks <plant_id>\n");
        return 1;
    }
    
    const char *csv = argv[1];
    const char *cmd = argv[2];
    
    if (strcmp(cmd, "histo") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Erreur: histo requires option [max|src|real|all]\n");
            return 1;
        }
        return process_histo(csv, argv[3]);
    } else if (strcmp(cmd, "leaks") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Erreur: leaks requires plant_id\n");
            return 1;
        }
        return process_leaks(csv, argv[3]);
    } else {
        fprintf(stderr, "Commande inconnue: %s\n", cmd);
        return 2;
    }
}