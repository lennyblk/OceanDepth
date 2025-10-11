#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/shop.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/menu.h"
#include "../include/ascii_art.h"


void initialize_shop_items(ShopItem shop_items[], int *item_count) {
    *item_count = 0;
    
    // ITEM_OXYGEN_CAPSULE
    shop_items[*item_count] = (ShopItem){
        .type = ITEM_OXYGEN_CAPSULE,
        .name = "Capsule d'oxygene",
        .description = "Restaure 30 points d'oxygene instantanement",
        .price = 15,
        .stock = -1,
        .min_zone_required = 0
    };
    (*item_count)++;
    
    // ITEM_HEALTH_KIT
    shop_items[*item_count] = (ShopItem){
        .type = ITEM_HEALTH_KIT,
        .name = "Kit medical",
        .description = "Restaure 40 points de vie",
        .price = 25,
        .stock = -1,
        .min_zone_required = 0
    };
    (*item_count)++;
    
    // ITEM_STIMULANT
    shop_items[*item_count] = (ShopItem){
        .type = ITEM_STIMULANT,
        .name = "Stimulant",
        .description = "Reduit la fatigue et boost temporairement",
        .price = 35,
        .stock = -1,
        .min_zone_required = 1
    };
    (*item_count)++;
    
    // ITEM_ANTIDOTE
    shop_items[*item_count] = (ShopItem){
        .type = ITEM_ANTIDOTE,
        .name = "Antidote",
        .description = "Soigne le poison et la paralysie",
        .price = 30,
        .stock = -1,
        .min_zone_required = 1
    };
    (*item_count)++;
}

int is_shop_unlocked(Player *player, Map *map) {
    (void)map;
    (void)player;
    
    // Option 1 : Toujours débloqué (recommandé pour la zone de départ)
   // return 1;
    
    // Option 2 : Débloqué si au moins une zone est accessible
    return (player->zones_unlocked >= 1);
}

const char* get_item_rarity_color(int price) {
    if (price < 20) return COLOR_GREEN;
    if (price < 35) return COLOR_CYAN;
    if (price < 50) return COLOR_YELLOW;
    return COLOR_MAGENTA;
}

void visit_shop(Player *player) {
    display_shop_menu(player);
}

void display_shop_menu(Player *player) {
    ShopItem shop_items[10];
    int item_count = 0;
    initialize_shop_items(shop_items, &item_count);
    
    while (1) {
        clear_screen();
        display_marchand_ascii();
        printf(COLOR_CYAN COLOR_BOLD);
        printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                           🏪 MARCHE DU PLONGEUR 🏪                                 ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
        printf(COLOR_RESET);
        
        printf("\n" COLOR_YELLOW "💰 Vos perles: %d" COLOR_RESET " | ", player->pearls);
        printf(COLOR_CYAN "🎒 Inventaire: %d/4" COLOR_RESET "\n\n", player->inventory_count);
        
        print_separator('-', 90);
        printf(COLOR_BOLD "Articles disponibles:\n" COLOR_RESET);
        print_separator('-', 90);
        
        for (int i = 0; i < item_count; i++) {
            ShopItem *item = &shop_items[i];
            
            if (player->zones_unlocked < item->min_zone_required) {
                printf(COLOR_BOLD "%d. [VERROUILLE - Zone %d requise]\n" COLOR_RESET, 
                       i + 1, item->min_zone_required);
                continue;
            }
            
            const char* color = get_item_rarity_color(item->price);
            
            printf("%s%d. %-25s" COLOR_RESET " | ", color, i + 1, item->name);
            printf(COLOR_YELLOW "💰 %d perles" COLOR_RESET, item->price);
            
            if (item->stock > 0) {
                printf(" | " COLOR_CYAN "Stock: %d" COLOR_RESET, item->stock);
            } else if (item->stock == -1) {
                printf(" | " COLOR_GREEN "♾️  Stock illimite" COLOR_RESET);
            }
            
            printf("\n   └─ %s\n", item->description);
        }
        
        print_separator('-', 90);
        printf("\n" COLOR_GREEN "V." COLOR_RESET " 💸 Vendre des objets\n");
        printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Quitter le marche\n");
        
        printf("\n" COLOR_BOLD "Que voulez-vous acheter ? (0 pour quitter): " COLOR_RESET);
        
        char choice = get_char_input();
        
        if (choice == '0') {
            printf(COLOR_CYAN "\n👋 Merci de votre visite ! Revenez quand vous voulez.\n" COLOR_RESET);
            pause_screen();
            return;
        } else if (choice == 'v' || choice == 'V') {
            sell_items(player);
        } else if (choice >= '1' && choice <= '9') {
            int item_index = choice - '1';
            
            if (item_index < item_count) {
                ShopItem *selected_item = &shop_items[item_index];
                
                if (player->zones_unlocked < selected_item->min_zone_required) {
                    printf(COLOR_RED "\n⚠️  Cet article necessite d'avoir debloque la zone %d !\n" COLOR_RESET, 
                           selected_item->min_zone_required);
                    pause_screen();
                    continue;
                }
                
                buy_item(player, selected_item);
            } else {
                print_error("Article invalide !");
                pause_screen();
            }
        } else {
            print_error("Choix invalide !");
            pause_screen();
        }
    }
}

