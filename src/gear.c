#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/gear.h"
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"

void manage_equipment(Player *player) {
    int running = 1;
    
    while (running) {
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
               player->equipped_weapon.rarity == RARITY_COMMON ? "Commune" :
               player->equipped_weapon.rarity == RARITY_RARE ? "Rare" :
               player->equipped_weapon.rarity == RARITY_EPIC ? "Épique" : "Légendaire");
        
        printf("\n");
        
        // Affichage de l'armure avec détails
        printf(COLOR_BLUE "🛡️  Armure équipée:" COLOR_RESET " %s\n", player->equipped_armor.name);
        printf("     ├─ Défense: +%d\n", player->equipped_armor.defense);
        printf("     ├─ Coût oxygène: %d/tour\n", player->equipped_armor.oxygen_cost_per_turn);
        printf("     └─ Rareté: %s\n",
               player->equipped_armor.rarity == RARITY_COMMON ? "Commune" :
               player->equipped_armor.rarity == RARITY_RARE ? "Rare" :
               player->equipped_armor.rarity == RARITY_EPIC ? "Épique" : "Légendaire");
        
        printf("\n" COLOR_BOLD "═══ ACTIONS ═══" COLOR_RESET "\n");
        printf(COLOR_GREEN "1." COLOR_RESET " ⚔️  Changer d'arme\n");
        printf(COLOR_GREEN "2." COLOR_RESET " 🛡️  Changer d'armure\n");
        printf(COLOR_GREEN "3." COLOR_RESET " 📊 Voir statistiques de combat\n");
        printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour au menu principal\n");
        
        printf("\n" COLOR_BOLD "Votre choix: " COLOR_RESET);
        
        char choice = get_char_input();
        
        switch (choice) {
            case '1':
                equip_weapon_from_inventory(player);
                break;
                
            case '2':
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

void equip_weapon_from_inventory(Player *player) {
    clear_screen();
    printf(COLOR_CYAN COLOR_BOLD "⚔️ CHANGEMENT D'ARME\n" COLOR_RESET);
    print_separator('=', 50);
    
    printf(COLOR_BOLD "Arme actuelle:" COLOR_RESET " %s (%d-%d dégâts)\n\n", 
           player->equipped_weapon.name, 
           player->equipped_weapon.attack_min, 
           player->equipped_weapon.attack_max);
    
    // Chercher les armes dans l'inventaire
    int weapon_count = 0;
    int weapon_indices[4];
    
    printf(COLOR_BOLD "Armes disponibles dans l'inventaire:\n" COLOR_RESET);
    for (int i = 0; i < player->inventory_count; i++) {
        if (player->inventory[i].type == ITEM_WEAPON) {
            weapon_indices[weapon_count] = i;
            weapon_count++;
            
            Item *item = &player->inventory[i];
            printf(COLOR_GREEN "%d." COLOR_RESET " %s (%d-%d dégâts)\n", 
                   weapon_count, 
                   item->weapon_data.name,
                   item->weapon_data.attack_min,
                   item->weapon_data.attack_max);
        }
    }
    
    if (weapon_count == 0) {
        printf(COLOR_YELLOW "Aucune arme disponible dans l'inventaire.\n" COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Annuler\n");
    printf("\n" COLOR_BOLD "Quelle arme équiper ? " COLOR_RESET);
    
    char choice = get_char_input();
    
    if (choice == '0') return;
    
    int weapon_choice = choice - '1';
    if (weapon_choice < 0 || weapon_choice >= weapon_count) {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }
    
    // Équiper la nouvelle arme
    int inventory_index = weapon_indices[weapon_choice];
    Item *selected_weapon = &player->inventory[inventory_index];
    
    // Sauvegarder l'ancienne arme dans l'inventaire
    Item old_weapon;
    old_weapon.type = ITEM_WEAPON;
    strcpy(old_weapon.name, player->equipped_weapon.name);
    old_weapon.quantity = 1;
    old_weapon.weapon_data = player->equipped_weapon;
    
    // Équiper la nouvelle arme
    player->equipped_weapon = selected_weapon->weapon_data;
    
    // Remplacer l'arme dans l'inventaire par l'ancienne
    player->inventory[inventory_index] = old_weapon;
    
    printf(COLOR_GREEN "\n✅ Arme équipée: %s\n" COLOR_RESET, player->equipped_weapon.name);
    pause_screen();
}

void equip_armor_from_inventory(Player *player) {
    clear_screen();
    printf(COLOR_CYAN COLOR_BOLD "🛡️ CHANGEMENT D'ARMURE\n" COLOR_RESET);
    print_separator('=', 50);
    
    printf(COLOR_BOLD "Armure actuelle:" COLOR_RESET " %s (+%d défense)\n\n", 
           player->equipped_armor.name, 
           player->equipped_armor.defense);
    
    // Chercher les armures dans l'inventaire
    int armor_count = 0;
    int armor_indices[4];
    
    printf(COLOR_BOLD "Armures disponibles dans l'inventaire:\n" COLOR_RESET);
    for (int i = 0; i < player->inventory_count; i++) {
        if (player->inventory[i].type == ITEM_ARMOR) {
            armor_indices[armor_count] = i;
            armor_count++;
            
            Item *item = &player->inventory[i];
            printf(COLOR_GREEN "%d." COLOR_RESET " %s (+%d défense)\n", 
                   armor_count, 
                   item->armor_data.name,
                   item->armor_data.defense);
        }
    }
    
    if (armor_count == 0) {
        printf(COLOR_YELLOW "Aucune armure disponible dans l'inventaire.\n" COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Annuler\n");
    printf("\n" COLOR_BOLD "Quelle armure équiper ? " COLOR_RESET);
    
    char choice = get_char_input();
    
    if (choice == '0') return;
    
    int armor_choice = choice - '1';
    if (armor_choice < 0 || armor_choice >= armor_count) {
        print_error("Choix invalide !");
        pause_screen();
        return;
    }
    
    // Équiper la nouvelle armure
    int inventory_index = armor_indices[armor_choice];
    Item *selected_armor = &player->inventory[inventory_index];
    
    // Sauvegarder l'ancienne armure dans l'inventaire
    Item old_armor;
    old_armor.type = ITEM_ARMOR;
    strcpy(old_armor.name, player->equipped_armor.name);
    old_armor.quantity = 1;
    old_armor.armor_data = player->equipped_armor;
    
    // Équiper la nouvelle armure
    player->equipped_armor = selected_armor->armor_data;
    
    // Remplacer l'armure dans l'inventaire par l'ancienne
    player->inventory[inventory_index] = old_armor;
    
    printf(COLOR_GREEN "\n✅ Armure équipée: %s\n" COLOR_RESET, player->equipped_armor.name);
    pause_screen();
}

void show_available_weapons(Player *player) {
    printf(COLOR_BOLD "Armes disponibles:\n" COLOR_RESET);
    
    int weapon_count = 0;
    for (int i = 0; i < player->inventory_count; i++) {
        if (player->inventory[i].type == ITEM_WEAPON) {
            weapon_count++;
            Item *item = &player->inventory[i];
            printf("- %s (%d-%d dégâts)\n", 
                   item->weapon_data.name,
                   item->weapon_data.attack_min,
                   item->weapon_data.attack_max);
        }
    }
    
    if (weapon_count == 0) {
        printf(COLOR_YELLOW "Aucune arme dans l'inventaire.\n" COLOR_RESET);
    }
}

void show_available_armors(Player *player) {
    printf(COLOR_BOLD "Armures disponibles:\n" COLOR_RESET);
    
    int armor_count = 0;
    for (int i = 0; i < player->inventory_count; i++) {
        if (player->inventory[i].type == ITEM_ARMOR) {
            armor_count++;
            Item *item = &player->inventory[i];
            printf("- %s (+%d défense)\n", 
                   item->armor_data.name,
                   item->armor_data.defense);
        }
    }
    
    if (armor_count == 0) {
        printf(COLOR_YELLOW "Aucune armure dans l'inventaire.\n" COLOR_RESET);
    }
}
