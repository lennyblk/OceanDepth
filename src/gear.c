#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/gear.h"
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"

void manage_equipment(Player *player)
{
    int running = 1;

    while (running)
    {
        clear_screen();

        printf(COLOR_CYAN COLOR_BOLD "🛡️ GESTION DE L'ÉQUIPEMENT\n" COLOR_RESET);
        print_separator('=', 70);

        printf(COLOR_BOLD "Slayer: " COLOR_RESET "%s | ", player->name);
        printf(COLOR_RED "❤ %d/%d" COLOR_RESET " | ", player->hp, player->max_hp);
        printf(COLOR_CYAN "💨 %d/%d" COLOR_RESET "\n\n", player->oxygen, player->max_oxygen);

        printf(COLOR_BOLD "═══ ÉQUIPEMENT ACTUEL ═══" COLOR_RESET "\n");

        // Affichage de l'arme avec détails
        printf(COLOR_GREEN "🗡️  Arme équipée:" COLOR_RESET " %s\n", player->equipped_weapon.name);
        printf("     ├─ Attaque: %d-%d dégâts\n", player->equipped_weapon.attack_min, player->equipped_weapon.attack_max);
        printf("     ├─ Coût oxygène: %d/attaque\n", player->equipped_weapon.oxygen_cost_per_attack);
        printf("     ├─ Ignore défense: %d\n", player->equipped_weapon.defense_ignore);
        printf("     └─ Rareté: %s\n",
               player->equipped_weapon.rarity == RARITY_COMMON ? "Commune" : player->equipped_weapon.rarity == RARITY_RARE ? "Rare"
                                                                         : player->equipped_weapon.rarity == RARITY_EPIC   ? "Épique"
                                                                                                                           : "Légendaire");

        printf("\n");

        // Affichage de l'armure avec détails
        printf(COLOR_BLUE "🛡️  Armure équipée:" COLOR_RESET " %s\n", player->equipped_armor.name);
        printf("     ├─ Défense: +%d\n", player->equipped_armor.defense);
        printf("     ├─ Coût oxygène: %d/tour\n", player->equipped_armor.oxygen_cost_per_turn);
        printf("     └─ Rareté: %s\n",
               player->equipped_armor.rarity == RARITY_COMMON ? "Commune" : player->equipped_armor.rarity == RARITY_RARE ? "Rare"
                                                                        : player->equipped_armor.rarity == RARITY_EPIC   ? "Épique"
                                                                                                                         : "Légendaire");

        printf("\n" COLOR_BOLD "═══ ACTIONS ═══" COLOR_RESET "\n");
        printf(COLOR_GREEN "1." COLOR_RESET " ⚔️  Changer d'arme\n");
        printf(COLOR_GREEN "2." COLOR_RESET " 🛡️  Changer d'armure\n");
        printf(COLOR_GREEN "3." COLOR_RESET " 📊 Voir statistiques de combat\n");
        printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour au menu principal\n");

        printf("\n" COLOR_BOLD "Votre choix: " COLOR_RESET);

        char choice = get_char_input();

        switch (choice)
        {
        case '1':
            // DIRECTLY open weapon selection (combined list: stored + inventory)
            equip_weapon_from_inventory(player);
            break;

        case '2':
            // DIRECTLY open armor selection (combined list: stored + inventory)
            equip_armor_from_inventory(player);
            break;

        case '3':
        {
            clear_screen();
            printf(COLOR_BOLD "═══ STATISTIQUES DE COMBAT ═══" COLOR_RESET "\n");
            int avg_damage = (player->equipped_weapon.attack_min + player->equipped_weapon.attack_max) / 2;
            printf("🗡️  Dégâts moyens: %d\n", avg_damage);
            printf("🛡️  Protection totale: %d\n", player->equipped_armor.defense);
            printf("💨 Coût par combat: %d oxygène\n",
                   player->equipped_weapon.oxygen_cost_per_attack + player->equipped_armor.oxygen_cost_per_turn);
            pause_screen();
        }
        break;

        case '0':
            running = 0;
            break;

        default:
            print_error("Choix invalide !");
            pause_screen();
            break;
        }
    }
}

