#include <stdio.h>
#include <stdlib.h>
#include "../include/menu.h"
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/ascii_art.h"
#include "../include/map.h"

void display_title_screen() {
    clear_screen();
    
    // ASCII Art du titre
    printf(COLOR_CYAN COLOR_BOLD);
    printf("  ███████╗ ██████╗███████╗ █████╗ ███╗   ██╗    ██████╗ ███████╗██████╗ ████████╗██╗  ██╗███████╗\n");
    printf("  ██╔═══██╗██╔════╝██╔════╝██╔══██╗████╗  ██║    ██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██║  ██║██╔════╝\n");
    printf("  ██║   ██║██║     █████╗  ███████║██╔██╗ ██║    ██║  ██║█████╗  ██████╔╝   ██║   ███████║███████╗\n");
    printf("  ██║   ██║██║     ██╔══╝  ██╔══██║██║╚██╗██║    ██║  ██║██╔══╝  ██╔═══╝    ██║   ██╔══██║╚════██║\n");
    printf("  ╚██████╔╝╚██████╗███████╗██║  ██║██║ ╚████║    ██████╔╝███████╗██║        ██║   ██║  ██║███████║\n");
    printf("   ╚═════╝  ╚═════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝    ╚═════╝ ╚══════╝╚═╝        ╚═╝   ╚═╝  ╚═╝╚══════╝\n");
    printf(COLOR_RESET "\n");
    
    print_separator('=', 100);
    printf(COLOR_YELLOW "                           Explorez les profondeurs mystérieuses de l'océan\n" COLOR_RESET);
    print_separator('=', 100);
    printf("\n");
}

int display_main_menu(Player *player) {
    display_title_screen();
    
    // Affichage rapide des stats du joueur
    printf(COLOR_BOLD "Monster Slayer: " COLOR_RESET "%s | ", player->name);
    printf(COLOR_RED "❤ %d/%d" COLOR_RESET " | ", player->hp, player->max_hp);
    printf(COLOR_CYAN "💨 %d/%d" COLOR_RESET " | ", player->oxygen, player->max_oxygen);
    printf(COLOR_YELLOW "🔱 %d perles" COLOR_RESET " | ", player->pearls);
    printf(COLOR_GREEN "Niveau %d" COLOR_RESET "\n\n", player->level);
    
    print_separator('-', 100);
    
    printf(COLOR_BOLD "╔══════════════════════════════════════ MENU PRINCIPAL ══════════════════════════════════════╗\n" COLOR_RESET);
    printf("║                                                                                             ║\n");
    printf("║  " COLOR_GREEN "1." COLOR_RESET " 🗺️  Explorer la carte                    " COLOR_GREEN "6." COLOR_RESET " 🎒 Gérer l'inventaire                         ║\n");
    printf("║  " COLOR_GREEN "2." COLOR_RESET " ⚔️  Chercher des créatures               " COLOR_GREEN "7." COLOR_RESET " 🛡️  Équipement et armes                       ║\n");
    printf("║  " COLOR_GREEN "3." COLOR_RESET " 📊 Voir les statistiques                 " COLOR_GREEN "8." COLOR_RESET " 💾 Sauvegarder la partie                      ║\n");
    printf("║  " COLOR_GREEN "4." COLOR_RESET " 🏪 Visiter le marché                     " COLOR_GREEN "9." COLOR_RESET " 📖 Consulter le journal                       ║\n");
    printf("║  " COLOR_GREEN "5." COLOR_RESET " 🌊 Se reposer en surface                 " COLOR_GREEN "0." COLOR_RESET " ❌ Quitter le jeu                             ║\n");
    printf("║                                                                                             ║\n");
    printf("╚═════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n" COLOR_BOLD "Choisissez votre action: " COLOR_RESET);
    
    char choice = get_char_input();
    return (choice >= '0' && choice <= '9') ? (choice - '0') : -1;
}

void handle_menu_choice(int choice, Player *player, Map *map) {
    switch(choice) {
        case 1: 
            explore_map(player, map);
            break;
            
        case 2: 
            search_creatures(player, map);
            break;
            
        case 3: 
            player_display_stats(player);
            pause_screen();
            break;
            
        case 4: 
            visit_shop(player);
            break;
            
        case 5: 
            rest_at_surface(player);
            break;
            
        case 6: 
            manage_inventory(player);
            break;
            
        case 7: 
            manage_equipment(player);
            break;
            
        case 8: 
            save_game(player, map);
            break;
            
        case 9: 
            display_journal(player);
            break;
            
        case 0: 
            if (confirm_action("Êtes-vous sûr de vouloir quitter ?")) {
                printf(COLOR_CYAN "Merci d'avoir joué à Ocean Depths !\n" COLOR_RESET);
                exit(0);
            }
            break;
            
        default:
            print_error("Choix invalide !");
            pause_screen();
            break;
    }
}

