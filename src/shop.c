#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/shop.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/menu.h"
#include "../include/ascii_art.h"

static ShopItem shops[MAX_ZONES][10];
static int shop_counts[MAX_ZONES];
static int shop_initialized[MAX_ZONES];

void initialize_shop_items_for_zone(int zone)
{
    if (zone < 0 || zone >= MAX_ZONES)
        return;
    if (shop_initialized[zone])
        return;

    shop_counts[zone] = 0;

    // Zone 0:
    if (zone == 0)
    {
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_OXYGEN_CAPSULE,
            .name = "Capsule d'oxygene",
            .description = "Restaure 30 points d'oxygene instantanement",
            .price = 10,
            .stock = -1,
            .min_zone_required = 0,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_HEALTH_KIT,
            .name = "Kit medical",
            .description = "Restaure 40 points de vie",
            .price = 20,
            .stock = -1,
            .min_zone_required = 0,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_STIMULANT,
            .name = "Stimulant",
            .description = "Reduit la fatigue et boost temporairement",
            .price = 30,
            .stock = 10,
            .min_zone_required = 0,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_ANTIDOTE,
            .name = "Antidote",
            .description = "Soigne le poison et la paralysie",
            .price = 25,
            .stock = 5,
            .min_zone_required = 0,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
    }
    // Zones 1-3:
    else if (zone >= 1 && zone <= 3)
    {
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_OXYGEN_CAPSULE,
            .name = "Capsule d'oxygene (moyen)",
            .description = "Restaure 30 points d'oxygene",
            .price = 15,
            .stock = 20,
            .min_zone_required = zone,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_HEALTH_KIT,
            .name = "Kit medical",
            .description = "Restaure 40 points de vie",
            .price = 25,
            .stock = 15,
            .min_zone_required = zone,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_STIMULANT,
            .name = "Stimulant",
            .description = "Reduit la fatigue et boost temporairement",
            .price = 35,
            .stock = 12,
            .min_zone_required = zone,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_ANTIDOTE,
            .name = "Antidote",
            .description = "Soigne le poison et la paralysie",
            .price = 30,
            .stock = 10,
            .min_zone_required = zone,
            .hp_restore_override = 0,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
    }
    // Zones >=4: 
    else
    {
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_OXYGEN_CAPSULE,
            .name = "Capsule d'oxygene (rare)",
            .description = "Restaure 50 points d'oxygene - formulation rare",
            .price = 30,
            .stock = 5,
            .min_zone_required = zone,
            .hp_restore_override = 0,
            .oxygen_restore_override = 50,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_HEALTH_KIT,
            .name = "Kit medical (ameliore)",
            .description = "Restaure 80 points de vie",
            .price = 55,
            .stock = 4,
            .min_zone_required = zone,
            .hp_restore_override = 80,
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_STIMULANT,
            .name = "Stimulant (puissant)",
            .description = "Reduit la fatigue et booste fortement",
            .price = 70,
            .stock = 3,
            .min_zone_required = zone,
            .hp_restore_override = 0,
            .oxygen_restore_override = 20, 
            .fatigue_reduce_override = 40,
            .removes_effect_override = 0};
        shops[zone][shop_counts[zone]++] = (ShopItem){
            .type = ITEM_ANTIDOTE,
            .name = "Antidote (concentre)",
            .description = "Soigne le poison et la paralysie plus efficacement",
            .price = 60,
            .stock = 3,
            .min_zone_required = zone,
            .hp_restore_override = 20, 
            .oxygen_restore_override = 0,
            .fatigue_reduce_override = 0,
            .removes_effect_override = 1};
    }

    shop_initialized[zone] = 1;
}

int is_shop_unlocked(Player *player, Map *map)
{
    if (player == NULL || map == NULL)
        return 0;

    int zone = player->current_zone;
    if (zone <= 0)
        return 1;

    for (int d = 0; d < 4; d++)
    {
        if (!map->zones[zone].destinations[d].cleared)
            return 0;
    }
    return 1;
}

const char *get_item_rarity_color(int price)
{
    if (price < 20)
        return COLOR_GREEN;
    if (price < 35)
        return COLOR_CYAN;
    if (price < 50)
        return COLOR_YELLOW;
    return COLOR_MAGENTA;
}

