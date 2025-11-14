#ifndef CONSTANTS_H
#define CONSTANTS_H

// Limites générales
#define MAX_INPUT_LENGTH 100
#define MAX_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 200

// Stats de départ du joueur
#define STARTING_HP 100
#define STARTING_OXYGEN 100
#define STARTING_PEARLS 0
#define STARTING_EXPERIENCE 0
#define STARTING_LEVEL 1

// Limites du monde
#define MAX_ZONES 100
#define MAX_CREATURES_PER_ZONE 5
#define MAX_INVENTORY_SIZE 20

// Couleurs ANSI pour l'affichage
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"

// Messages d'erreur
#define ERROR_INVALID_INPUT "Entrée invalide. Veuillez réessayer."
#define ERROR_MEMORY_ALLOCATION "Erreur d'allocation mémoire."

// Profondeurs des zones
#define SURFACE_DEPTH 0
#define SHALLOW_DEPTH 50
#define MEDIUM_DEPTH 200
#define DEEP_DEPTH 500
#define ABYSS_DEPTH 1000

#endif