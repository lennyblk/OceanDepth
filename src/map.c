#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/map.h"
#include "../include/creature.h"
#include "../include/constants.h"

void map_init(Map *map)
{
    map->zone_count = 4;  // On commence avec 4 zones
    map->current_zone_index = 0;

    ZoneType initial_types[] = {ZONE_SURFACE, ZONE_REEF, ZONE_ABYSS, ZONE_ABYSS};
    const char *initial_names[] = {"Surface", "Récifs Coralliens", "Territoire des Monstres", "Début des Abysses"};

    for (int i = 0; i < 4; i++)
    {
        map->zones[i].type = initial_types[i];
        strncpy(map->zones[i].name, initial_names[i], sizeof(map->zones[i].name) - 1);
        map->zones[i].name[sizeof(map->zones[i].name) - 1] = '\0';

        generate_new_zone(map, i);
    }
}

void generate_new_zone(Map *map, int zone_index)
{
    if (zone_index >= 100)
        return;

    Zone *zone = &map->zones[zone_index];
    int zone_depth = zone_index * 50;

    zone->depth = zone_depth;
    zone->id = zone_index;
    zone->is_discovered = 1;

    if (zone_index == 0 || zone_index == 1)
    {
        zone->can_save = 1;
    }
    else
    {
        zone->can_save = 0;
    }

    zone->has_shop = (zone_index == 0);

    size_t creature_count;
    generate_creatures(zone_depth, zone->creatures, 4, &creature_count);
    zone->creature_count = (int)creature_count;

    for (int j = 0; j < 4; j++)
    {
        zone->destinations[j].cleared = 0;
    }
}