void visit_shop(Player *player, Map *map)
{
    if (!is_shop_unlocked(player, map))
    {
        clear_screen();
        printf(COLOR_YELLOW "🔒 Le marché n'est pas accessible ici.\n" COLOR_RESET);
        printf("Vous devez d'abord nettoyer toutes les destinations de cette zone pour ouvrir le marché.\n");
        pause_screen();
        return;
    }

    int zone = player->current_zone;
    initialize_shop_items_for_zone(zone);
    display_shop_menu(player, zone);
}

void display_shop_menu(Player *player, int zone)
{
    // use persistent per-zone shop
    if (zone < 0 || zone >= MAX_ZONES)
    {
        print_error("Zone de boutique invalide.");
        return;
    }

    ShopItem *shop_items = shops[zone];
    int item_count = shop_counts[zone];

    while (1)
    {
        clear_screen();
        display_marchand_ascii();
        printf(COLOR_CYAN COLOR_BOLD);
        printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                           🏪 MARCHE DU PLONGEUR - ZONE %d 🏪                         ║\n", zone);
        printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
        printf(COLOR_RESET);

        printf("\n" COLOR_YELLOW "💰 Vos perles: %d" COLOR_RESET " | ", player->pearls);
        printf(COLOR_CYAN "🎒 Inventaire: %d/4" COLOR_RESET "\n\n", player->inventory_count);

        print_separator('-', 90);
        printf(COLOR_BOLD "Articles disponibles:\n" COLOR_RESET);
        print_separator('-', 90);

        for (int i = 0; i < item_count; i++)
        {
            ShopItem *item = &shop_items[i];

            if (player->zones_unlocked < item->min_zone_required)
            {
                printf(COLOR_BOLD "%d. [VERROUILLE - Zone %d requise]\n" COLOR_RESET,
                       i + 1, item->min_zone_required);
                continue;
            }

            const char *color = get_item_rarity_color(item->price);

            printf("%s%d. %-35s" COLOR_RESET " | ", color, i + 1, item->name);
            printf(COLOR_YELLOW "💰 %d perles" COLOR_RESET, item->price);

            if (item->stock > 0)
            {
                printf(" | " COLOR_GREEN "Stock: %d" COLOR_RESET, item->stock);
            }
            else
            {
                printf(" | " COLOR_RED "Rupture de stock" COLOR_RESET);
            }

            printf("\n");
        }

        print_separator('-', 90);
        printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour\n");

        printf("\n" COLOR_BOLD "Que voulez-vous faire ? (0 pour annuler): " COLOR_RESET);

        char choice = get_char_input();

        if (choice == '0')
        {
            return;
        }

        int item_index = choice - '1';

        if (item_index >= 0 && item_index < item_count)
        {
            ShopItem *selected_item = &shop_items[item_index];

            printf("\nVous avez choisi: " COLOR_CYAN "%s" COLOR_RESET "\n", selected_item->name);
            printf("Description: %s\n", selected_item->description);
            printf(COLOR_YELLOW "Prix: %d perles\n" COLOR_RESET, selected_item->price);

            if (selected_item->stock > 0)
            {
                printf(COLOR_GREEN "Stock disponible: %d\n" COLOR_RESET, selected_item->stock);
            }
            else
            {
                printf(COLOR_RED "Rupture de stock !\n" COLOR_RESET);
            }

            printf("\n" COLOR_BOLD "Voulez-vous acheter cet article ? (o/n): " COLOR_RESET);
            char confirm = get_char_input();

            if (confirm == 'o' || confirm == 'O')
            {
                buy_item(player, selected_item);
            }
            else
            {
                printf(COLOR_YELLOW "Retour au menu principal du marché.\n" COLOR_RESET);
            }
        }
        else
        {
            printf(COLOR_RED "Choix invalide. Veuillez reessayer.\n" COLOR_RESET);
        }

        pause_screen();
    }
}

// Check whether player can buy the shop item (money, inventory space, stock)
int can_buy_item(Player *player, ShopItem *item)
{
    if (player == NULL || item == NULL)
        return 0;

    if (player->pearls < item->price)
    {
        printf(COLOR_RED "\n❌ Vous n'avez pas assez de perles !\n" COLOR_RESET);
        printf("   Il vous faut %d perles mais vous n'en avez que %d.\n",
               item->price, player->pearls);
        return 0;
    }

    if (player->inventory_count >= 4)
    {
        printf(COLOR_RED "\n❌ Votre inventaire est plein !\n" COLOR_RESET);
        printf("   Vendez ou utilisez des objets d'abord.\n");
        return 0;
    }

    if (item->stock == 0)
    {
        printf(COLOR_RED "\n❌ Cet article est en rupture de stock !\n" COLOR_RESET);
        return 0;
    }

    return 1;
}

