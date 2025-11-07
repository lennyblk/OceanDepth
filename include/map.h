#ifndef MAP_H
#define MAP_H

#include "types.h"

void map_init(Map *map);
void generate_new_zone(Map *map, int zone_index);

#endif