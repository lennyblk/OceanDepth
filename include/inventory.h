#ifndef INVENTORY_H
#define INVENTORY_H

#include "types.h"

// Fonctions principales de gestion d'inventaire
void manage_inventory(Player *player);
void manage_equipment(Player *player);

// Fonctions d'actions sur l'inventaire
void use_inventory_item(Player *player);
void examine_inventory_item(Player *player);
void drop_inventory_item(Player *player);

// Fonctions utilitaires d'inventaire
int calculate_inventory_weight(Player *player);
void remove_item_from_inventory(Player *player, int index);

#endif
