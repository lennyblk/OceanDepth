#ifndef COMBAT_H
#define COMBAT_H

#include "types.h"

// Fonctions combat
int start_combat(Player *player, Creature *creatures, int creature_count);
void display_combat_status(Player *player, Creature *creatures, int creature_count);
int player_turn(Player *player, Creature *creatures, int creature_count);
void creatures_turn(Player *player, Creature *creatures, int creature_count);

// Actions joueur
int attack_creature(Player *player, Creature *target);
void use_skill(Player *player, Creature *creatures, int creature_count);
void use_item(Player *player);
int try_escape(Player *player, Creature *creatures, int creature_count);

// Calculs combat
int calculate_damage(int attack_min, int attack_max, int defense);
void apply_special_effect(Player *player, SpecialEffect effect, int duration);
int check_combat_end(Player *player, Creature *creatures, int creature_count);

// Utilitaires combat
void display_combat_menu(void);
Creature* select_target(Creature *creatures, int creature_count);
int count_alive_creatures(Creature *creatures, int creature_count);
void give_combat_rewards(Player *player, Creature *creatures, int creature_count);

#endif