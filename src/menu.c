#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/menu.h"
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/ascii_art.h"
#include "../include/map.h"
#include "../include/shop.h"
#include "../include/inventory.h"
#include "../include/gear.h"
#include "../include/combat.h"
#include "../include/save.h"
#include "../include/creature.h"

static int get_possible_creature_types(int zone, CreatureType possible_types[])
{
    int num_possible_types = 0;
    switch (zone)
    {
    case 0:
        possible_types[0] = CREATURE_JELLYFISH;
        num_possible_types = 1;
        break;
    case 1:
        possible_types[0] = CREATURE_JELLYFISH;
        possible_types[1] = CREATURE_SWORDFISH;
        possible_types[2] = CREATURE_GIANT_CRAB;
        num_possible_types = 3;
        break;
    case 2:
        possible_types[0] = CREATURE_SHARK;
        possible_types[1] = CREATURE_SWORDFISH;
        possible_types[2] = CREATURE_KRAKEN;
        num_possible_types = 3;
        break;
    case 3:
    default:
        possible_types[0] = CREATURE_KRAKEN;
        possible_types[1] = CREATURE_SHARK;
        possible_types[3] = CREATURE_SWORDFISH;
        possible_types[4] = CREATURE_GIANT_CRAB;
        num_possible_types = 4;
        break;
    }
    return num_possible_types;
}

static void set_creature_base_stats(Creature *creature, int zone)
{
    int hp_bonus = zone * 15;
    int atk_bonus = zone * 3;
    int def_bonus = zone * 2;

    switch (creature->type)
    {
    case CREATURE_KRAKEN:
        strcpy(creature->name, "Kraken");
        creature->hp_max = random_range(150, 200) + hp_bonus;
        creature->attack_min = 20 + atk_bonus;
        creature->attack_max = 35 + atk_bonus;
        creature->defense = 12 + def_bonus;
        creature->speed = 5;
        break;
    case CREATURE_SHARK:
        strcpy(creature->name, "Requin Affamé");
        creature->hp_max = random_range(80, 120) + hp_bonus;
        creature->attack_min = 18 + atk_bonus;
        creature->attack_max = 28 + atk_bonus;
        creature->defense = 6 + def_bonus;
        creature->speed = 15;
        break;
    case CREATURE_JELLYFISH:
        strcpy(creature->name, "Méduse Électrique");
        creature->hp_max = random_range(30, 50) + hp_bonus;
        creature->attack_min = 10 + atk_bonus;
        creature->attack_max = 16 + atk_bonus;
        creature->defense = 2 + def_bonus;
        creature->speed = 8;
        break;
    case CREATURE_SWORDFISH:
        strcpy(creature->name, "Espadon Vif");
        creature->hp_max = random_range(70, 100) + hp_bonus;
        creature->attack_min = 20 + atk_bonus;
        creature->attack_max = 30 + atk_bonus;
        creature->defense = 5 + def_bonus;
        creature->speed = 12;
        break;
    case CREATURE_GIANT_CRAB:
        strcpy(creature->name, "Crabe Carapace");
        creature->hp_max = random_range(100, 150) + hp_bonus;
        creature->attack_min = 15 + atk_bonus;
        creature->attack_max = 22 + atk_bonus;
        creature->defense = 20 + def_bonus;
        creature->speed = 3;
        break;
    default:
        strcpy(creature->name, "Poisson Étrange");
        creature->hp_max = 50 + hp_bonus;
        creature->attack_min = 8 + atk_bonus;
        creature->attack_max = 12 + atk_bonus;
        creature->defense = 3 + def_bonus;
        creature->speed = 0;
        break;
    }
    creature->hp_current = creature->hp_max;
    creature->effect_duration = 0;
    creature->special_effect = EFFECT_NONE;
}

static void display_map_header(void)
{
    printf(COLOR_BOLD "                    CARTOGRAPHIE OCÉANIQUE - SECTEUR PACIFIQUE\n" COLOR_RESET);
    print_separator('-', 80);
}

