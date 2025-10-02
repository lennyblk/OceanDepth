#ifndef CREATURE_H
#define CREATURE_H

#include "types.h"

// création et gestion des créatures
Creature create_creature(CreatureType type, int zone_level);
void initialize_creature_stats(Creature *creature);
int get_creature_spawn_chance(int zone, CreatureType type);
Creature* generate_random_creatures(int zone, int *count);
const char* get_creature_name(CreatureType type);
const char* get_creature_emoji(CreatureType type);
void display_creature_info(Creature *creature);

// Fonctions utilitaires
int calculate_creature_level(int zone);
int is_creature_boss(CreatureType type);

#endif