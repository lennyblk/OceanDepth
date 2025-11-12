#ifndef GEAR_H
#define GEAR_H

#include "types.h"

void manage_equipment(Player *player);

void equip_weapon_from_inventory(Player *player);
void equip_armor_from_inventory(Player *player);

void show_available_weapons(Player *player);
void show_available_armors(Player *player);

#endif
