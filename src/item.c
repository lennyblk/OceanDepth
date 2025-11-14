#include <string.h>
#include "../include/types.h"

// Retourne les stats par défaut d'un type d'item consommable
Item get_item_default_stats(ItemType type)
{
    Item item;
    memset(&item, 0, sizeof(item));
    item.type = type;
    item.quantity = 1;

    switch (type)
    {
    case ITEM_OXYGEN_CAPSULE:
        strcpy(item.name, "Capsule d'oxygene");
        item.oxygen_restore = 30;
        item.hp_restore = 0;
        item.fatigue_reduce = 0;
        item.removes_effect = 0;
        break;

    case ITEM_HEALTH_KIT:
        strcpy(item.name, "Kit medical");
        item.hp_restore = 40;
        item.oxygen_restore = 0;
        item.fatigue_reduce = 0;
        item.removes_effect = 0;
        break;

    case ITEM_STIMULANT:
        strcpy(item.name, "Stimulant");
        item.fatigue_reduce = 20;
        item.oxygen_restore = 10;
        item.hp_restore = 0;
        item.removes_effect = 0;
        break;

    case ITEM_ANTIDOTE:
        strcpy(item.name, "Antidote");
        item.removes_effect = 1;
        item.hp_restore = 10;
        item.oxygen_restore = 0;
        item.fatigue_reduce = 0;
        break;

    default:
        strcpy(item.name, "Objet inconnu");
        break;
    }

    return item;
}

// Retourne un item amélioré pour les zones profondes (4+)
Item get_item_enhanced_stats(ItemType type)
{
    Item item = get_item_default_stats(type);
    
    switch (type)
    {
    case ITEM_OXYGEN_CAPSULE:
        strcpy(item.name, "Capsule d'oxygene (rare)");
        item.oxygen_restore = 50; 
        break;

    case ITEM_HEALTH_KIT:
        strcpy(item.name, "Kit medical (ameliore)");
        item.hp_restore = 80;  
        break;

    case ITEM_STIMULANT:
        strcpy(item.name, "Stimulant (puissant)");
        item.fatigue_reduce = 40;  
        item.oxygen_restore = 20;  
        break;

    case ITEM_ANTIDOTE:
        strcpy(item.name, "Antidote (concentre)");
        item.hp_restore = 20;  
        item.removes_effect = 1;
        break;

    default:
        break;
    }

    return item;
}
