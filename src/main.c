#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/ascii_art.h"
#include "../include/menu.h"
#include "../include/map.h"
#include "../include/save.h"
#include "../include/init.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    init_random();

    Player *player = malloc(sizeof(Player));
    Map *map = malloc(sizeof(Map));
    int game_time = 0;

    if (!player || !map)
    {
        printf("Erreur d'allocation mémoire !");
        return 1;
    }

    if (save_exists())
    {
        clear_screen();
        printf(COLOR_YELLOW COLOR_BOLD "💾 SAUVEGARDE DÉTECTÉE\n" COLOR_RESET);
        print_separator('=', 60);
        printf("\nUne partie sauvegardée a été trouvée.\n");
        printf("Voulez-vous la charger ?\n\n");
        printf(COLOR_GREEN "1." COLOR_RESET " Charger la partie sauvegardée\n");
        printf(COLOR_GREEN "2." COLOR_RESET " Commencer une nouvelle partie\n");
        printf("\n" COLOR_BOLD "Votre choix: " COLOR_RESET);

        char choice = get_char_input();

        if (choice == '1')
        {
            if (load_game(player, map, &game_time))
            {
                printf(COLOR_GREEN "\n✅ Partie chargée avec succès !\n" COLOR_RESET);
                pause_screen();
            }
            else
            {
                printf(COLOR_RED "\n❌ Échec du chargement. Nouvelle partie créée.\n" COLOR_RESET);
                pause_screen();
                initialize_game(player, map);
            }
        }
        else
        {
            printf(COLOR_YELLOW "\n⚠️  Attention: La sauvegarde existante sera écrasée.\n" COLOR_RESET);
            printf("Êtes-vous sûr de vouloir commencer une nouvelle partie ? (o/n): ");
            char confirm = get_char_input();

            if (confirm == 'o' || confirm == 'O')
            {
                initialize_game(player, map);
            }
            else
            {
                if (load_game(player, map, &game_time))
                {
                    printf(COLOR_GREEN "\n✅ Partie sauvegardée chargée.\n" COLOR_RESET);
                    pause_screen();
                }
                else
                {
                    printf(COLOR_RED "\nErreur de chargement.\n" COLOR_RESET);
                    free(player);
                    free(map);
                    return 1;
                }
            }
        }
    }
    else
    {
        initialize_game(player, map);
    }

    while (player_is_alive(player))
    {
        int choice = display_main_menu(player);
        handle_menu_choice(choice, player, map, &game_time);
        game_time++;

        if (player->oxygen <= 0)
        {
            clear_screen();
            printf(COLOR_RED COLOR_BOLD "💀 GAME OVER 💀\n" COLOR_RESET);
            printf("Vous avez manqué d'oxygène dans les profondeurs...\n");
            printf("\nTemps de jeu: %d tours\n", game_time);
            break;
        }

        if (player->hp <= 0)
        {
            clear_screen();
            printf(COLOR_RED COLOR_BOLD "💀 GAME OVER 💀\n" COLOR_RESET);
            printf("Vous avez succombé aux dangers des profondeurs...\n");
            printf("\nTemps de jeu: %d tours\n", game_time);
            break;
        }
    }

    free(player);
    free(map);

    return 0;
}