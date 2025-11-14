#ifndef SHOP_H
#define SHOP_H

#include "types.h"

typedef struct {
    ItemType type;
    char name[40];
    char description[100];
    int price;
    int stock;              // -1 = stock illimité
    int min_zone_required; 
} ShopItem;

void visit_shop(Player *player, Map *map);
int is_shop_unlocked(Player *player, Map *map);
void display_shop_menu(Player *player, int zone);
void buy_item(Player *player, ShopItem *item);
void sell_items(Player *player);

void initialize_shop_items_for_zone(int zone);

void initialize_shop_items(ShopItem shop_items[], int *item_count);
int can_buy_item(Player *player, ShopItem *item);
int add_item_to_inventory(Player *player, ItemType type);
const char* get_item_rarity_color(int price);

#endif