// Updated: show combined selectable list (stocked first if present, then inventory weapons)
void equip_weapon_from_inventory(Player *player)
{
    clear_screen();
    printf(COLOR_CYAN COLOR_BOLD "⚔️ SELECTION D'ARME\n" COLOR_RESET);
    print_separator('=', 60);

    printf(COLOR_BOLD "Arme actuelle: " COLOR_RESET "%s (%d-%d)\n\n",
           player->equipped_weapon.name,
           player->equipped_weapon.attack_min,
           player->equipped_weapon.attack_max);

    // Build options: stored weapon (if any) then weapons from inventory
    int option_count = 0;
    int inv_weapon_indices[4]; // map option->inventory index
    int has_stored = player->has_stored_weapon && player->stored_weapon.name[0];

    if (has_stored)
    {
        option_count++;
        printf(COLOR_GREEN "1." COLOR_RESET " %s (%d-%d)\n",
               player->stored_weapon.name, player->stored_weapon.attack_min, player->stored_weapon.attack_max);
    }

    // List weapons in inventory
    int printed = 0;
    for (int i = 0; i < player->inventory_count; ++i)
    {
        if (player->inventory[i].type == ITEM_WEAPON)
        {
            option_count++;
            inv_weapon_indices[printed++] = i;
            printf(COLOR_GREEN "%d." COLOR_RESET " %s (%d-%d)\n",
                   option_count,
                   player->inventory[i].weapon_data.name,
                   player->inventory[i].weapon_data.attack_min,
                   player->inventory[i].weapon_data.attack_max);
        }
    }

    if (option_count == 0)
    {
        printf(COLOR_YELLOW "Aucune arme disponible (stockée ou inventaire).\n" COLOR_RESET);
        pause_screen();
        return;
    }

    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Annuler\n");
    printf("\n" COLOR_BOLD "Choisissez l'arme à équiper: " COLOR_RESET);

    char c = get_char_input();
    int sel = c - '0';
    if (c < '0' || c > '9')
    {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }
    if (sel == 0)
        return;
    if (sel < 1 || sel > option_count)
    {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }

    // Determine selection: if has_stored and sel==1 -> stored, else compute inventory index
    if (has_stored && sel == 1)
    {
        // swap equipped <-> stored
        Weapon tmp = player->equipped_weapon;
        player->equipped_weapon = player->stored_weapon;
        player->stored_weapon = tmp;
        player->has_stored_weapon = (player->stored_weapon.name[0] != '\0');
        printf(COLOR_GREEN "\n✅ Arme équipée: %s\n" COLOR_RESET, player->equipped_weapon.name);
        pause_screen();
        return;
    }

    // Otherwise it's an inventory selection. Compute which inventory weapon corresponds.
    int inv_option_index = sel - (has_stored ? 2 : 1); // zero-based into printed array
    if (inv_option_index < 0 || inv_option_index >= printed)
    {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }

    int inv_idx = inv_weapon_indices[inv_option_index];

    // swap: place old equipped into that inventory slot, equip selected weapon
    Item old_weapon;
    memset(&old_weapon, 0, sizeof(old_weapon));
    old_weapon.type = ITEM_WEAPON;
    old_weapon.quantity = 1;
    old_weapon.weapon_data = player->equipped_weapon;
    strncpy(old_weapon.name, player->equipped_weapon.name, sizeof(old_weapon.name)-1);

    player->equipped_weapon = player->inventory[inv_idx].weapon_data;

    // replace inventory slot with old weapon
    player->inventory[inv_idx] = old_weapon;

    printf(COLOR_GREEN "\n✅ Arme équipée: %s\n" COLOR_RESET, player->equipped_weapon.name);
    pause_screen();
}