int can_buy_item(Player *player, ShopItem *item) {
    if (player->pearls < item->price) {
        printf(COLOR_RED "\n❌ Vous n'avez pas assez de perles !\n" COLOR_RESET);
        printf("   Il vous faut %d perles mais vous n'en avez que %d.\n", 
               item->price, player->pearls);
        return 0;
    }
    
    if (player->inventory_count >= 4) {
        printf(COLOR_RED "\n❌ Votre inventaire est plein !\n" COLOR_RESET);
        printf("   Vendez ou utilisez des objets d'abord.\n");
        return 0;
    }
    
    if (item->stock == 0) {
        printf(COLOR_RED "\n❌ Cet article est en rupture de stock !\n" COLOR_RESET);
        return 0;
    }
    
    return 1;
}

int add_item_to_inventory(Player *player, ItemType type) {
    
    Item new_item;
    new_item.type = type;
    new_item.quantity = 1;
    
    switch (type) {
        case ITEM_OXYGEN_CAPSULE:
            strcpy(new_item.name, "Capsule d'oxygene");
            new_item.oxygen_restore = 30;
            new_item.hp_restore = 0;
            new_item.fatigue_reduce = 0;
            new_item.removes_effect = 0;
            break;
            
        case ITEM_HEALTH_KIT:
            strcpy(new_item.name, "Kit medical");
            new_item.hp_restore = 40;
            new_item.oxygen_restore = 0;
            new_item.fatigue_reduce = 0;
            new_item.removes_effect = 0;
            break;
            
        case ITEM_STIMULANT:
            strcpy(new_item.name, "Stimulant");
            new_item.fatigue_reduce = 20;
            new_item.oxygen_restore = 10;
            new_item.hp_restore = 0;
            new_item.removes_effect = 0;
            break;
            
        case ITEM_ANTIDOTE:
            strcpy(new_item.name, "Antidote");
            new_item.removes_effect = 1;
            new_item.hp_restore = 10;
            new_item.oxygen_restore = 0;
            new_item.fatigue_reduce = 0;
            break;
            
        default:
            return 0;
    }
    
    player->inventory[player->inventory_count] = new_item;
    player->inventory_count++;
    
    return 1;
}

