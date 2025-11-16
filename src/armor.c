#include <string.h>
#include "../include/types.h"

// Définitions statiques pour chaque ArmorType
Armor get_armor_stats(ArmorType type)
{
    Armor a;
    memset(&a, 0, sizeof(a));
    switch (type)
    {
    case ARMOR_BASIC_SUIT:
        a.type = ARMOR_BASIC_SUIT;
        strcpy(a.name, "Combinaison basique");
        a.defense = 8;
        a.oxygen_cost_per_turn = 0;
        a.rarity = RARITY_COMMON;
        break;
    case ARMOR_REINFORCED_SUIT:
        a.type = ARMOR_REINFORCED_SUIT;
        strcpy(a.name, "Combinaison renforcee");
        a.defense = 12;
        a.oxygen_cost_per_turn = 1;
        a.rarity = RARITY_RARE;
        break;
    case ARMOR_INSULATED_SUIT:
        a.type = ARMOR_INSULATED_SUIT;
        strcpy(a.name, "Combinaison isolee");
        a.defense = 18;
        a.oxygen_cost_per_turn = 2;
        a.rarity = RARITY_EPIC;
        break;
    case ARMOR_ADVANCED_SUIT:
        a.type = ARMOR_ADVANCED_SUIT;
        strcpy(a.name, "Combinaison avancee");
        a.defense = 24;
        a.oxygen_cost_per_turn = 3;
        a.rarity = RARITY_LEGENDARY;
        break;
    default:
        a.type = ARMOR_BASIC_SUIT;
        strcpy(a.name, "Combinaison inconnue");
        a.defense = 2;
        a.oxygen_cost_per_turn = 0;
        a.rarity = RARITY_COMMON;
        break;
    }
    return a;
}
