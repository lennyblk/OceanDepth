#include <string.h>
#include "../include/types.h"

// Définitions statiques pour chaque WeaponType
Weapon get_weapon_stats(WeaponType type)
{
    Weapon w;
    memset(&w, 0, sizeof(w));
    switch (type)
    {
    case WEAPON_RUSTY_HARPOON:
        w.type = WEAPON_RUSTY_HARPOON;
        strcpy(w.name, "Harpon rouille");
        w.attack_min = 4;
        w.attack_max = 7;
        w.oxygen_cost_per_attack = 2;
        w.defense_ignore = 0;
        w.rarity = RARITY_COMMON;
        break;
    case WEAPON_STEEL_HARPOON:
        w.type = WEAPON_STEEL_HARPOON;
        strcpy(w.name, "Harpon en acier");
        w.attack_min = 8;
        w.attack_max = 12;
        w.oxygen_cost_per_attack = 3;
        w.defense_ignore = 2;
        w.rarity = RARITY_RARE;
        break;
    case WEAPON_ELECTRIC_HARPOON:
        w.type = WEAPON_ELECTRIC_HARPOON;
        strcpy(w.name, "Harpon electrique");
        w.attack_min = 12;
        w.attack_max = 18;
        w.oxygen_cost_per_attack = 4;
        w.defense_ignore = 4;
        w.rarity = RARITY_EPIC;
        break;
    case WEAPON_LASER_HARPOON:
        w.type = WEAPON_LASER_HARPOON;
        strcpy(w.name, "Harpon laser");
        w.attack_min = 18;
        w.attack_max = 26;
        w.oxygen_cost_per_attack = 6;
        w.defense_ignore = 6;
        w.rarity = RARITY_LEGENDARY;
        break;
    default:
        // fallback
        w.type = WEAPON_RUSTY_HARPOON;
        strcpy(w.name, "Harpon inconnu");
        w.attack_min = 5;
        w.attack_max = 8;
        w.oxygen_cost_per_attack = 3;
        w.defense_ignore = 0;
        w.rarity = RARITY_COMMON;
        break;
    }
    return w;
}