void explore_map(Player *player, Map *map) {
    clear_screen();
    
    printf("╔══════════════════════════════════════ CARTOGRAPHIE OCÉANIQUE ══════════════════════════════════════╗\n");
printf("║                                    Position actuelle: Zone %d (%dm)                                  ║\n", 
           player->current_zone, get_zone_depth(player->current_zone));
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
    
    display_zone_map(player, map);
    
    printf(COLOR_BOLD "\n═══ NAVIGATION ═══" COLOR_RESET "\n");
    printf(COLOR_GREEN "1." COLOR_RESET " ⬆️  Remonter à la zone supérieure\n");
    printf(COLOR_GREEN "2." COLOR_RESET " ⬇️  Descendre à la zone inférieure\n");
    printf(COLOR_GREEN "3." COLOR_RESET " 🎯 Choisir une destination dans cette zone\n");
    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour au menu principal\n");
    
    printf("\n" COLOR_BOLD "Votre choix: " COLOR_RESET);
    
    char choice = get_char_input();
    
    switch(choice) {
        case '1': // Zone supérieure
            if (player->current_zone > 0) {
                player->current_zone--;
                printf(COLOR_CYAN "Vous remontez vers la zone %d...\n" COLOR_RESET, player->current_zone);
                pause_screen();
                explore_map(player, map);
            } else {
                printf(COLOR_YELLOW "Vous êtes déjà à la surface !\n" COLOR_RESET);
                pause_screen();
                explore_map(player, map);
            }
            break;
            
        case '2': // Zone inférieure
            if (player->current_zone < 3) {
                if (is_zone_unlocked(player, player->current_zone + 1)) {
                    player->current_zone++;
                    printf(COLOR_BLUE "Vous descendez vers la zone %d...\n" COLOR_RESET, player->current_zone);
                    pause_screen();
                    explore_map(player, map);
                } else {
                    printf(COLOR_RED "⚠️  Cette zone est verrouillée ! Terminez d'abord la zone actuelle.\n" COLOR_RESET);
                    pause_screen();
                    explore_map(player, map);
                }
            } else {
                printf(COLOR_RED "Vous êtes déjà dans les profondeurs maximales !\n" COLOR_RESET);
                pause_screen();
                explore_map(player, map);
            }
            break;
            
        case '3': 
            select_destination(player, map);
            explore_map(player, map);
            break;
            
        case '0': // Retour
            return;
            
        default:
            print_error("Choix invalide !");
            pause_screen();
            explore_map(player, map);
            break;
    }
}

void display_zone_map(Player *player, Map *map) {
    const char* zone_names[] = {"SURFACE", "ZONE 1", "ZONE 2", "ZONE 3"};
    const char* zone_depths[] = {"0m", "-50m", "-150m", "-300m"};
    
    // Destinations par zone (4 destinations par zone)
    const char* destinations[4][4] = {
        {"🚤 Base", "🌊 Océan", "🌊 Océan", "🚤 Bateau"},           // Surface
        {"🪸 Récif", "💰 Épave", "🌿 Algues", "🕳️ Grotte"},        // Zone 1
        {"🦈 Requin", "❌ Vide", "🦑 Kraken", "❌ Vide"},           // Zone 2
        {"❓ Inconnu", "❓ Inconnu", "❓ Inconnu", "❓ Inconnu"}      // Zone 3
    };
    
    printf(COLOR_BOLD "                    CARTOGRAPHIE OCÉANIQUE - SECTEUR PACIFIQUE\n" COLOR_RESET);
    print_separator('-', 80);
    
    for (int zone = 0; zone < 4; zone++) {
        // Ligne de séparation
        printf("┌─────────┬─────────┬─────────┬─────────┐ ");
        
        if (zone == player->current_zone) {
            printf(COLOR_YELLOW "← VOUS ÊTES ICI" COLOR_RESET);
        } else if (!is_zone_unlocked(player, zone)) {
            printf(COLOR_RED "🔒 VERROUILLÉ" COLOR_RESET);
        }
        printf("\n");
        
        // Contenu de la zone
        printf("│");
        for (int col = 0; col < 4; col++) {
            if (is_destination_cleared(map, zone, col)) {
                printf(COLOR_GREEN " ✓ %s " COLOR_RESET, destinations[zone][col]);
            } else if (is_zone_unlocked(player, zone)) {
                printf(" %s ", destinations[zone][col]);
            } else {
                printf(COLOR_BOLD " 🔒 Vide " COLOR_RESET);
            }
            printf("│");
        }
        printf(" %s | %s\n", zone_names[zone], zone_depths[zone]);
        
        // Ligne de fermeture
        printf("└─────────┴─────────┴─────────┴─────────┘\n");
    }
    
    printf("\n" COLOR_YELLOW "Légende: " COLOR_RESET);
    printf(COLOR_GREEN "✓ = Terminé" COLOR_RESET " | ");
    printf(COLOR_BOLD "🔒 = Verrouillé" COLOR_RESET " | ");
    printf(COLOR_YELLOW "Zone actuelle surlignée" COLOR_RESET "\n");
}

