#ifndef MENU_H
#define MENU_H

#include "types.h"

// Fonctions principales du menu
void display_title_screen(void);
int display_main_menu(Player *player);
void handle_menu_choice(int choice, Player *player, Map *map);

// Fonctions des options du menu principal
void explore_map(Player *player, Map *map);
void search_creatures(Player *player, Map *map);
void visit_shop(Player *player);
void rest_at_surface(Player *player);
void manage_inventory(Player *player);
void manage_equipment(Player *player);
void save_game(Player *player, Map *map);
void display_journal(Player *player);

// Fonctions du système de carte et navigation
void display_zone_map(Player *player, Map *map);
void select_destination(Player *player, Map *map);
void enter_destination(Player *player, Map *map, int zone, int destination);

// Fonctions utilitaires pour le système de zones
int get_zone_depth(int zone);
int is_zone_unlocked(Player *player, int zone);
int is_destination_available(Map *map, int zone, int destination);
int is_destination_cleared(Map *map, int zone, int destination);
void mark_destination_cleared(Map *map, int zone, int destination);
int get_monsters_in_destination(int zone, int destination);
int is_zone_completely_cleared(Map *map, int zone);
void unlock_next_zone(Player *player, Map *map, int current_zone);

// Fonctions de combat
void create_creatures_for_zone(Creature creatures[], int creature_count, int zone);
int fight_all_monsters(Player *player, int zone, int monsters_count);
void give_zone_rewards(Player *player, int zone, int destination);

// Fonctions utilitaires
int confirm_action(const char *message);

#endif