// Updated: similar logic for armors (stocked + inventory)
void equip_armor_from_inventory(Player *player)
{
    clear_screen();
    printf(COLOR_CYAN COLOR_BOLD "🛡️ SELECTION D'ARMURE\n" COLOR_RESET);
    print_separator('=', 60);

    printf(COLOR_BOLD "Armure actuelle: " COLOR_RESET "%s (+%d)\n\n",
           player->equipped_armor.name,
           player->equipped_armor.defense);

    int option_count = 0;
    int inv_armor_indices[4];
    int printed = 0;
    int has_stored = player->has_stored_armor && player->stored_armor.name[0];

    if (has_stored)
    {
        option_count++;
        printf(COLOR_GREEN "1." COLOR_RESET " %s (+%d)\n",
               player->stored_armor.name, player->stored_armor.defense);
    }

    for (int i = 0; i < player->inventory_count; ++i)
    {
        if (player->inventory[i].type == ITEM_ARMOR)
        {
            option_count++;
            inv_armor_indices[printed++] = i;
            printf(COLOR_GREEN "%d." COLOR_RESET " %s (+%d)\n",
                   option_count,
                   player->inventory[i].armor_data.name,
                   player->inventory[i].armor_data.defense);
        }
    }

    if (option_count == 0)
    {
        printf(COLOR_YELLOW "Aucune armure disponible (stockée ou inventaire).\n" COLOR_RESET);
        pause_screen();
        return;
    }

    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Annuler\n");
    printf("\n" COLOR_BOLD "Choisissez l'armure à équiper: " COLOR_RESET);

    char c = get_char_input();
    int sel = c - '0';
    if (c < '0' || c > '9')
    {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }
    if (sel == 0)
        return;
    if (sel < 1 || sel > option_count)
    {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }

    if (has_stored && sel == 1)
    {
        Armor tmp = player->equipped_armor;
        player->equipped_armor = player->stored_armor;
        player->stored_armor = tmp;
        player->has_stored_armor = (player->stored_armor.name[0] != '\0');
        printf(COLOR_GREEN "\n✅ Armure équipée: %s\n" COLOR_RESET, player->equipped_armor.name);
        pause_screen();
        return;
    }

    int inv_option_index = sel - (has_stored ? 2 : 1);
    if (inv_option_index < 0 || inv_option_index >= printed)
    {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }

    int inv_idx = inv_armor_indices[inv_option_index];

    Item old_armor;
    memset(&old_armor, 0, sizeof(old_armor));
    old_armor.type = ITEM_ARMOR;
    old_armor.quantity = 1;
    old_armor.armor_data = player->equipped_armor;
    strncpy(old_armor.name, player->equipped_armor.name, sizeof(old_armor.name)-1);

    player->equipped_armor = player->inventory[inv_idx].armor_data;
    player->inventory[inv_idx] = old_armor;

    printf(COLOR_GREEN "\n✅ Armure équipée: %s\n" COLOR_RESET, player->equipped_armor.name);
    pause_screen();
}

void show_available_weapons(Player *player)
{
    printf(COLOR_BOLD "Armes disponibles:\n" COLOR_RESET);

    int weapon_count = 0;
    for (int i = 0; i < player->inventory_count; i++)
    {
        if (player->inventory[i].type == ITEM_WEAPON)
        {
            weapon_count++;
            Item *item = &player->inventory[i];
            printf("- %s (%d-%d dégâts)\n",
                   item->weapon_data.name,
                   item->weapon_data.attack_min,
                   item->weapon_data.attack_max);
        }
    }

    if (weapon_count == 0)
    {
        printf(COLOR_YELLOW "Aucune arme dans l'inventaire.\n" COLOR_RESET);
    }
}

void show_available_armors(Player *player)
{
    printf(COLOR_BOLD "Armures disponibles:\n" COLOR_RESET);

    int armor_count = 0;
    for (int i = 0; i < player->inventory_count; i++)
    {
        if (player->inventory[i].type == ITEM_ARMOR)
        {
            armor_count++;
            Item *item = &player->inventory[i];
            printf("- %s (+%d défense)\n",
                   item->armor_data.name,
                   item->armor_data.defense);
        }
    }

    if (armor_count == 0)
    {
        printf(COLOR_YELLOW "Aucune armure dans l'inventaire.\n" COLOR_RESET);
    }
}