static void display_map_row(Player *player, Map *map, int zone, const char *destinations[4][4])
{
    const char *zone_names[] = {"SURFACE", "ZONE 1", "ZONE 2", "ZONE 3"};
    const char *zone_depths[] = {"0m", "-50m", "-150m", "-300m"};

    printf("┌─────────┬─────────┬─────────┬─────────┐ ");
    if (zone == player->current_zone)
        printf(COLOR_YELLOW "← VOUS ÊTES ICI" COLOR_RESET);
    else if (!is_zone_unlocked(player, zone))
        printf(COLOR_RED "🔒 VERROUILLÉ" COLOR_RESET);
    printf("\n");

    printf("│");
    for (int col = 0; col < 4; col++)
    {
        if (is_destination_cleared(map, zone, col))
            printf(COLOR_GREEN " ✓ %s " COLOR_RESET, destinations[zone][col]);
        else if (is_zone_unlocked(player, zone))
            printf(" %s ", destinations[zone][col]);
        else
            printf(COLOR_BOLD " 🔒 Vide " COLOR_RESET);
        printf("│");
    }
    printf(" %s | %s\n", zone_names[zone], zone_depths[zone]);
    printf("└─────────┴─────────┴─────────┴─────────┘\n");
}

static void display_map_legend(void)
{
    printf("\n" COLOR_YELLOW "Légende: " COLOR_RESET);
    printf(COLOR_GREEN "✓ = Terminé" COLOR_RESET " | ");
    printf(COLOR_BOLD "🔒 = Verrouillé" COLOR_RESET " | ");
    printf(COLOR_YELLOW "Zone actuelle surlignée" COLOR_RESET "\n");
}

static void handle_cleared_destination(Player *player)
{
    printf(COLOR_GREEN "Cette zone a déjà été nettoyée de ses créatures.\n" COLOR_RESET);
    printf("Vous pouvez chercher des ressources supplémentaires.\n\n");

    int bonus_pearls = 1 + random_range(1, 2);
    player->pearls += bonus_pearls;
    printf(COLOR_YELLOW "✨ Vous trouvez %d perle(s) supplémentaire(s) !\n" COLOR_RESET, bonus_pearls);
}

static void handle_hostile_destination(Player *player, Map *map, int zone, int destination, int monsters_count)
{
    printf("⚠️  Vous détectez %d créature(s) hostile(s) dans cette zone !\n", monsters_count);
    printf("Préparez-vous au combat...\n\n");
    pause_screen();

    int combat_result = fight_all_monsters(player, zone, monsters_count);

    if (combat_result == 1)
    {
        mark_destination_cleared(map, zone, destination);
        give_zone_rewards(player, zone, destination);
        if (is_zone_completely_cleared(player, map, zone))
            unlock_next_zone(player, map, zone);
    }
    else if (combat_result == -1)
    {
        printf("Vous avez été vaincu...\n");
    }
}

static void handle_safe_destination(Player *player, Map *map, int zone, int destination)
{
    printf(COLOR_GREEN "Cette zone est paisible, aucune créature hostile détectée.\n" COLOR_RESET);
    if (zone == 0 && (destination == 0 || destination == 3))
    {
        printf("Vous pouvez vous reposer ici et récupérer votre oxygène.\n");
        player->oxygen = player->max_oxygen;
        printf(COLOR_CYAN "💨 Oxygène restauré à %d/%d\n" COLOR_RESET, player->oxygen, player->max_oxygen);
    }
    mark_destination_cleared(map, zone, destination);
    give_zone_rewards(player, zone, destination);

    if (is_zone_completely_cleared(player, map, zone))
        unlock_next_zone(player, map, zone);
}

