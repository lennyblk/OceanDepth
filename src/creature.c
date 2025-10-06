#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/creature.h"
#include "../include/types.h"

// ID unique
static int next_creature_id = 1;

const char *get_creature_name(CreatureType type)
{
    switch (type)
    {
    case CREATURE_KRAKEN:
        return "Kraken";
    case CREATURE_SHARK:
        return "Requin-Tigre";
    case CREATURE_JELLYFISH:
        return "Méduse Bleue";
    case CREATURE_SWORDFISH:
        return "Poisson-Épée";
    case CREATURE_GIANT_CRAB:
        return "Crabe Géant";
    default:
        return "Créature Inconnue";
    }
}

int calculate_creature_level(int zone)
{
    return 1 + (zone / 50);
}

void initialize_creature_stats(Creature *creature)
{
    if (creature == NULL)
        return;
    int zone_multiplier = 1 + (creature->id / 10);

    switch (creature->type)
    {
    case CREATURE_KRAKEN:
        creature->hp_max = 120 + (rand() % 61) + (zone_multiplier * 10);
        break;

    case CREATURE_SHARK:
        creature->hp_max = 60 + (rand() % 41) + (zone_multiplier * 8);
        break;

    case CREATURE_JELLYFISH:
        creature->hp_max = 20 + (rand() % 21) + (zone_multiplier * 3);
        break;

    case CREATURE_SWORDFISH:
        creature->hp_max = 70 + (rand() % 21) + (zone_multiplier * 6);
        break;

    case CREATURE_GIANT_CRAB:
        creature->hp_max = 80 + (rand() % 41) + (zone_multiplier * 8);
        break;

    default:
        creature->hp_max = 50;
        creature->attack_min = 10;
        creature->attack_max = 15;
        creature->defense = 5;
        creature->speed = 5;
        creature->special_effect = EFFECT_NONE;
        strcpy(creature->name, "Créature Inconnue");
        break;
    }

    creature->hp_current = creature->hp_max;
    creature->effect_duration = 0;
    creature->is_alive = 1;
    creature->is_active = 1;
}

Creature create_creature(CreatureType type, int zone_level)
{
    Creature creature;

    creature.id = next_creature_id++;
    creature.type = type;

    initialize_creature_stats(&creature);

    return creature;
}

int get_creature_spawn_chance(int zone, CreatureType type)
{
    switch (type)
    {
    case CREATURE_KRAKEN: // +
        if (zone < 200)
            return 0;
        if (zone < 300)
            return 10;
        return 25;

    case CREATURE_SHARK: // +
        if (zone < 100)
            return 40;
        if (zone < 200)
            return 50;
        return 30;

    case CREATURE_JELLYFISH: // -
        if (zone < 100)
            return 60;
        if (zone < 200)
            return 40;
        return 20;

    case CREATURE_SWORDFISH: // +
        if (zone < 100)
            return 30;
        if (zone < 200)
            return 50;
        return 35;

    case CREATURE_GIANT_CRAB: // +
        if (zone < 100)
            return 20;
        if (zone < 200)
            return 40;
        return 60;

    default:
        return 0;
    }
}