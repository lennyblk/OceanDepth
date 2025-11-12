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
#include "../include/combat.h"
#include "../include/save.h"
#include "../include/creature.h"
#include "../include/shop.h"
#include "../include/inventory.h"
#include "../include/gear.h"

static void display_map_header(void)
{
    printf(COLOR_BOLD "                    CARTOGRAPHIE OCÉANIQUE - SECTEUR PACIFIQUE\n" COLOR_RESET);
    print_separator('-', 80);
}

static void display_map_row(Player *player, Map *map, int zone, const char *destinations[4][4])
{
    char zone_name[20];
    char zone_depth[20];
    
    if (zone == 0)
    {
        strcpy(zone_name, "SURFACE");
        strcpy(zone_depth, "0m");
    }
    else if (zone <= 3)
    {
        snprintf(zone_name, sizeof(zone_name), "ZONE %d", zone);
        snprintf(zone_depth, sizeof(zone_depth), "-%dm", zone * 50);
    }
    else
    {
        snprintf(zone_name, sizeof(zone_name), "ABYSSE %d", zone - 3);
        snprintf(zone_depth, sizeof(zone_depth), "-%dm", zone * 50);
    }

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
    printf(" %s | %s\n", zone_name, zone_depth);
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
    printf("⚠️  Vous détectez des créatures hostiles dans cette zone !\n");
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

void display_title_screen()
{
    clear_screen();

    // ASCII Art du titre
    printf(COLOR_CYAN COLOR_BOLD);
    printf("  ███████╗ ██████╗ ███████╗ █████╗ ███╗   ██╗    ██████╗ ███████╗██████╗ ████████╗██╗  ██╗\n");
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
    printf("║  " COLOR_GREEN "2." COLOR_RESET " 📊 Voir les statistiques                 " COLOR_GREEN "7." COLOR_RESET " 🛡️  Équipement et armes                       ║\n");
    printf("║  " COLOR_GREEN "3." COLOR_RESET " 🏪 Visiter le marché                     " COLOR_GREEN "8." COLOR_RESET " 💾 Sauvegarder la partie                      ║\n");
    printf("║  " COLOR_GREEN "4." COLOR_RESET " 🌊 Se reposer en surface                 " COLOR_GREEN "9." COLOR_RESET " 📖 Consulter le journal                       ║\n");
    printf("║  " COLOR_GREEN "5." COLOR_RESET " ???                                      " COLOR_GREEN "0." COLOR_RESET " ❌ Quitter le jeu                             ║\n");
    printf("║                                                                                             ║\n");
    printf("╚═════════════════════════════════════════════════════════════════════════════════════════════╝\n");

    printf("\n" COLOR_BOLD "Choisissez votre action: " COLOR_RESET);

    char choice = get_char_input();
    return (choice >= '0' && choice <= '9') ? (choice - '0') : -1;
}

int handle_menu_choice(int choice, Player *player, Map *map, int *game_time)
{
    switch (choice)
    {
    case 1:
        explore_map(player, map);
        break;
    case 2:
        player_display_stats(player);
        pause_screen();
        break;
    case 3:
        visit_shop(player);
        break;
    case 4:
        rest_at_surface(player);
        break;
    case 5:
        sananes();
        break;
    case 6:
        manage_inventory(player);
        break;
    case 7:
        manage_equipment(player);
        break;
    case 8:
        save_game(player, map, *game_time);
        pause_screen();
        break;
    case 9:
        display_journal(player);
        break;
    case 0:
        if (confirm_action("Êtes-vous sûr de vouloir quitter ?"))
        {
            printf(COLOR_CYAN "Merci d'avoir joué à Ocean Depths !\n" COLOR_RESET);
            return 0;
        }
        break;
    default:
        print_error("Choix invalide !");
        pause_screen();
        break;
    }
    return 1; // Continuer le jeu
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
        break;
    case '3': // Choisir une destination
        select_destination(player, map);
        explore_map(player, map);
        break;
    case '0': // Retour au menu
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
    const char *base_destinations[4][4] = {
        {"🚤 Base", "🌊 Océan", "🌊 Océan", "🚤 Bateau"},
        {"🪸 Récif", "💰 Épave", "🌿 Algues", "🕳️ Grotte"},
        {"🦈 Requin", "❌ Vide", "🦑 Kraken", "❌ Vide"},
        {"❓ Abysse", "❓ Abysse", "❓ Abysse", "❓ Abysse"}
    };
    
    const char *abyss_destinations[4] = {"💀 Danger", "💀 Danger", "💀 Danger", "💀 Danger"};

    display_map_header();
    
    int zones_to_display = (player->current_zone + 2 > map->zone_count) ? map->zone_count : player->current_zone + 2;
    if (zones_to_display > map->zone_count) zones_to_display = map->zone_count;
    
    for (int zone = 0; zone < zones_to_display; zone++)
    {
        const char *destinations[4][4];
        
        if (zone < 4)
        {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    destinations[i][j] = base_destinations[i][j];
        }
        else
        {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    destinations[i][j] = abyss_destinations[j];
        }
        
        display_map_row(player, map, zone, destinations);
    }
    
    display_map_legend();
    
    if (player->current_zone >= 4)
    {
        printf("\n" COLOR_RED "⚠️  VOUS ÊTES DANS LES ABYSSES PROFONDS - ZONE %d\n" COLOR_RESET, player->current_zone);
        printf(COLOR_YELLOW "Les créatures sont %d%% plus puissantes qu'en surface !\n" COLOR_RESET, 
               (player->current_zone - 3) * 10);
    }
}

void select_destination(Player *player, Map *map)
{
    clear_screen();

    // Destinations dynamiques
    const char *destinations[4];

    if (player->current_zone == 0)
    {
        destinations[0] = "🚤 Base";
        destinations[1] = "🌊 Océan";
        destinations[2] = "🌊 Océan";
        destinations[3] = "🚤 Bateau";
    }
    else if (player->current_zone == 1)
    {
        destinations[0] = "🪸 Récif";
        destinations[1] = "💰 Épave";
        destinations[2] = "🌿 Algues";
        destinations[3] = "🕳️ Grotte";
    }
    else if (player->current_zone == 2)
    {
        destinations[0] = "??";
        destinations[1] = "🌿 Algues";
        destinations[2] = "??";
        destinations[3] = "🪸 Récif";
    }
    else
    {
        // Zones 3+ : toutes des destinations dangereuses
        destinations[0] = "💀 Abysses Profonds";
        destinations[1] = "💀 Abysses Profonds";
        destinations[2] = "💀 Abysses Profonds";
        destinations[3] = "💀 Abysses Profonds";
    }

    printf(COLOR_CYAN COLOR_BOLD "🎯 SÉLECTION DE DESTINATION - ZONE %d\n" COLOR_RESET, player->current_zone);
    print_separator('=', 60);

    printf("Destinations disponibles:\n\n");

    for (int i = 0; i < 4; i++)
    {
        if (is_destination_available(player, map, player->current_zone, i))
        {
            if (is_destination_cleared(map, player->current_zone, i))
                printf(COLOR_GREEN "%d." COLOR_RESET " %s " COLOR_GREEN "[TERMINÉ]" COLOR_RESET "\n",
                       i + 1, destinations[i]);
            else
                printf(COLOR_GREEN "%d." COLOR_RESET " %s\n", i + 1, destinations[i]);
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
        {
            enter_destination(player, map, player->current_zone, dest);
        }
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
        {"Zone inconnue", "Zone inconnue", "Zone inconnue", "Zone inconnue"}
    };

    printf(COLOR_CYAN COLOR_BOLD "🏊 EXPLORATION: %s\n" COLOR_RESET, dest_names[zone][destination]);
    print_separator('=', 60);

    if (is_destination_cleared(map, zone, destination))
    {
        handle_cleared_destination(player);
    }
    else
    {
        int monsters_count = random_range(1, 4);
        
        if (zone == 0 && (destination == 0 || destination == 3))
            monsters_count = 0;
        else if (zone == 2 && (destination == 0 || destination == 2))
            monsters_count = 1;
        
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
    return depths[zone];
}

int is_zone_unlocked(const Player *player, int zone)
{
    if (player == NULL || zone < 0 || zone >= MAX_ZONES)
        return 0;
    if (zone == 0)
        return 1;
    return player->zones_unlocked > zone;
}

int is_destination_available(const Player *player, Map *map, int zone, int destination)
{
    (void)map; // Supprime l'avertissement de paramètre non utilisé
    // Zone 2 a des destinations vides
    if (zone == 2 && (destination == 1 || destination == 3))
    {
        return false;
    }
    return is_zone_unlocked((Player *)player, zone);
}

int is_destination_cleared(Map *map, int zone, int destination)
{
    return map->zones[zone].destinations[destination].cleared;
}

void mark_destination_cleared(Map *map, int zone, int destination)
{
    map->zones[zone].destinations[destination].cleared = 1;
}

int get_monsters_in_destination(int zone, int destination)
{
    // Placeholder implementation
    (void)zone;
    (void)destination;
    return 0;
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
    return true;
}

void unlock_next_zone(Player *player, Map *map, int current_zone)
{
    if (player == NULL || map == NULL || current_zone < 0)
        return;

    int next_zone = current_zone + 1;

    if (next_zone >= map->zone_count)
    {
        generate_new_zone(map, next_zone);
        map->zone_count = next_zone + 1;
        printf(COLOR_YELLOW "\n🌊 LES ABYSSES S'APPROFONDISSENT !\n" COLOR_RESET);
        printf("Une nouvelle zone insondable se révèle devant vous...\n");
        printf(COLOR_RED "Les créatures y sont plus puissantes que jamais !\n" COLOR_RESET);
    }
    else
    {
        printf(COLOR_YELLOW "\n🔓 NOUVELLE ZONE DÉBLOQUÉE !\n" COLOR_RESET);
        printf("Vous pouvez maintenant accéder à la zone %d !\n", next_zone);
    }

    player->zones_unlocked = next_zone + 1;
    pause_screen();
}

void rest_at_surface(Player *player)
{
    printf("Vous vous reposez en surface...\n");
    player->hp = player->max_hp;
    player->oxygen = player->max_oxygen;
    printf(COLOR_GREEN "❤ HP et 💨 Oxygène restaurés !\n" COLOR_RESET);
    pause_screen();
}

void display_journal(Player *player)
{
    if (player == NULL)
        return;

    clear_screen();

    printf(COLOR_CYAN COLOR_BOLD);
    printf("╔═══════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                            📖 JOURNAL DE BORD - OCEAN DEPTHS                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf(COLOR_RESET);

    printf("\n" COLOR_YELLOW "═══ VOTRE HISTOIRE ═══" COLOR_RESET "\n\n");

    printf("Vous êtes %s, un plongeur des profondeurs devenu chasseur de monstres marins.\n", player->name);
    printf("Après la disparition mystérieuse de votre équipage lors d'une expédition,\n");
    printf("vous avez juré de découvrir les secrets cachés dans les abysses.\n\n");

    printf("Les légendes parlent d'un " COLOR_RED "Kraken ancestral" COLOR_RESET " tapi dans les profondeurs,\n");
    printf("gardien d'un trésor inestimable et responsable de nombreux naufrages.\n\n");

    printf(COLOR_BOLD "Votre mission : " COLOR_RESET "Descendre toujours plus profond, affronter les créatures\n");
    printf("des abysses, et prouver que vous êtes le plus grand chasseur sous-marin !\n\n");

    print_separator('=', 87);
    printf("\n" COLOR_YELLOW "═══ COMMENT JOUER ═══" COLOR_RESET "\n\n");

    printf(COLOR_CYAN "🗺️  EXPLORATION" COLOR_RESET "\n");
    printf("   • Naviguez entre 4 zones de profondeur croissante (Surface → Abysses)\n");
    printf("   • Chaque zone contient 4 destinations à explorer\n");
    printf("   • Terminez toutes les destinations pour débloquer la zone suivante\n");
    printf("   • " COLOR_RED "ATTENTION :" COLOR_RESET " Plus vous descendez, plus les créatures sont dangereuses !\n\n");

    printf(COLOR_RED "⚔️  COMBAT" COLOR_RESET "\n");
    printf("   • Vous avez AUTANT d'actions que d'ennemis vivants par tour\n");
    printf("   • Attaquez avec votre harpon ou utilisez des compétences marines\n");
    printf("   • Gérez votre " COLOR_CYAN "oxygène" COLOR_RESET " : chaque action en consomme !\n");
    printf("   • Si votre oxygène atteint 0, vous perdez des PV chaque tour\n\n");

    printf(COLOR_YELLOW "💡 PROGRESSION" COLOR_RESET "\n");
    printf("   • Gagnez de l'XP en vainquant des créatures\n");
    printf("   • Montez de niveau pour augmenter vos PV et votre oxygène max\n");
    printf("   • Collectez des " COLOR_YELLOW "perles" COLOR_RESET " pour acheter équipement et améliorations\n\n");

    printf(COLOR_GREEN "🌊 REPOS & RESSOURCES" COLOR_RESET "\n");
    printf("   • Retournez à la surface (Zone 0) pour restaurer PV et oxygène\n");
    printf("   • Certaines destinations sont des zones sûres sans combat\n");
    printf("   • Explorez les épaves pour trouver des trésors cachés\n\n");

    print_separator('=', 87);
    printf("\n" COLOR_YELLOW "═══ TYPES DE CRÉATURES ═══" COLOR_RESET "\n\n");

    printf("💧 " COLOR_CYAN "Méduse Bleue" COLOR_RESET " (Zones 0-1)\n");
    printf("   Attaque paralysante - Vous paralyse pour 1 tour\n\n");

    printf("🗡️  " COLOR_BLUE "Espadon" COLOR_RESET " (Zones 0-1)\n");
    printf("   Charge perforante - Ignore une partie de votre défense\n\n");

    printf("🦈 " COLOR_RED "Requin-Tigre" COLOR_RESET " (Zones 2+)\n");
    printf("   Frénésie sanguinaire - Plus dangereux quand blessé\n\n");

    printf("🦀 " COLOR_YELLOW "Crabe Géant" COLOR_RESET " (Zones 2+)\n");
    printf("   Carapace durcie - Très résistant aux dégâts\n\n");

    printf("🦑 " COLOR_MAGENTA "KRAKEN" COLOR_RESET " (Zones 3+)\n");
    printf("   " COLOR_RED "BOSS LÉGENDAIRE" COLOR_RESET " - Attaque DEUX fois par tour !\n\n");

    print_separator('=', 87);
    printf("\n" COLOR_YELLOW "═══ OBJECTIF FINAL ═══" COLOR_RESET "\n\n");

    printf(COLOR_BOLD "Survivez le plus longtemps possible dans les abysses infinis !\n" COLOR_RESET);
    printf("Chaque zone au-delà de la 3 augmente la puissance des créatures de +10%%.\n");
    printf("Jusqu'où oserez-vous descendre, %s ?\n\n", player->name);

    printf(COLOR_GREEN "Bonne chance, chasseur des profondeurs ! 🌊\n\n" COLOR_RESET);

    pause_screen();
    // return 1; // Victoire
}

void create_creatures_for_zone(Creature creatures[], int creature_count, int zone)
{
    if (creature_count <= 0 || creature_count > MAX_CREATURES_PER_ZONE)
        return;

    int zone_depth = zone * 50;
    size_t generated_count;
    generate_creatures(zone_depth, creatures, (size_t)creature_count, &generated_count);

    // Afficher le nombre réel de créatures générées
    printf(COLOR_RED "⚔️  %zu créature(s) hostile(s) apparaissent !\n" COLOR_RESET, generated_count);
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
    if (player == NULL || zone < 0 || destination < 0)
        return;

    int base_reward = 10;
    int zone_multiplier = zone + 1;
    int dest_multiplier = destination + 1;
    int random_bonus = random_range(0, 5 * zone_multiplier);
    int pearl_reward = base_reward * zone_multiplier + dest_multiplier * 5 + random_bonus;

    if (pearl_reward > 0)
        player->pearls += pearl_reward;

    if (zone == 1 && destination == 1)
    {
        printf(COLOR_YELLOW "✨ Vous avez trouvé un trésor dans l'épave ! ✨\n" COLOR_RESET);
        int treasure_pearls = random_range(50, 100);
        player->pearls += treasure_pearls;
    }
}