void select_destination(Player *player, Map *map) {
    clear_screen();
    
    const char* destinations[4][4] = {
        {"🚤 Base [SAUF]", "🌊 Océan", "🌊 Océan", "🚤 Bateau [SHOP]"},
        {"🪸 Récif [3 ENM]", "💰 Épave [TRÉSOR]", "🌿 Algues [5 ENM]", "🕳️ Grotte [SAUF]"},
        {"🦈 Requin [BOSS]", "❌ Vide", "🦑 Kraken [8 ENM]", "❌ Vide"},
        {"❓ Inconnu", "❓ Inconnu", "❓ Inconnu", "❓ Inconnu"}
    };
    
    printf(COLOR_CYAN COLOR_BOLD "🎯 SÉLECTION DE DESTINATION - ZONE %d\n" COLOR_RESET, player->current_zone);
    print_separator('=', 60);
    
    printf("Destinations disponibles:\n\n");
    
    for (int i = 0; i < 4; i++) {
        if (is_destination_available(map, player->current_zone, i)) {
            if (is_destination_cleared(map, player->current_zone, i)) {
                printf(COLOR_GREEN "%d." COLOR_RESET " %s " COLOR_GREEN "[TERMINÉ]" COLOR_RESET "\n", 
                       i + 1, destinations[player->current_zone][i]);
            } else {
                printf(COLOR_GREEN "%d." COLOR_RESET " %s\n", i + 1, destinations[player->current_zone][i]);
            }
        } else {
            printf(COLOR_BOLD "%d. [Destination non disponible]" COLOR_RESET "\n", i + 1);
        }
    }
    
    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour\n");
    
    printf("\n" COLOR_BOLD "Choisissez votre destination: " COLOR_RESET);
    
    char choice = get_char_input();
    
    if (choice >= '1' && choice <= '4') {
        int dest = choice - '1';
        if (is_destination_available(map, player->current_zone, dest)) {
            enter_destination(player, map, player->current_zone, dest);
        } else {
            printf(COLOR_RED "Cette destination n'est pas disponible !\n" COLOR_RESET);
            pause_screen();
        }
    } else if (choice != '0') {
        print_error("Choix invalide !");
        pause_screen();
    }
}

void enter_destination(Player *player, Map *map, int zone, int destination) {
    clear_screen();
    
    const char* dest_names[4][4] = {
        {"Base de plongée", "Océan libre", "Océan libre", "Bateau marchand"},
        {"Récif corallien", "Épave du galion", "Forêt d'algues", "Grotte sous-marine"},
        {"Territoire du requin", "Zone vide", "Repaire du Kraken", "Zone vide"},
        {"Zone inconnue", "Zone inconnue", "Zone inconnue", "Zone inconnue"}
    };
    
    printf(COLOR_CYAN COLOR_BOLD "🏊 EXPLORATION: %s\n" COLOR_RESET, dest_names[zone][destination]);
    print_separator('=', 60);
    
    if (is_destination_cleared(map, zone, destination)) {
        printf(COLOR_GREEN "Cette zone a déjà été nettoyée de ses créatures.\n" COLOR_RESET);
        printf("Vous pouvez chercher des ressources supplémentaires.\n\n");
        
        // Petite récompense pour revisiter
        int bonus_pearls = 1 + rand() % 2;
        player->pearls += bonus_pearls;
        printf(COLOR_YELLOW "✨ Vous trouvez %d perle(s) supplémentaire(s) !\n" COLOR_RESET, bonus_pearls);
        
        pause_screen();
        return;
    }
    
    // Système de combat selon la zone
    int monsters_count = get_monsters_in_destination(zone, destination);
    
    if (monsters_count > 0) {
        printf("⚠️  Vous détectez %d créature(s) hostile(s) dans cette zone !\n", monsters_count);
        printf("Préparez-vous au combat...\n\n");
        
        pause_screen();
        
        // Combat contre toutes les créatures
        bool victory = fight_all_monsters(player, zone, monsters_count);
        
        if (victory) {
            mark_destination_cleared(map, zone, destination);
            
            give_zone_rewards(player, zone, destination);
            
            if (is_zone_completely_cleared(map, zone)) {
                unlock_next_zone(player, map, zone);
            }
        }
    } else {
        printf(COLOR_GREEN "Cette zone est paisible, aucune créature hostile détectée.\n" COLOR_RESET);
        // Zone sûre (comme les bases)
        if (zone == 0 && (destination == 0 || destination == 3)) {
            printf("Vous pouvez vous reposer ici et récupérer votre oxygène.\n");
            player->oxygen = player->max_oxygen;
            printf(COLOR_CYAN "💨 Oxygène restauré à %d/%d\n" COLOR_RESET, player->oxygen, player->max_oxygen);
        }
    }
    
    pause_screen();
}

