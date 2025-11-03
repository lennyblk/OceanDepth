#include "creature.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int next_creature_id = 1;

static void initialize_creature_stats(Creature *creature, int zone_level)
{
    assert(creature != NULL);
    assert(creature->type >= CREATURE_KRAKEN && creature->type < CREATURE_COUNT);

    // Le bonus ne s'applique qu'à partir du niveau 4
    int level_bonus_hp = 0;
    int level_bonus_atk = 0;
    int level_bonus_def = 0;
    
    if (zone_level >= 4)
    {
        level_bonus_hp = (zone_level - 3) * 8;
        level_bonus_atk = (zone_level - 3) * 1;
        level_bonus_def = (zone_level - 3) * 1;
    }

    switch (creature->type)
    {
    case CREATURE_KRAKEN:
        creature->hp_max = random_range(100, 150) + level_bonus_hp;
        creature->attack_min = 18 + level_bonus_atk;
        creature->attack_max = 28 + level_bonus_atk;
        creature->defense = 12 + level_bonus_def;
        creature->speed = 5;
        creature->special_effect = EFFECT_NONE;
        break;
    case CREATURE_SHARK:
        creature->hp_max = random_range(40, 70) + level_bonus_hp;
        creature->attack_min = 10 + level_bonus_atk;
        creature->attack_max = 16 + level_bonus_atk;
        creature->defense = 3 + level_bonus_def;
        creature->speed = 15;
        creature->special_effect = EFFECT_BLEED;
        break;
    case CREATURE_JELLYFISH:
        creature->hp_max = random_range(15, 30) + level_bonus_hp;
        creature->attack_min = 5 + level_bonus_atk;
        creature->attack_max = 10 + level_bonus_atk;
        creature->defense = 0 + level_bonus_def;
        creature->speed = 8;
        creature->special_effect = EFFECT_PARALYSIS;
        break;
    case CREATURE_SWORDFISH:
        creature->hp_max = random_range(50, 70) + level_bonus_hp;
        creature->attack_min = 12 + level_bonus_atk;
        creature->attack_max = 20 + level_bonus_atk;
        creature->defense = 5 + level_bonus_def;
        creature->speed = 12;
        creature->special_effect = EFFECT_NONE;
        break;
    case CREATURE_GIANT_CRAB:
        creature->hp_max = random_range(60, 90) + level_bonus_hp;
        creature->attack_min = 8 + level_bonus_atk;
        creature->attack_max = 14 + level_bonus_atk;
        creature->defense = 15 + level_bonus_def;
        creature->speed = 3;
        creature->special_effect = EFFECT_NONE;
        break;
    case CREATURE_COUNT:
    default:
        creature->hp_max = 0;
        creature->attack_min = 0;
        creature->attack_max = 0;
        creature->defense = 0;
        creature->speed = 0;
        creature->special_effect = EFFECT_NONE;
        break;
    }

    strncpy(creature->name, get_creature_name(creature->type), sizeof(creature->name) - 1);
    creature->name[sizeof(creature->name) - 1] = '\0';
    creature->hp_current = creature->hp_max;
    creature->effect_duration = 0;
    creature->is_alive = 1;
    creature->is_active = 1;
}

Creature create_creature(CreatureType type, int zone_level)
{
    Creature new_creature;
    new_creature.id = next_creature_id++;
    new_creature.type = type;
    initialize_creature_stats(&new_creature, zone_level);
    return new_creature;
}

void generate_creatures(int zone_depth, Creature creature_array[], size_t array_size, size_t *generated_count)
{
    assert(creature_array != NULL);
    assert(generated_count != NULL);
    assert(array_size > 0);

    size_t creatures_to_generate = random_range(1, 4);
    if (creatures_to_generate > array_size)
    {
        creatures_to_generate = array_size;
    }

    *generated_count = 0;
    int zone_level = 1 + (zone_depth / 50);
    
    // Convertir la profondeur en numéro de zone (je sais pas pourquoi on avait deux échelles différentes donc j'ai changé)
    int zone_number;
    if (zone_depth <= 0)
        zone_number = 0;  // Surface
    else if (zone_depth <= 50)
        zone_number = 1;  // Zone 1
    else if (zone_depth <= 150)
        zone_number = 2;  // Zone 2
    else
        zone_number = 3;  // Zone 3 (Abysse)


    for (size_t i = 0; i < creatures_to_generate; ++i)
    {
        CreatureType random_type;
        
        if (zone_number == 0 || zone_number == 1)
        {
            // Zone 0-1 (Surface et 0-50m) : Méduses et Poissons-Épées
            int choice = random_range(0, 1);
            random_type = (choice == 0) ? CREATURE_JELLYFISH : CREATURE_SWORDFISH;
        }
        else if (zone_number == 2)
        {
            // Zone 2 (50-150m) : Requins et Crabes Géants
            int choice = random_range(0, 1);
            random_type = (choice == 0) ? CREATURE_SHARK : CREATURE_GIANT_CRAB;
        }
        else  // zone_number == 3
        {
            // Zone 3 (150m+) : Toutes les créatures y compris le Kraken
            int random_type_value = random_range(0, CREATURE_COUNT - 1);
            random_type = (CreatureType)random_type_value;
        }

        creature_array[i] = create_creature(random_type, zone_level);
        (*generated_count)++;
    }
}

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
        return "Espadon";
    case CREATURE_GIANT_CRAB:
        return "Crabe Géant";
    case CREATURE_COUNT:
    default:
        return "Créature Inconnue";
    }
}

const char *get_creature_emoji(CreatureType type)
{
    switch (type)
    {
    case CREATURE_KRAKEN:
        return "🦑";
    case CREATURE_SHARK:
        return "🦈";
    case CREATURE_JELLYFISH:
        return "💧";
    case CREATURE_SWORDFISH:
        return "🗡️";
    case CREATURE_GIANT_CRAB:
        return "🦀";
    case CREATURE_COUNT:
    default:
        return "❓";
    }
}

void display_creature_info(const Creature *creature)
{
    assert(creature != NULL);

    printf("%s %s\n", get_creature_emoji(creature->type), creature->name);
    print_separator('-', 20);
    printf("PV: %d/%d\n", creature->hp_current, creature->hp_max);
    printf("Attaque: %d-%d\n", creature->attack_min, creature->attack_max);
    printf("Défense: %d\n", creature->defense);
    printf("Vitesse: %d\n", creature->speed);
}

int calculate_creature_level(int zone)
{
    return 1 + (zone / 50);
}

int is_creature_boss(CreatureType type)
{
    return type == CREATURE_KRAKEN;
}