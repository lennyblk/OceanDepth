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

// Génère un nombre aléatoire entre min et max (inclus)
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

// Lecture sécurisée d'une chaîne de caractères
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

// Lecture sécurisée d'un entier avec validation
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

// Affiche une ligne de séparation
void print_separator(char character, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", character);
    }
    printf("\n");
}

// Affiche un titre centré avec des bordures
void print_title(const char *title) {
    int title_length = strlen(title);
    int total_width = 60;
    int padding = (total_width - title_length - 2) / 2;
    
    print_separator('=', total_width);
    printf("|");
    
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    
    printf(COLOR_BOLD COLOR_CYAN "%s" COLOR_RESET, title);
    
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    
    // Ajuste si la longueur est impaire
    if ((total_width - title_length - 2) % 2 != 0) {
        printf(" ");
    }
    
    printf("|\n");
    print_separator('=', total_width);
}

void print_error(const char *message) {
    printf(COLOR_RED "ERREUR: %s" COLOR_RESET "\n", message);
}

void print_success(const char *message) {
    printf(COLOR_GREEN "SUCCÈS: %s" COLOR_RESET "\n", message);
}

// Affiche un message d'information en bleu
void print_info(const char *message) {
    printf(COLOR_BLUE "INFO: %s" COLOR_RESET "\n", message);
}

// Pause avec message "Appuyez sur Entrée pour continuer"
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

// Convertit une chaîne en minuscules
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Supprime les espaces en début et fin de chaîne
void trim_string(char *str) {
    int start = 0;
    int end = strlen(str) - 1;
    
    // Trouve le premier caractère non-espace
    while (str[start] && isspace(str[start])) {
        start++;
    }
    
    // Trouve le dernier caractère non-espace
    while (end >= start && isspace(str[end])) {
        end--;
    }
    
    // Déplace la chaîne
    int length = end - start + 1;
    memmove(str, str + start, length);
    str[length] = '\0';
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