void display_title_screen(void)
{
    clear_screen();
    // ASCII Art du titre
    printf(COLOR_CYAN COLOR_BOLD);
    printf("  ███████╗ ██████╗███████╗ █████╗ ███╗   ██╗    ██████╗ ███████╗██████╗ ████████╗██╗  ██╗\n");
    printf("  ██╔═══██╗██╔════╝██╔════╝██╔══██╗████╗  ██║    ██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██║  ██║\n");
    printf("  ██║   ██║██║     █████╗  ███████║██╔██╗ ██║    ██║  ██║█████╗  ██████╔╝   ██║   ███████║\n");
    printf("  ██║   ██║██║     ██╔══╝  ██╔══██║██║╚██╗██║    ██║  ██║██╔══╝  ██╔═══╝    ██║   ██╔══██║\n");
    printf("  ╚██████╔╝╚██████╗███████╗██║  ██║██║ ╚████║    ██████╔╝███████╗██║        ██║   ██║  ██║\n");
    printf("   ╚═════╝  ╚═════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝    ╚═════╝ ╚══════╝╚═╝        ╚═╝   ╚═╝  ╚═╝\n");
    printf(COLOR_RESET "\n");
    print_separator('=', 100);
    printf(COLOR_YELLOW "                           Explorez les profondeurs mystérieuses de l'océan\n" COLOR_RESET);
    print_separator('=', 100);
    printf("\n");
}

int display_main_menu(Player *player)
{
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
            if (is_shop_unlocked(player, map)) {
                visit_shop(player);
            } else {
                clear_screen();
                printf(COLOR_RED "🔒 LE MARCHÉ EST VERROUILLÉ\n" COLOR_RESET);
                print_separator('-', 60);
                printf("\nPour débloquer le marché, vous devez:\n");
                printf(COLOR_YELLOW "→ Terminer toutes les destinations d'au moins une zone\n" COLOR_RESET);
                printf("\nProgressez dans l'exploration pour accéder au marché !\n");
                pause_screen();
            }
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
    return 1;
}

