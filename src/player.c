#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/types.h"
#include "../include/ascii_art.h"  // Nouveau include

void player_init(Player *player) {
    if (player == NULL) {
        print_error("Pointeur player NULL dans player_init");
        return;
    }
    
    player->hp = STARTING_HP;
    player->max_hp = STARTING_HP;
    player->oxygen = STARTING_OXYGEN;
    player->max_oxygen = STARTING_OXYGEN;
    player->pearls = STARTING_PEARLS;
    player->experience = STARTING_EXPERIENCE;
    player->level = STARTING_LEVEL;
    
    player->x = 0;
    player->y = 0;
    player->current_depth = SURFACE_DEPTH;
    
    strcpy(player->name, "Monster Slayer");
    
    print_success("Joueur initialisé avec succès!");
}

void player_init_with_name(Player *player, const char *name) {
    player_init(player);
    
    if (name != NULL && strlen(name) > 0) {
        strncpy(player->name, name, MAX_NAME_LENGTH - 1);
        player->name[MAX_NAME_LENGTH - 1] = '\0'; 
    }
}

void player_display_stats(const Player *player) {
    if (player == NULL) {
        print_error("Pointeur player NULL dans player_display_stats");
        return;
    }
    
    clear_screen();
    
    // Affichage de l'ASCII art du joueur
    display_player_ascii();
    
    print_title("STATISTIQUES DU JOUEUR");
    
    printf(COLOR_BOLD "Nom: " COLOR_RESET "%s\n", player->name);
    printf(COLOR_BOLD "Niveau: " COLOR_RESET "%d\n", player->level);
    printf(COLOR_BOLD "Expérience: " COLOR_RESET "%d\n", player->experience);
    printf(COLOR_BOLD "Perles: " COLOR_RESET COLOR_YELLOW "%d" COLOR_RESET "\n", player->pearls);
    
    printf("\n" COLOR_BOLD "ÉTAT PHYSIQUE:" COLOR_RESET "\n");
    
    printf(COLOR_BOLD "Santé: " COLOR_RESET);
    print_progress_bar(player->hp, player->max_hp, 20);
    printf("\n");
    
    printf(COLOR_BOLD "Oxygène: " COLOR_RESET);
    print_progress_bar(player->oxygen, player->max_oxygen, 20);
    printf("\n");
    
    printf("\n" COLOR_BOLD "POSITION:" COLOR_RESET "\n");
    printf("Coordonnées: (%d, %d)\n", player->x, player->y);
    printf("Profondeur: %d mètres\n", player->current_depth);
    
    print_separator('-', 50);
}

int player_take_damage(Player *player, int damage) {
    if (player == NULL) {
        print_error("Pointeur player NULL dans player_take_damage");
        return 0;
    }
    
    if (damage <= 0) {
        return 1; // Pas de dégâts
    }
    
    player->hp -= damage;
    if (player->hp < 0) {
        player->hp = 0;
    }
    
    printf(COLOR_RED "%s subit %d points de dégâts!" COLOR_RESET "\n", 
           player->name, damage);
    
    if (player->hp == 0) {
        printf(COLOR_RED "%s est inconscient!" COLOR_RESET "\n", player->name);
        return 0; 
    }
    
    return 1;
}

void player_heal(Player *player, int heal_amount) {
    if (player == NULL) {
        print_error("Pointeur player NULL dans player_heal");
        return;
    }
    
    if (heal_amount <= 0) {
        return;
    }
    
    int old_hp = player->hp;
    player->hp += heal_amount;
    
    if (player->hp > player->max_hp) {
        player->hp = player->max_hp;
    }
    
    int actual_heal = player->hp - old_hp;
    
    if (actual_heal > 0) {
        printf(COLOR_GREEN "%s récupère %d points de santé!" COLOR_RESET "\n", 
               player->name, actual_heal);
    } else {
        printf(COLOR_YELLOW "%s est déjà en pleine santé!" COLOR_RESET "\n", 
               player->name);
    }
}

