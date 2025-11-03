#include <stdio.h>
#include <stdlib.h>
#include "../include/types.h"
#include "../include/map.h"
#include "../include/creature.h"

void map_init(Map *map)
{
    map->zone_count = 4;
    map->current_zone_index = 0;

    for (int i = 0; i < 4; i++)
    {
        int zone_depth = i * 50; 
        
        size_t creature_count;
        generate_creatures(zone_depth, map->zones[i].creatures, 4, &creature_count);
        map->zones[i].creature_count = (int)creature_count;
        
        for (int j = 0; j < 4; j++)
        {
            map->zones[i].destinations[j].cleared = 0;
        }
    }
}