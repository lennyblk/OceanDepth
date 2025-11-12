#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/map.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/ascii_art.h"

void initialize_game(Player *player, Map *map)
{
    clear_screen();
    printf(COLOR_CYAN COLOR_BOLD "=== CRÉATION DE VOTRE SLAYER ===" COLOR_RESET "\n\n");

    display_player_ascii();

    printf("Bienvenue dans les profondeurs mystérieuses de l'océan !\n");
    printf("Comment vous appelez-vous, notre Monster Slayer ?\n");
    printf(COLOR_YELLOW "(Maximum %d caractères): " COLOR_RESET, MAX_NAME_LENGTH - 1);

    char name[MAX_NAME_LENGTH];
    get_string_input(name, sizeof(name)); // name est vide ici 

    player_init_with_name(player, name);
    map_init(map);

    if (name[0] == 'T' || name[0] == 't')
    {
        printf(COLOR_RED "\n*** MODE TEST ACTIVÉ ***\n" COLOR_RESET);
        
        player->hp = 9999;
        player->max_hp = 9999;
        player->oxygen = 9999;
        player->max_oxygen = 9999;
        player->level = 50;
        
        strcpy(player->equipped_weapon.name, "Harpon Divin");
        player->equipped_weapon.attack_min = 500;
        player->equipped_weapon.attack_max = 1000;
        player->equipped_weapon.oxygen_cost_per_attack = 0;
        
        strcpy(player->equipped_armor.name, "Armure Divine");
        player->equipped_armor.defense = 999;
        
        player->zones_unlocked = 4;
        player->current_zone = 3;
        
        
        printf(COLOR_GREEN "✓ Perso de test créé !\n" COLOR_RESET);
    }
    else
    {
        printf("\nSalut %s ! Préparez-vous pour une aventure sous-marine épique !\n", player->name);
        printf("Vous commencez votre périple à la surface, avec votre équipement de base.\n");
    }

    pause_screen();
}