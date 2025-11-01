#ifndef COMBAT_H
#define COMBAT_H

#include "types.h"

// Fonctions combat
int start_combat(Player *player, Creature creatures[], int creature_count);
void display_combat_status(const Player *player, const Creature creatures[], int creature_count);
int player_turn(Player *player, Creature creatures[], int creature_count);
void creatures_turn(Player *player, Creature creatures[], int creature_count);

// Actions joueur
int attack_creature(Player *player, Creature *target);
void display_skills_menu(const Player *player);
int handle_skill_choice(Player *player, Creature creatures[], int creature_count);
void use_item(Player *player);

// Utilitaires combat
int calculate_damage(int attack_min, int attack_max, int defense);
int calculate_player_damage(const Player *player, const Creature *target);
int calculate_creature_damage(const Creature *attacker, const Player *player);
int check_combat_end(const Player *player, const Creature creatures[], int creature_count);
void display_combat_menu(int attacks_left);
Creature *select_target(Creature creatures[], int creature_count);
int count_alive_creatures(const Creature creatures[], int creature_count);

#endif