// Modified signature: now receives ShopItem* to preserve overrides
int add_item_to_inventory(Player *player, ShopItem *shop_item)
{
    if (player == NULL || shop_item == NULL)
        return 0;

    if (player->inventory_count >= 4)
        return 0;

    Item new_item;
    memset(&new_item, 0, sizeof(new_item));
    new_item.type = shop_item->type;
    new_item.quantity = 1;

    // Use overrides when fournis (non-zero), sinon valeurs par défaut
    switch (shop_item->type)
    {
    case ITEM_OXYGEN_CAPSULE:
        strcpy(new_item.name, shop_item->name);
        new_item.oxygen_restore = (shop_item->oxygen_restore_override > 0) ? shop_item->oxygen_restore_override : 30;
        new_item.hp_restore = (shop_item->hp_restore_override > 0) ? shop_item->hp_restore_override : 0;
        new_item.fatigue_reduce = (shop_item->fatigue_reduce_override > 0) ? shop_item->fatigue_reduce_override : 0;
        new_item.removes_effect = shop_item->removes_effect_override;
        break;

    case ITEM_HEALTH_KIT:
        strcpy(new_item.name, shop_item->name);
        new_item.hp_restore = (shop_item->hp_restore_override > 0) ? shop_item->hp_restore_override : 40;
        new_item.oxygen_restore = (shop_item->oxygen_restore_override > 0) ? shop_item->oxygen_restore_override : 0;
        new_item.fatigue_reduce = (shop_item->fatigue_reduce_override > 0) ? shop_item->fatigue_reduce_override : 0;
        new_item.removes_effect = shop_item->removes_effect_override;
        break;

    case ITEM_STIMULANT:
        strcpy(new_item.name, shop_item->name);
        new_item.fatigue_reduce = (shop_item->fatigue_reduce_override > 0) ? shop_item->fatigue_reduce_override : 20;
        new_item.oxygen_restore = (shop_item->oxygen_restore_override > 0) ? shop_item->oxygen_restore_override : 10;
        new_item.hp_restore = (shop_item->hp_restore_override > 0) ? shop_item->hp_restore_override : 0;
        new_item.removes_effect = shop_item->removes_effect_override;
        break;

    case ITEM_ANTIDOTE:
        strcpy(new_item.name, shop_item->name);
        new_item.removes_effect = (shop_item->removes_effect_override > 0) ? shop_item->removes_effect_override : 1;
        new_item.hp_restore = (shop_item->hp_restore_override > 0) ? shop_item->hp_restore_override : 10;
        new_item.oxygen_restore = (shop_item->oxygen_restore_override > 0) ? shop_item->oxygen_restore_override : 0;
        new_item.fatigue_reduce = (shop_item->fatigue_reduce_override > 0) ? shop_item->fatigue_reduce_override : 0;
        break;

    default:
        return 0;
    }

    player->inventory[player->inventory_count] = new_item;
    player->inventory_count++;

    return 1;
}

// Prompt and perform purchase; decrement shop stock (if limited) and persist per-zone inventory
void buy_item(Player *player, ShopItem *item)
{
    if (player == NULL || item == NULL)
        return;

    clear_screen();

    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
    printf(COLOR_BOLD "   ACHAT: %s\n" COLOR_RESET, item->name);
    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);

    printf("\n%s\n", item->description);
    printf(COLOR_YELLOW "\n💰 Prix: %d perles" COLOR_RESET "\n", item->price);
    printf(COLOR_CYAN "🎒 Place dans l'inventaire: %d/4" COLOR_RESET "\n\n", player->inventory_count);

    if (!can_buy_item(player, item))
    {
        pause_screen();
        return;
    }

    printf(COLOR_BOLD "Confirmer l'achat ? (o/n): " COLOR_RESET);
    char confirm = get_char_input();

    if (confirm == 'o' || confirm == 'O')
    {
        player->pearls -= item->price;

        // Passer le ShopItem pour préserver les overrides
        if (add_item_to_inventory(player, item))
        {
            printf(COLOR_GREEN "\n✅ Achat reussi !\n" COLOR_RESET);
            printf("Vous avez achete: %s\n", item->name);
            printf(COLOR_YELLOW "💰 Il vous reste %d perles\n" COLOR_RESET, player->pearls);
            printf(COLOR_CYAN "🎒 Inventaire: %d/4\n" COLOR_RESET, player->inventory_count);

            if (item->stock > 0)
            {
                item->stock--;
            }
        }
        else
        {
            player->pearls += item->price;
            printf(COLOR_RED "\n❌ Erreur lors de l'ajout a l'inventaire !\n" COLOR_RESET);
        }
    }
    else
    {
        printf(COLOR_YELLOW "\n❌ Achat annule.\n" COLOR_RESET);
    }

    pause_screen();
}

