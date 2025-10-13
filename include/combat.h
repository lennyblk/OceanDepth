#ifndef COMBAT_H
#define COMBAT_H

#include "types.h"

int start_combat(Player *player, Creature creatures[], int creature_count);
void display_combat_status(const Player *player, const Creature creatures[], int creature_count);
int player_turn(Player *player, Creature creatures[], int creature_count);
void creatures_turn(Player *player, Creature creatures[], int creature_count);

int get_player_attacks_per_turn(const Player *player);
int attack_creature(Player *player, Creature *target);
void use_skill(Player *player, Creature creatures[], int creature_count);
void use_item(Player *player);

int calculate_player_damage(const Player *player, const Creature *target);
int calculate_creature_damage(const Creature *attacker, const Player *player);
void apply_special_effect(Player *player, SpecialEffect effect, int duration);
int check_combat_end(const Player *player, const Creature creatures[], int creature_count);

void display_combat_menu(int attacks_left);
Creature* select_target(Creature creatures[], int creature_count);
int count_alive_creatures(const Creature creatures[], int creature_count);

#endif