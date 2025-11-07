#include <stdio.h>
#include <stdlib.h>
#include "../include/types.h"
#include "../include/map.h"
#include "../include/creature.h"
#include "../include/constants.h"

void map_init(Map *map)
{
    map->zone_count = 4;  // On commence avec 4 zones
    map->current_zone_index = 0;

    for (int i = 0; i < 4; i++)
    {
        generate_new_zone(map, i);
    }
}

void generate_new_zone(Map *map, int zone_index)
{
    if (zone_index >= 100) return; 
    
    int zone_depth = zone_index * 50;
    
    size_t creature_count;
    generate_creatures(zone_depth, map->zones[zone_index].creatures, 4, &creature_count);
    map->zones[zone_index].creature_count = (int)creature_count;
    
    for (int j = 0; j < 4; j++)
    {
        map->zones[zone_index].destinations[j].cleared = 0;
    }
}