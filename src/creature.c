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

    int base_hp_min, base_hp_max, base_atk_min, base_atk_max, base_def;

    switch (creature->type)
    {
    case CREATURE_KRAKEN:
        base_hp_min = 120;
        base_hp_max = 180;
        base_atk_min = 35;
        base_atk_max = 45;
        base_def = 8;
        creature->speed = 5;
        creature->special_effect = EFFECT_NONE;
        break;
    case CREATURE_SHARK:
        base_hp_min = 50;
        base_hp_max = 75;
        base_atk_min = 14;
        base_atk_max = 20;
        base_def = 2;
        creature->speed = 15;
        creature->special_effect = EFFECT_BLEED;
        break;
    case CREATURE_JELLYFISH:
        base_hp_min = 20;
        base_hp_max = 35;
        base_atk_min = 10;
        base_atk_max = 15;
        base_def = 0;
        creature->speed = 8;
        creature->special_effect = EFFECT_PARALYSIS;
        break;
    case CREATURE_SWORDFISH:
        base_hp_min = 40;
        base_hp_max = 60;
        base_atk_min = 25;
        base_atk_max = 30;
        base_def = 1;
        creature->speed = 12;
        creature->special_effect = EFFECT_NONE;
        break;
    case CREATURE_GIANT_CRAB:
        base_hp_min = 70;
        base_hp_max = 100;
        base_atk_min = 20;
        base_atk_max = 30;
        base_def = 10;
        creature->speed = 3;
        creature->special_effect = EFFECT_NONE;
        break;
    default:
        base_hp_min = 0;
        base_hp_max = 0;
        base_atk_min = 0;
        base_atk_max = 0;
        base_def = 0;
        creature->speed = 0;
        creature->special_effect = EFFECT_NONE;
        break;
    }

    float multiplier = 1.0f;
    if (zone_level > 3)
    {
        multiplier = 1.0f + ((zone_level - 3) * 0.10f);
    }

    creature->hp_max = (int)(random_range(base_hp_min, base_hp_max) * multiplier);
    creature->attack_min = (int)(base_atk_min * multiplier);
    creature->attack_max = (int)(base_atk_max * multiplier);
    creature->defense = (int)(base_def * multiplier);

    strncpy(creature->name, get_creature_name(creature->type), sizeof(creature->name) - 1);
    creature->name[sizeof(creature->name) - 1] = '\0';
    creature->hp_current = creature->hp_max;
    creature->effect_duration = 0;
    creature->is_alive = 1;
    creature->is_active = 1;

    // AJOUTÉ
    creature->is_pacified = 0;
    creature->pacified_turns_left = 0;
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
        zone_number = 0; // Surface
    else if (zone_depth <= 50)
        zone_number = 1; // Zone 1
    else if (zone_depth <= 150)
        zone_number = 2; // Zone 2
    else
        zone_number = 3; // Zone 3 (Abysse)

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
        else // zone_number == 3
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