// Sell items from player's inventory (unchanged logic, kept concise)
void sell_items(Player *player)
{
    if (player == NULL)
        return;

    if (player->inventory_count == 0)
    {
        printf(COLOR_YELLOW "\n📦 Votre inventaire est vide !\n" COLOR_RESET);
        pause_screen();
        return;
    }

    clear_screen();

    printf(COLOR_CYAN COLOR_BOLD);
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    💸 VENTE D'OBJETS 💸                            ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    printf(COLOR_RESET);

    printf("\n" COLOR_YELLOW "💰 Vos perles: %d" COLOR_RESET "\n\n", player->pearls);

    printf(COLOR_BOLD "Votre inventaire:\n" COLOR_RESET);
    print_separator('-', 70);

    for (int i = 0; i < player->inventory_count; i++)
    {
        Item *item = &player->inventory[i];
        int sell_price = 0;

        switch (item->type)
        {
        case ITEM_OXYGEN_CAPSULE:
            sell_price = 7;
            break;
        case ITEM_HEALTH_KIT:
            sell_price = 12;
            break;
        case ITEM_STIMULANT:
            sell_price = 17;
            break;
        case ITEM_ANTIDOTE:
            sell_price = 15;
            break;
        default:
            sell_price = 5;
            break;
        }

        printf(COLOR_GREEN "%d." COLOR_RESET " %-25s x%d | ",
               i + 1, item->name, item->quantity);
        printf(COLOR_YELLOW "💰 %d perles/unite" COLOR_RESET "\n", sell_price);
    }

    print_separator('-', 70);
    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour\n");

    printf("\n" COLOR_BOLD "Quel objet voulez-vous vendre ? (0 pour annuler): " COLOR_RESET);

    char choice = get_char_input();

    if (choice == '0')
    {
        return;
    }

    int item_index = choice - '1';

    if (item_index >= 0 && item_index < player->inventory_count)
    {
        Item *item = &player->inventory[item_index];

        int sell_price = 0;
        switch (item->type)
        {
        case ITEM_OXYGEN_CAPSULE:
            sell_price = 7;
            break;
        case ITEM_HEALTH_KIT:
            sell_price = 12;
            break;
        case ITEM_STIMULANT:
            sell_price = 17;
            break;
        case ITEM_ANTIDOTE:
            sell_price = 15;
            break;
        default:
            sell_price = 5;
            break;
        }

        printf("\n" COLOR_BOLD "Combien d'unites vendre ? (max %d): " COLOR_RESET, item->quantity);
        int quantity = get_int_input(1, item->quantity);

        if (quantity > 0 && quantity <= item->quantity)
        {
            int total = sell_price * quantity;
            player->pearls += total;
            item->quantity -= quantity;

            printf(COLOR_GREEN "\n✅ Vente reussie !\n" COLOR_RESET);
            printf("Vous avez vendu %dx %s pour %d perles\n", quantity, item->name, total);
            printf(COLOR_YELLOW "💰 Vous avez maintenant %d perles\n" COLOR_RESET, player->pearls);

            if (item->quantity == 0)
            {
                for (int i = item_index; i < player->inventory_count - 1; i++)
                {
                    player->inventory[i] = player->inventory[i + 1];
                }
                player->inventory_count--;
            }
        }
        else
        {
            printf(COLOR_RED "\n❌ Quantite invalide !\n" COLOR_RESET);
        }
    }
    else
    {
        print_error("Objet invalide !");
    }

    pause_screen();
}