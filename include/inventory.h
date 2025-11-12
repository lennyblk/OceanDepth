#ifndef INVENTORY_H
#define INVENTORY_H

#include "types.h"

void manage_inventory(Player *player);

void use_inventory_item(Player *player);
void examine_inventory_item(Player *player);
void drop_inventory_item(Player *player);

int calculate_inventory_weight(Player *player);
void remove_item_from_inventory(Player *player, int index);

int calculate_inventory_weight(Player *player);
void remove_item_from_inventory(Player *player, int index);

#endif
