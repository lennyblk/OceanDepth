#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../include/utils.h"
#include "../include/constants.h"

void init_random() {
    srand(time(NULL));
}

int random_range(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min + 1);
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void get_string_input(char *buffer, int max_length) {
    if (fgets(buffer, max_length, stdin) != NULL) {
        // Supprime le '\n' à la fin si présent
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0'; // Chaîne vide en cas d'erreur
    }
}

int get_int_input(int min, int max) {
    char buffer[MAX_INPUT_LENGTH];
    int value;
    char *endptr;
    
    while (1) {
        printf("Entrez un nombre entre %d et %d: ", min, max);
        get_string_input(buffer, MAX_INPUT_LENGTH);
        
        // Conversion en entier
        value = strtol(buffer, &endptr, 10);
        
        // Vérification de la validité
        if (*endptr == '\0' && value >= min && value <= max) {
            return value;
        }
        
        printf(COLOR_RED "%s" COLOR_RESET "\n", ERROR_INVALID_INPUT);
    }
}

// Lecture d'un caractère unique (pour les menus)
char get_char_input() {
    char buffer[MAX_INPUT_LENGTH];
    get_string_input(buffer, MAX_INPUT_LENGTH);
    
    if (strlen(buffer) == 1) {
        return tolower(buffer[0]);
    }
    
    return '\0'; 
}

void print_separator(char character, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", character);
    }
    printf("\n");
}


void print_error(const char *message) {
    printf(COLOR_RED "ERREUR: %s" COLOR_RESET "\n", message);
}

void print_info(const char *message) {
    printf(COLOR_BLUE "INFO: %s" COLOR_RESET "\n", message);
}

void pause_screen() {
    printf(COLOR_YELLOW "\nAppuyez sur Entrée pour continuer..." COLOR_RESET);
    getchar();
}

int confirm_action(const char *message) {
    char response;
    
    while (1) {
        printf("%s (o/n): ", message);
        response = get_char_input();
        
        if (response == 'o' || response == 'y') {
            return 1;
        } else if (response == 'n') {
            return 0;
        }
        
        printf(COLOR_RED "Répondez par 'o' (oui) ou 'n' (non)." COLOR_RESET "\n");
    }
}

void print_progress_bar(int current, int max, int width) {
    float percentage = (float)current / max;
    int filled = (int)(percentage * width);
    
    printf("[");
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printf(COLOR_GREEN "█" COLOR_RESET);
        } else {
            printf("░");
        }
    }
    printf("] %d/%d (%.1f%%)", current, max, percentage * 100);
}

void print_success(const char* message) {
    printf(COLOR_GREEN "✓ %s" COLOR_RESET "\n", message);
}

void print_title(const char* title) {
    print_separator('=', 60);
    printf(COLOR_CYAN COLOR_BOLD "  %s  " COLOR_RESET "\n", title);
    print_separator('=', 60);
}