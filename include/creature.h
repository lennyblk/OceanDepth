#ifndef CREATURE_H
#define CREATURE_H

#include "types.h"
#include <stddef.h>

Creature create_creature(CreatureType type);

void generate_creatures(int zone_depth, Creature creature_array[], size_t array_size, size_t *generated_count);

const char *get_creature_name(CreatureType type);

const char *get_creature_emoji(CreatureType type);

void display_creature_info(const Creature *creature);

#endif