void explore_map(Player *player, Map *map)
{
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
    switch (choice)
    {
    case '1': // Zone supérieure
        if (player->current_zone > 0)
        {
            player->current_zone--;
            printf(COLOR_CYAN "Vous remontez vers la zone %d...\n" COLOR_RESET, player->current_zone);
            pause_screen();
            explore_map(player, map);
        }
        else
        {
            printf(COLOR_YELLOW "Vous êtes déjà à la surface !\n" COLOR_RESET);
            pause_screen();
            explore_map(player, map);
        }
        break;
    case '2': // Zone inférieure
        if (player->current_zone < 3)
        {
            if (is_zone_unlocked(player, player->current_zone + 1))
            {
                player->current_zone++;
                printf(COLOR_BLUE "Vous descendez vers la zone %d...\n" COLOR_RESET, player->current_zone);
                pause_screen();
                explore_map(player, map);
            }
            else
            {
                printf(COLOR_RED "⚠️  Cette zone est verrouillée ! Terminez d'abord la zone actuelle.\n" COLOR_RESET);
                pause_screen();
                explore_map(player, map);
            }
        }
        else
        {
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

void display_zone_map(Player *player, Map *map)
{
    // Destinations par zone (4 destinations par zone)
    const char *destinations[4][4] = {
        {"🚤 Base", "🌊 Océan", "🌊 Océan", "🚤 Bateau"},        // Surface
        {"🪸 Récif", "💰 Épave", "🌿 Algues", "🕳️ Grotte"},      // Zone 1
        {"🦈 Requin", "❌ Vide", "🦑 Kraken", "❌ Vide"},        // Zone 2
        {"❓ Inconnu", "❓ Inconnu", "❓ Inconnu", "❓ Inconnu"} // Zone 3
    };

    display_map_header();
    for (int zone = 0; zone < 4; zone++)
    {
        display_map_row(player, map, zone, destinations);
    }
    display_map_legend();
}

void select_destination(Player *player, Map *map)
{
    clear_screen();
    const char *destinations[4][4] = {
        {"🚤 Base [SAUF]", "🌊 Océan", "🌊 Océan", "🚤 Bateau [SHOP]"},
        {"🪸 Récif [3 ENM]", "💰 Épave [TRÉSOR]", "🌿 Algues [5 ENM]", "🕳️ Grotte [SAUF]"},
        {"🦈 Requin [BOSS]", "❌ Vide", "🦑 Kraken [8 ENM]", "❌ Vide"},
        {"❓ Inconnu", "❓ Inconnu", "❓ Inconnu", "❓ Inconnu"}};

    printf(COLOR_CYAN COLOR_BOLD "🎯 SÉLECTION DE DESTINATION - ZONE %d\n" COLOR_RESET, player->current_zone);
    print_separator('=', 60);
    printf("Destinations disponibles:\n\n");

    for (int i = 0; i < 4; i++)
    {
        if (is_destination_available(player, map, player->current_zone, i))
        {
            if (is_destination_cleared(map, player->current_zone, i))
                printf(COLOR_GREEN "%d." COLOR_RESET " %s " COLOR_GREEN "[TERMINÉ]" COLOR_RESET "\n",
                       i + 1, destinations[player->current_zone][i]);
            else
                printf(COLOR_GREEN "%d." COLOR_RESET " %s\n", i + 1, destinations[player->current_zone][i]);
        }
        else
        {
            printf(COLOR_BOLD "%d. [Destination non disponible]" COLOR_RESET "\n", i + 1);
        }
    }
    printf(COLOR_GREEN "0." COLOR_RESET " 🔙 Retour\n");
    printf("\n" COLOR_BOLD "Choisissez votre destination: " COLOR_RESET);

    char choice = get_char_input();
    if (choice >= '1' && choice <= '4')
    {
        int dest = choice - '1';
        if (is_destination_available(player, map, player->current_zone, dest))
            enter_destination(player, map, player->current_zone, dest);
        else
        {
            printf(COLOR_RED "Cette destination n'est pas disponible !\n" COLOR_RESET);
            pause_screen();
        }
    }
    else if (choice != '0')
    {
        print_error("Choix invalide !");
        pause_screen();
    }
}

void enter_destination(Player *player, Map *map, int zone, int destination)
{
    clear_screen();
    const char *dest_names[4][4] = {
        {"Base de plongée", "Océan libre", "Océan libre", "Bateau marchand"},
        {"Récif corallien", "Épave du galion", "Forêt d'algues", "Grotte sous-marine"},
        {"Territoire du requin", "Zone vide", "Repaire du Kraken", "Zone vide"},
        {"Zone inconnue", "Zone inconnue", "Zone inconnue", "Zone inconnue"}};

    printf(COLOR_CYAN COLOR_BOLD "🏊 EXPLORATION: %s\n" COLOR_RESET, dest_names[zone][destination]);
    print_separator('=', 60);

    if (is_destination_cleared(map, zone, destination))
    {
        handle_cleared_destination(player);
    }
    else
    {
        int monsters_count = get_monsters_in_destination(zone, destination);
        if (monsters_count > 0)
            handle_hostile_destination(player, map, zone, destination, monsters_count);
        else
            handle_safe_destination(player, map, zone, destination);
    }
    pause_screen();
}

// Fonctions utilitaires pour le système de zones
int get_zone_depth(int zone)
{
    int depths[] = {0, 50, 150, 300};
    int array_size = (int)(sizeof(depths) / sizeof(depths[0]));
    if (zone >= 0 && zone < array_size)
        return depths[zone];
    return 0;
}

int is_zone_unlocked(const Player *player, int zone)
{
    if (player == NULL)
    {
        fprintf(stderr, "Erreur: Pointeur player NULL dans is_zone_unlocked\n");
        return 0;
    }
    if (zone < 0 || zone >= MAX_ZONES)
        return 0;
    if (zone == 0)
        return 1;
    return player->zones_unlocked > zone;
}

int is_destination_available(const Player *player, Map *map, int zone, int destination)
{
    (void)map;
    if (player == NULL || zone < 0 || zone >= MAX_ZONES || destination < 0 || destination >= 4)
        return 0;
    if (zone == 2 && (destination == 1 || destination == 3))
        return 0;
    return is_zone_unlocked(player, zone);
}

int is_destination_cleared(Map *map, int zone, int destination)
{
    if (map == NULL || zone < 0 || zone >= map->zone_count || destination < 0 || destination >= 4)
        return 0;
    return map->zones[zone].destinations[destination].cleared;
}

void mark_destination_cleared(Map *map, int zone, int destination)
{
    if (map != NULL && zone >= 0 && zone < map->zone_count && destination >= 0 && destination < 4)
    {
        map->zones[zone].destinations[destination].cleared = 1;
    }
}

int get_monsters_in_destination(int zone, int destination)
{
    if (zone < 0 || zone >= 4 || destination < 0 || destination >= 4)
        return 0;

    int monsters[4][4] = {
        {0, 2, 1, 0},  // Surface: Base=0, Océan=2, Océan=1, Bateau=0
        {3, 1, 5, 0},  // Zone 1: Récif=3, Épave=1, Algues=5, Grotte=0 (sûre)
        {1, 0, 8, 0},  // Zone 2: Requin=1 (boss), Vide=0, Kraken=8, Vide=0
        {5, 7, 10, 12} // Zone 3: Tous dangereux
    };

    if (zone == 2 && destination == 0)
        return 1;
    if (zone == 2 && destination == 2)
        return 1;
    return monsters[zone][destination];
}

int is_zone_completely_cleared(const Player *player, Map *map, int zone)
{
    if (player == NULL || map == NULL || zone < 0 || zone >= map->zone_count)
        return 0;
    for (int i = 0; i < 4; i++)
    {
        if (is_destination_available(player, map, zone, i) && !is_destination_cleared(map, zone, i))
            return 0;
    }
    return 1;
}

void unlock_next_zone(Player *player, Map *map, int current_zone)
{
    (void)map;
    if (player == NULL || current_zone < 0 || current_zone >= MAX_ZONES - 1)
        return;

    int next_zone = current_zone + 1;
    if (next_zone < player->zones_unlocked || next_zone >= MAX_ZONES)
        return;

    player->zones_unlocked = next_zone + 1;
    printf(COLOR_YELLOW "\n🔓 NOUVELLE ZONE DÉBLOQUÉE !\n" COLOR_RESET);
    printf("Vous pouvez maintenant accéder à la zone %d !\n", next_zone);
    pause_screen();
}

void search_creatures(Player *player, Map *map) {
    (void)map; 
    clear_screen();
    printf(COLOR_CYAN "🔍 RECHERCHE DE CRÉATURES\n" COLOR_RESET);
    print_separator('-', 60);
    
    if (player->oxygen < 10) {
        printf(COLOR_RED "⚠️  Vous n'avez pas assez d'oxygène pour chercher des créatures !\n");
        printf("Retournez à la surface pour vous reposer.\n" COLOR_RESET);
        pause_screen();
        return;
    }

    printf("Vous cherchez des créatures dans la zone %d...\n", player->current_zone);
    
    player->oxygen -= 10;
    printf(COLOR_CYAN "(-10 💨 oxygène)\n" COLOR_RESET);

    if (rand() % 2 == 0) {
        int pearls = 2 + (player->current_zone * 2);
        printf(COLOR_GREEN "\n✨ Vous avez trouvé une créature !\n");
        printf("Vous gagnez %d perles !\n" COLOR_RESET, pearls);
        player->pearls += pearls;
    } else {
        printf(COLOR_YELLOW "\nOops ! Vous n'avez rien trouvé cette fois-ci...\n" COLOR_RESET);
    }

    pause_screen();
}


void rest_at_surface(Player *player)
{
    if (player == NULL)
        return;
    if (player->current_zone == 0)
    {
        printf("Vous vous reposez en surface...\n");
        player->hp = player->max_hp;
        player->oxygen = player->max_oxygen;
        printf(COLOR_GREEN "❤ HP et 💨 Oxygène restaurés !\n" COLOR_RESET);
    }
    else
    {
        print_error("Vous ne pouvez vous reposer qu'à la surface !");
    }
    pause_screen();
}

void save_game(Player *player, Map *map) {
    (void)player;
    (void)map;
    printf("Fonction save_game pas encore implémentée.\n");
    pause_screen();
}

void manage_inventory(Player *player)
{
    (void)player;
    printf("Fonction manage_inventory pas encore implémentée.\n");
    pause_screen();
}

void manage_equipment(Player *player)
{
    (void)player;
    printf("Fonction manage_equipment pas encore implémentée.\n");
    pause_screen();
}

void display_journal(Player *player)
{
    (void)player;
    printf("Fonction display_journal pas encore implémentée.\n");
    pause_screen();
}

void create_creatures_for_zone(Creature creatures[], int creature_count, int zone)
{
    if (creature_count <= 0 || creature_count > MAX_CREATURES_PER_ZONE)
        return;

    CreatureType possible_types[CREATURE_COUNT];
    int num_possible_types = get_possible_creature_types(zone, possible_types);
    if (num_possible_types == 0)
        return;

    for (int i = 0; i < creature_count; i++)
    {
        CreatureType type = possible_types[random_range(0, num_possible_types - 1)];
        creatures[i].id = i;
        creatures[i].is_alive = 1;
        creatures[i].is_active = 1;
        creatures[i].type = type;
        set_creature_base_stats(&creatures[i], zone);
    }
}

int give_zone_rewards(Player *player, int zone, int destination) {
    int reward = (zone + 1) * (destination + 1) * 5;
    player->pearls += reward;
    printf(COLOR_YELLOW "✨ Vous gagnez %d perles !\n" COLOR_RESET, reward);
    return 1;
}

int fight_all_monsters(Player *player, int zone, int monsters_count)

{
    if (player == NULL || zone < 0 || zone >= MAX_ZONES || monsters_count <= 0)
    {
        print_error("Erreur lors de l'initialisation du combat.");
        return -1;
    }

    if (monsters_count > MAX_CREATURES_PER_ZONE)
        monsters_count = MAX_CREATURES_PER_ZONE;

    Creature creatures[MAX_CREATURES_PER_ZONE];
    memset(creatures, 0, sizeof(creatures));
    create_creatures_for_zone(creatures, monsters_count, zone);

    int result = start_combat(player, creatures, monsters_count);

    if (result == 1)
    {
        printf(COLOR_GREEN "\nVICTOIRE ! Vous avez vaincu toutes les créatures.\n" COLOR_RESET);
        int exp_gain = monsters_count * 15 + zone * 10;
        player_add_experience(player, exp_gain);
        return 1;
    }
    else
    {
        printf(COLOR_RED "\nDÉFAITE... Vous avez succombé aux profondeurs.\n" COLOR_RESET);
        return -1;
    }
}

void give_zone_rewards(Player *player, int zone, int destination)
{
    if (player == NULL || zone < 0 || zone >= MAX_ZONES || destination < 0 || destination >= 4)
        return;

    int base_reward = 10;
    int zone_multiplier = zone + 1;
    int dest_multiplier = destination + 1;
    int random_bonus = random_range(0, 5 * zone_multiplier);
    int pearl_reward = base_reward * zone_multiplier + dest_multiplier * 5 + random_bonus;

    if (pearl_reward > 0)
        player_add_pearls(player, pearl_reward);

    if (zone == 1 && destination == 1)
    {
        printf(COLOR_YELLOW "✨ Vous avez trouvé un trésor dans l'épave ! ✨\n" COLOR_RESET);
        int treasure_pearls = random_range(50, 100);
        player_add_pearls(player, treasure_pearls);
    }
    if (zone == 2 && destination == 0)
    {
        printf(COLOR_YELLOW "Vous récupérez une dent de requin ancienne !\n" COLOR_RESET);
        int boss_pearls = random_range(100, 150);
        player_add_pearls(player, boss_pearls);
    }
    if (zone == 2 && destination == 2)
    {
        printf(COLOR_YELLOW "Un morceau de tentacule luisant reste derrière...\n" COLOR_RESET);
        int boss_pearls = random_range(120, 180);
        player_add_pearls(player, boss_pearls);
    }
}