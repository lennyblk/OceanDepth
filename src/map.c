#include <stdio.h>
#include <stdlib.h>
#include "../include/types.h"
#include "../include/map.h"

void map_init(Map *map)
{
    map->zone_count = 4;
    map->current_zone_index = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            map->zones[i].destinations[j].cleared = 0;
        }
    }
}