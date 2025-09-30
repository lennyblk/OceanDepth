#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/map.h"
#include "../include/utils.h"
#include "../include/constants.h"


void initialize_game(Player *player, Map *map) {
    clear_screen();
    printf(COLOR_CYAN COLOR_BOLD "=== CRÉATION DE VOTRE SLAYER ===" COLOR_RESET "\n\n");
    
    display_player_ascii();
    
    printf("Bienvenue dans les profondeurs mystérieuses de l'océan !\n");
    printf("Comment vous appelez-vous, notre Monster Slayer ? ");
    
    char name[MAX_NAME_LENGTH];
    get_string_input(name, sizeof(name));
    
    player_init_with_name(player, name);
    
    map_init(map);
    
    printf("\nSalut %s ! Préparez-vous pour une aventure sous-marine épique !\n", player->name);
    printf("Vous commencez votre périple à la surface, avec votre équipement de base.\n");
    
    pause_screen();
}