void buy_item(Player *player, ShopItem *item) {
    clear_screen();
    
    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
    printf(COLOR_BOLD "   ACHAT: %s\n" COLOR_RESET, item->name);
    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
    
    printf("\n%s\n", item->description);
    printf(COLOR_YELLOW "\n💰 Prix: %d perles" COLOR_RESET "\n", item->price);
    printf(COLOR_CYAN "🎒 Place dans l'inventaire: %d/4" COLOR_RESET "\n\n", player->inventory_count);
    
    
    if (!can_buy_item(player, item)) {
        pause_screen();
        return;
    }
    
    printf(COLOR_BOLD "Confirmer l'achat ? (o/n): " COLOR_RESET);
    char confirm = get_char_input();
    
    if (confirm == 'o' || confirm == 'O') {
        player->pearls -= item->price;
                
        if (add_item_to_inventory(player, item->type)) {
            printf(COLOR_GREEN "\n✅ Achat reussi !\n" COLOR_RESET);
            printf("Vous avez achete: %s\n", item->name);
            printf(COLOR_YELLOW "💰 Il vous reste %d perles\n" COLOR_RESET, player->pearls);
            printf(COLOR_CYAN "🎒 Inventaire: %d/4\n" COLOR_RESET, player->inventory_count);
            
            if (item->stock > 0) {
                item->stock--;
            }
        } else {
            player->pearls += item->price;
            printf(COLOR_RED "\n❌ Erreur lors de l'ajout a l'inventaire !\n" COLOR_RESET);
        }
    } else {
        printf(COLOR_YELLOW "\n❌ Achat annule.\n" COLOR_RESET);
    }
    
    pause_screen();
}

void sell_items(Player *player) {
    if (player->inventory_count == 0) {
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
    
    for (int i = 0; i < player->inventory_count; i++) {
        Item *item = &player->inventory[i];
        int sell_price = 0;
        
        switch (item->type) {
            case ITEM_OXYGEN_CAPSULE: sell_price = 7; break;
            case ITEM_HEALTH_KIT: sell_price = 12; break;
            case ITEM_STIMULANT: sell_price = 17; break;
            case ITEM_ANTIDOTE: sell_price = 15; break;
            default: sell_price = 5; break;
        }
        
        printf(COLOR_GREEN "%d." COLOR_RESET " %-25s x%d | ", 
               i + 1, item->name, item->quantity);
        printf(COLOR_YELLOW "💰 %d perles/unite" COLOR_RESET "\n", sell_price);
    }
    
    print_separator('-', 70);
    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour\n");
    
    printf("\n" COLOR_BOLD "Quel objet voulez-vous vendre ? (0 pour annuler): " COLOR_RESET);
    
    char choice = get_char_input();
    
    if (choice == '0') {
        return;
    }
    
    int item_index = choice - '1';
    
    if (item_index >= 0 && item_index < player->inventory_count) {
        Item *item = &player->inventory[item_index];
        
        int sell_price = 0;
        switch (item->type) {
            case ITEM_OXYGEN_CAPSULE: sell_price = 7; break;
            case ITEM_HEALTH_KIT: sell_price = 12; break;
            case ITEM_STIMULANT: sell_price = 17; break;
            case ITEM_ANTIDOTE: sell_price = 15; break;
            default: sell_price = 5; break;
        }
        
        printf("\n" COLOR_BOLD "Combien d'unites vendre ? (max %d): " COLOR_RESET, item->quantity);
        int quantity = get_int_input(1, item->quantity);
        
        if (quantity > 0 && quantity <= item->quantity) {
            int total = sell_price * quantity;
            player->pearls += total;
            item->quantity -= quantity;
            
            printf(COLOR_GREEN "\n✅ Vente reussie !\n" COLOR_RESET);
            printf("Vous avez vendu %dx %s pour %d perles\n", quantity, item->name, total);
            printf(COLOR_YELLOW "💰 Vous avez maintenant %d perles\n" COLOR_RESET, player->pearls);
            
            if (item->quantity == 0) {
                for (int i = item_index; i < player->inventory_count - 1; i++) {
                    player->inventory[i] = player->inventory[i + 1];
                }
                player->inventory_count--;
            }
        } else {
            printf(COLOR_RED "\n❌ Quantite invalide !\n" COLOR_RESET);
        }
    } else {
        print_error("Objet invalide !");
    }
    
    pause_screen();
}