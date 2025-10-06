#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/inventory.h"
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"

void manage_inventory(Player *player) {
    int running = 1;
    
    while (running) {
        clear_screen();
        
        printf(COLOR_CYAN COLOR_BOLD "🎒 GESTION DE L'INVENTAIRE\n" COLOR_RESET);
        print_separator('=', 60);
        
        printf(COLOR_BOLD "Joueur: " COLOR_RESET "%s | ", player->name);
        printf(COLOR_YELLOW "Perles: %d" COLOR_RESET " | ", player->pearls);
        printf(COLOR_GREEN "Objets: %d/4" COLOR_RESET "\n\n", player->inventory_count);
        
        if (player->inventory_count == 0) {
            printf(COLOR_YELLOW "Votre inventaire est vide.\n" COLOR_RESET);
            printf("\n" COLOR_GREEN "0." COLOR_RESET " 🔙 Retour au menu principal\n");
            
            printf("\n" COLOR_BOLD "Votre choix: " COLOR_RESET);
            char choice = get_char_input();
            
            if (choice == '0') {
                running = 0;
            }
            continue;
        }
        
        // Affichage de l'inventaire
        printf(COLOR_BOLD "═══ OBJETS DANS L'INVENTAIRE ═══" COLOR_RESET "\n");
        for (int i = 0; i < player->inventory_count; i++) {
            Item *item = &player->inventory[i];
            
            printf("%s%d.%s ", COLOR_GREEN, i + 1, COLOR_RESET);
            
            switch (item->type) {
                case ITEM_OXYGEN_CAPSULE:
                    printf("💨 ");
                    break;
                case ITEM_HEALTH_KIT:
                    printf("❤️ ");
                    break;
                case ITEM_STIMULANT:
                    printf("⚡ ");
                    break;
                case ITEM_ANTIDOTE:
                    printf("🧪 ");
                    break;
                default:
                    printf("📦 ");
                    break;
            }
            
            printf("%s", item->name);
            
            if (item->quantity > 1) {
                printf(" (x%d)", item->quantity);
            }
            
            // Affichage des effets
            if (item->hp_restore > 0) {
                printf(" [HP: +%d]", item->hp_restore);
            }
            if (item->oxygen_restore > 0) {
                printf(" [O2: +%d]", item->oxygen_restore);
            }
            if (item->fatigue_reduce > 0) {
                printf(" [Fatigue: -%d]", item->fatigue_reduce);
            }
            if (item->removes_effect) {
                printf(" [Antidote]");
            }
            
            printf("\n");
        }
        
        printf("\n" COLOR_BOLD "═══ ACTIONS ═══" COLOR_RESET "\n");
        printf(COLOR_GREEN "U." COLOR_RESET " 🍷 Utiliser un objet\n");
        printf(COLOR_GREEN "E." COLOR_RESET " 🔍 Examiner un objet\n");
        printf(COLOR_GREEN "D." COLOR_RESET " 🗑️  Jeter un objet\n");
        printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour au menu principal\n");
        
        printf("\n" COLOR_BOLD "Votre choix: " COLOR_RESET);
        
        char choice = get_char_input();
        
        switch (choice) {
            case 'u':
            case 'U':
                use_inventory_item(player);
                break;
                
            case 'e':
            case 'E':
                examine_inventory_item(player);
                break;
                
            case 'd':
            case 'D':
                drop_inventory_item(player);
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

void use_inventory_item(Player *player) {
    if (player->inventory_count == 0) {
        printf(COLOR_YELLOW "Aucun objet à utiliser.\n" COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n" COLOR_BOLD "Quel objet voulez-vous utiliser ? (1-%d, 0 pour annuler): " COLOR_RESET, player->inventory_count);
    
    char choice = get_char_input();
    
    if (choice == '0') return;
    
    int index = choice - '1'; // conversion du char en index int d'array 
    if (index < 0 || index >= player->inventory_count) {
        print_error("Numéro d'objet invalide !");
        pause_screen();
        return;
    }
    
    Item *item = &player->inventory[index];
    
    printf("\n" COLOR_CYAN "Vous utilisez: %s\n" COLOR_RESET, item->name);
    
    // Appliquer l'effet de l'objet selon son type
    switch (item->type) {
        case ITEM_HEALTH_KIT:
            if (player->hp < player->max_hp) {
                int old_hp = player->hp;
                player->hp = (player->hp + item->hp_restore > player->max_hp) ? 
                            player->max_hp : player->hp + item->hp_restore;
                printf(COLOR_GREEN "❤ +%d HP (de %d à %d)\n" COLOR_RESET, 
                       player->hp - old_hp, old_hp, player->hp);
            } else {
                printf(COLOR_YELLOW "Vos HP sont déjà au maximum !\n" COLOR_RESET);
                pause_screen();
                return;
            }
            break;
            
        case ITEM_OXYGEN_CAPSULE:
            if (player->oxygen < player->max_oxygen) {
                int old_oxygen = player->oxygen;
                player->oxygen = (player->oxygen + item->oxygen_restore > player->max_oxygen) ? 
                                player->max_oxygen : player->oxygen + item->oxygen_restore;
                printf(COLOR_CYAN "💨 +%d Oxygène (de %d à %d)\n" COLOR_RESET, 
                       player->oxygen - old_oxygen, old_oxygen, player->oxygen);
            } else {
                printf(COLOR_YELLOW "Votre oxygène est déjà au maximum !\n" COLOR_RESET);
                pause_screen();
                return;
            }
            break;
            
        case ITEM_ANTIDOTE:
            if (player->is_poisoned) {
                player->is_poisoned = 0;
                player->poison_turns_left = 0;
                printf(COLOR_GREEN "🧪 Empoisonnement soigné !\n" COLOR_RESET);
            } else {
                printf(COLOR_YELLOW "Vous n'êtes pas empoisonné.\n" COLOR_RESET);
                pause_screen();
                return;
            }
            break;
            
        case ITEM_STIMULANT:
            if (player->is_paralyzed) {
                player->is_paralyzed = 0;
                player->paralysis_turns_left = 0;
                printf(COLOR_GREEN "⚡ Paralysie supprimée !\n" COLOR_RESET);
            } else {
                printf(COLOR_YELLOW "Vous n'êtes pas paralysé.\n" COLOR_RESET);
                // Donne quand même un petit boost
                if (player->oxygen < player->max_oxygen) {
                    player->oxygen += 10;
                    if (player->oxygen > player->max_oxygen) {
                        player->oxygen = player->max_oxygen;
                    }
                    printf(COLOR_CYAN "💨 +10 Oxygène de stimulation !\n" COLOR_RESET);
                }
            }
            break;
            
        default:
            printf(COLOR_RED "Cet objet ne peut pas être utilisé.\n" COLOR_RESET);
            pause_screen();
            return;
    }
    
    // Réduire la quantité ou supprimer l'objet
    item->quantity--;
    if (item->quantity <= 0) {
        remove_item_from_inventory(player, index);
        printf(COLOR_YELLOW "L'objet a été consommé.\n" COLOR_RESET);
    }
    
    pause_screen();
}

void examine_inventory_item(Player *player) {
    if (player->inventory_count == 0) {
        printf(COLOR_YELLOW "Aucun objet à examiner.\n" COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n" COLOR_BOLD "Quel objet voulez-vous examiner ? (1-%d, 0 pour annuler): " COLOR_RESET, player->inventory_count);
    
    char choice = get_char_input();
    
    if (choice == '0') return;
    
    int index = choice - '1';
    if (index < 0 || index >= player->inventory_count) {
        print_error("Numéro d'objet invalide !");
        pause_screen();
        return;
    }
    
    Item *item = &player->inventory[index];
    
    clear_screen();
    printf(COLOR_CYAN COLOR_BOLD "🔍 EXAMEN D'OBJET\n" COLOR_RESET);
    print_separator('=', 40);
    
    printf(COLOR_BOLD "Nom: " COLOR_RESET "%s\n", item->name);
    printf(COLOR_BOLD "Quantité: " COLOR_RESET "%d\n", item->quantity);
    printf(COLOR_BOLD "Type: " COLOR_RESET);
    
    switch (item->type) {
        case ITEM_HEALTH_KIT:
            printf("Kit de soins\n");
            printf(COLOR_BOLD "Effet: " COLOR_RESET "Restaure %d HP\n", item->hp_restore);
            break;
        case ITEM_OXYGEN_CAPSULE:
            printf("Capsule d'oxygène\n");
            printf(COLOR_BOLD "Effet: " COLOR_RESET "Restaure %d oxygène\n", item->oxygen_restore);
            break;
        case ITEM_ANTIDOTE:
            printf("Antidote\n");
            printf(COLOR_BOLD "Effet: " COLOR_RESET "Soigne l'empoisonnement\n");
            break;
        case ITEM_STIMULANT:
            printf("Stimulant\n");
            printf(COLOR_BOLD "Effet: " COLOR_RESET "Supprime la paralysie et redonne de l'énergie\n");
            break;
        default:
            printf("Objet inconnu\n");
            break;
    }
    
    pause_screen();
}

void drop_inventory_item(Player *player) {
    if (player->inventory_count == 0) {
        printf(COLOR_YELLOW "Aucun objet à jeter.\n" COLOR_RESET);
        pause_screen();
        return;
    }
    
    printf("\n" COLOR_BOLD "Quel objet voulez-vous jeter ? (1-%d, 0 pour annuler): " COLOR_RESET, player->inventory_count);
    
    char choice = get_char_input();
    
    if (choice == '0') return;
    
    int index = choice - '1';
    if (index < 0 || index >= player->inventory_count) {
        print_error("Numéro d'objet invalide !");
        pause_screen();
        return;
    }
    
    Item *item = &player->inventory[index];
    
    printf("\n" COLOR_RED "Êtes-vous sûr de vouloir jeter '%s' ? (o/n): " COLOR_RESET, item->name);
    
    char confirm = get_char_input();
    
    if (confirm == 'o' || confirm == 'O') {
        printf(COLOR_YELLOW "Vous jetez: %s\n" COLOR_RESET, item->name);
        remove_item_from_inventory(player, index);
        printf(COLOR_GREEN "Objet jeté avec succès.\n" COLOR_RESET);
    } else {
        printf(COLOR_CYAN "Action annulée.\n" COLOR_RESET);
    }
    
    pause_screen();
}

void sell_treasures(Player *player) {
    // Cette fonction n'est pas applicable avec le système d'objets actuel
    (void)player;
    printf(COLOR_YELLOW "Aucun trésor à vendre dans l'inventaire actuel.\n" COLOR_RESET);
    pause_screen();
}

void manage_equipment(Player *player) {
    clear_screen();
    
    printf(COLOR_CYAN COLOR_BOLD "🛡️ GESTION DE L'ÉQUIPEMENT\n" COLOR_RESET);
    print_separator('=', 60);
    
    printf(COLOR_BOLD "═══ ÉQUIPEMENT ACTUEL ═══" COLOR_RESET "\n");
    printf(COLOR_GREEN "Arme équipée:" COLOR_RESET " %s\n", player->equipped_weapon.name);
    printf("  - Attaque: %d-%d\n", player->equipped_weapon.attack_min, player->equipped_weapon.attack_max);
    printf("  - Coût oxygène/attaque: %d\n", player->equipped_weapon.oxygen_cost_per_attack);
    
    printf("\n");
    printf(COLOR_GREEN "Armure équipée:" COLOR_RESET " %s\n", player->equipped_armor.name);
    printf("  - Défense: +%d\n", player->equipped_armor.defense);
    printf("  - Coût oxygène/tour: %d\n", player->equipped_armor.oxygen_cost_per_turn);
    
    printf("\n" COLOR_YELLOW "Note: La gestion avancée de l'équipement sera implémentée plus tard.\n" COLOR_RESET);
    
    pause_screen();
}

int calculate_inventory_weight(Player *player) {
    int weight = 0;
    for (int i = 0; i < player->inventory_count; i++) {
        weight += player->inventory[i].quantity;
    }
    return weight;
}

void remove_item_from_inventory(Player *player, int index) {
    if (index < 0 || index >= player->inventory_count) {
        return;
    }
    
    // Décaler tous les objets suivants vers la gauche
    for (int i = index; i < player->inventory_count - 1; i++) {
        player->inventory[i] = player->inventory[i + 1];
    }
    
    player->inventory_count--; // on regarde jamais le trou qui est tout a droite mtn
}