// Consomme de l'oxygène
int player_use_oxygen(Player *player, int oxygen_cost) {
    if (player == NULL) {
        print_error("Pointeur player NULL dans player_use_oxygen");
        return 0;
    }
    
    if (oxygen_cost <= 0) {
        return 1;
    }
    
    if (player->oxygen < oxygen_cost) {
        printf(COLOR_RED "Pas assez d'oxygène! (%d requis, %d disponible)" COLOR_RESET "\n", 
               oxygen_cost, player->oxygen);
        return 0;
    }
    
    player->oxygen -= oxygen_cost;
    
    if (player->oxygen <= 20) {
        printf(COLOR_RED "ATTENTION: Niveau d'oxygène critique! (%d%%)" COLOR_RESET "\n", 
               player->oxygen);
    }
    
    return 1;
}

void player_restore_oxygen(Player *player, int oxygen_amount) {
    if (player == NULL) {
        print_error("Pointeur player NULL dans player_restore_oxygen");
        return;
    }
    
    if (oxygen_amount <= 0) {
        return;
    }
    
    int old_oxygen = player->oxygen;
    player->oxygen += oxygen_amount;
    
    if (player->oxygen > player->max_oxygen) {
        player->oxygen = player->max_oxygen;
    }
    
    int actual_restore = player->oxygen - old_oxygen;
    
    if (actual_restore > 0) {
        printf(COLOR_CYAN "%s récupère %d%% d'oxygène!" COLOR_RESET "\n", 
               player->name, actual_restore);
    }
}

void player_move(Player *player, int delta_x, int delta_y) {
    if (player == NULL) {
        print_error("Pointeur player NULL dans player_move");
        return;
    }
    
    player->x += delta_x;
    player->y += delta_y;
    
    // Mise à jour de la profondeur basée sur Y
    player->current_depth = player->y * 10; // 10 mètres par unité Y
    if (player->current_depth < 0) {
        player->current_depth = 0;
    }
    
    // Consommation d'oxygène selon la profondeur
    int oxygen_cost = 1 + (player->current_depth / 100);
    player_use_oxygen(player, oxygen_cost);
    
    printf(COLOR_BLUE "%s se déplace à la position (%d, %d) - Profondeur: %d m" COLOR_RESET "\n", 
           player->name, player->x, player->y, player->current_depth);
}

int player_is_alive(const Player *player) {
    if (player == NULL) {
        return 0;
    }
    
    return (player->hp > 0 && player->oxygen > 0) ? 1 : 0;
}

void player_add_experience(Player *player, int exp) {
    if (player == NULL || exp <= 0) {
        return;
    }
    
    player->experience += exp;
    printf(COLOR_GREEN "%s gagne %d points d'expérience!" COLOR_RESET "\n", 
           player->name, exp);
    
    int required_exp = player->level * 100;
    
    while (player->experience >= required_exp) {
        player->level++;
        player->experience -= required_exp;
        
        // Bonus de montée de niveau
        int hp_bonus = 10;
        int oxygen_bonus = 5;
        
        player->max_hp += hp_bonus;
        player->hp = player->max_hp; // Soins complets
        player->max_oxygen += oxygen_bonus;
        player->oxygen = player->max_oxygen;
        
        printf(COLOR_YELLOW "🎉 %s monte au niveau %d!" COLOR_RESET "\n", 
               player->name, player->level);
        printf(COLOR_GREEN "Bonus: +%d HP max, +%d Oxygène max" COLOR_RESET "\n", 
               hp_bonus, oxygen_bonus);
        
        required_exp = player->level * 100;
    }
}

// Ajoute des perles
void player_add_pearls(Player *player, int pearls) {
    if (player == NULL || pearls <= 0) {
        return;
    }
    
    player->pearls += pearls;
    printf(COLOR_YELLOW "%s trouve %d perles! (Total: %d)" COLOR_RESET "\n", 
           player->name, pearls, player->pearls);
}