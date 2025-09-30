#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/ascii_art.h"
#include "../include/menu.h"
#include "../include/map.h"

#ifdef _WIN32
#include <windows.h>
#endif


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    init_random();
    
    Player *player = malloc(sizeof(Player));
    Map *map = malloc(sizeof(Map));
    
    if (!player || !map) {
        printf("Erreur d'allocation mémoire !");
        return 1;
    }
    
    initialize_game(player, map);
    
    while (player_is_alive(player)) {
        int choice = display_main_menu(player);
        handle_menu_choice(choice, player, map);
        
        if (player->oxygen <= 0) {
            clear_screen();
            printf(COLOR_RED COLOR_BOLD "💀 GAME OVER 💀\n" COLOR_RESET);
            printf("Vous avez manqué d'oxygène dans les profondeurs...\n");
            break;
        }
        
        if (player->hp <= 0) {
            clear_screen();
            printf(COLOR_RED COLOR_BOLD "💀 GAME OVER 💀\n" COLOR_RESET);
            printf("Vous avez succombé aux dangers des profondeurs...\n");
            break;
        }
    }
    
    free(player);
    free(map);
    
    return 0;
}