// Fonctions utilitaires pour le système de zones

int get_zone_depth(int zone) {
    int depths[] = {0, 50, 150, 300};
    return depths[zone];
}

int is_zone_unlocked(Player *player, int zone) {
    if (zone <= 0) return true;
    return player->zones_unlocked >= zone;
}

int is_destination_available(Map *map, int zone, int destination) {
    (void)map; // Supprime l'avertissement de paramètre non utilisé
    // Zone 2 a des destinations vides
    if (zone == 2 && (destination == 1 || destination == 3)) {
        return false;
    }
    return is_zone_unlocked(NULL, zone);
}

int is_destination_cleared(Map *map, int zone, int destination) {
    return map->zones[zone].destinations[destination].cleared;
}

void mark_destination_cleared(Map *map, int zone, int destination) {
    map->zones[zone].destinations[destination].cleared = true;
}

int get_monsters_in_destination(int zone, int destination) {
    int monsters[4][4] = {
        {0, 2, 1, 0},    // Surface: Base=0, Océan=2, Océan=1, Bateau=0
        {3, 1, 5, 0},    // Zone 1: Récif=3, Épave=1, Algues=5, Grotte=0 (sûre)
        {1, 0, 8, 0},    // Zone 2: Requin=1 (boss), Vide=0, Kraken=8, Vide=0
        {5, 7, 10, 12}   // Zone 3: Tous dangereux
    };
    
    return monsters[zone][destination];
}

int is_zone_completely_cleared(Map *map, int zone) {
    for (int i = 0; i < 4; i++) {
        if (is_destination_available(map, zone, i) && !is_destination_cleared(map, zone, i)) {
            return false;
        }
    }
    return true;
}

void unlock_next_zone(Player *player, Map *map, int current_zone) {
    (void)map; // Supprime l'avertissement de paramètre non utilisé
    if (current_zone + 1 < 4 && player->zones_unlocked <= current_zone) {
        player->zones_unlocked = current_zone + 1;
        printf(COLOR_YELLOW "\n🔓 NOUVELLE ZONE DÉBLOQUÉE !\n" COLOR_RESET);
        printf("Vous pouvez maintenant accéder à la zone %d !\n", current_zone + 1);
        pause_screen();
    }
}

// Implémentations temporaires des fonctions manquantes
void search_creatures(Player *player, Map *map) {
    (void)player;
    (void)map;
    printf("Fonction search_creatures pas encore implémentée.\n");
    pause_screen();
}

void visit_shop(Player *player) {
    (void)player;
    printf("Fonction visit_shop pas encore implémentée.\n");
    pause_screen();
}

void rest_at_surface(Player *player) {
    printf("Vous vous reposez en surface...\n");
    player->hp = player->max_hp;
    player->oxygen = player->max_oxygen;
    printf(COLOR_GREEN "❤ HP et 💨 Oxygène restaurés !\n" COLOR_RESET);
    pause_screen();
}

void manage_inventory(Player *player) {
    (void)player;
    printf("Fonction manage_inventory pas encore implémentée.\n");
    pause_screen();
}

void manage_equipment(Player *player) {
    (void)player;
    printf("Fonction manage_equipment pas encore implémentée.\n");
    pause_screen();
}

void save_game(Player *player, Map *map) {
    (void)player;
    (void)map;
    printf("Fonction save_game pas encore implémentée.\n");
    pause_screen();
}

void display_journal(Player *player) {
    (void)player;
    printf("Fonction display_journal pas encore implémentée.\n");
    pause_screen();
}

int fight_all_monsters(Player *player, int zone, int monsters_count) {
    (void)player;
    (void)zone;
    printf("Combat contre %d créature(s)... (temporaire)\n", monsters_count);
    printf("Vous gagnez le combat !\n");
    pause_screen();
    return 1; // Victoire
}

void give_zone_rewards(Player *player, int zone, int destination) {
    int reward = (zone + 1) * (destination + 1) * 5;
    player->pearls += reward;
    printf(COLOR_YELLOW "✨ Vous gagnez %d perles !\n" COLOR_RESET, reward);
}
