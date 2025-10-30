#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

void player_init(Player *player);
void player_init_with_name(Player *player, const char *name);
void player_init_skills(Player *player);

void player_display_stats(const Player *player);

int player_take_damage(Player *player, int damage);
void player_heal(Player *player, int heal_amount);
int player_use_oxygen(Player *player, int oxygen_cost);
void player_restore_oxygen(Player *player, int oxygen_amount);

void player_move(Player *player, int delta_x, int delta_y);

int player_is_alive(const Player *player);

void player_add_experience(Player *player, int exp);
void player_add_pearls(Player *player, int pearls);
void player_update_cooldowns(Player *player);

#endif