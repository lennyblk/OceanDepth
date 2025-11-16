#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/types.h"
#include "../include/ascii_art.h"
#include "../include/weapon.h"
#include "../include/armor.h"

int get_zone_from_depth(int depth);

void player_init_skills(Player *player)
{
    if (player == NULL)
    {
        return;
    }

    Skill *skill_breath = &player->skills[SKILL_EXTENDED_BREATH];
    skill_breath->type = SKILL_EXTENDED_BREATH;
    strncpy(skill_breath->name, "Apnée Prolongée", 39);
    skill_breath->oxygen_cost = 0;
    skill_breath->oxygen_restore = 20;
    skill_breath->cooldown_max = 4;
    skill_breath->cooldown_current = 0;
    skill_breath->is_unlocked = 1;

    Skill *skill_discharge = &player->skills[SKILL_ELECTRIC_DISCHARGE];
    skill_discharge->type = SKILL_ELECTRIC_DISCHARGE;
    strncpy(skill_discharge->name, "Décharge Électrique", 39);
    skill_discharge->oxygen_cost = 18;
    skill_discharge->damage_min = 20;
    skill_discharge->damage_max = 30;
    skill_discharge->hits_all_enemies = 1;
    skill_discharge->cooldown_max = 5;
    skill_discharge->cooldown_current = 0;
    skill_discharge->is_unlocked = 1;

    Skill *skill_comm = &player->skills[SKILL_MARINE_COMMUNICATION];
    skill_comm->type = SKILL_MARINE_COMMUNICATION;
    strncpy(skill_comm->name, "Communication Marine", 39);
    skill_comm->oxygen_cost = 8;
    skill_comm->pacifies_enemy = 1;
    skill_comm->cooldown_max = 3;
    skill_comm->cooldown_current = 0;
    skill_comm->is_unlocked = 1; // MODIFIÉ (précédemment 0)

    Skill *skill_vortex = &player->skills[SKILL_WATER_VORTEX];
    skill_vortex->type = SKILL_WATER_VORTEX;
    strncpy(skill_vortex->name, "Tourbillon Aquatique", 39);
    skill_vortex->oxygen_cost = 22;
    skill_vortex->slows_enemies = 1;
    skill_vortex->cooldown_max = 6;
    skill_vortex->cooldown_current = 0;
    skill_vortex->is_unlocked = 1; // MODIFIÉ (précédemment 0)
}

void player_init(Player *player)
{
    if (player == NULL)
    {
        print_error("Pointeur player NULL dans player_init");
        return;
    }

    player->hp = 120;
    player->max_hp = 120;
    player->oxygen = 120;
    player->max_oxygen = 120;
    player->pearls = STARTING_PEARLS;
    player->experience = STARTING_EXPERIENCE;
    player->level = STARTING_LEVEL;
    player->rest_count = 0;

    player->x = 0;
    player->y = 0;
    player->current_zone = SURFACE_DEPTH;
    strcpy(player->name, "Monster Slayer");
    player->zones_unlocked = 1;
    player->inventory_count = 0;

    player->equipped_weapon = get_weapon_stats(WEAPON_RUSTY_HARPOON);
    player->equipped_armor = get_armor_stats(ARMOR_BASIC_SUIT);

    player->is_paralyzed = 0;
    player->is_poisoned = 0;
    player->paralysis_turns_left = 0;
    player->poison_turns_left = 0;

    player->attacks_reduced_next_turn = 0;

    player_init_skills(player);

    print_success("Joueur initialisé avec succès!");
}

void player_init_with_name(Player *player, const char *name)
{
    player_init(player);

    if (name != NULL && strlen(name) > 0)
    {
        strncpy(player->name, name, MAX_NAME_LENGTH - 1);
        player->name[MAX_NAME_LENGTH - 1] = '\0';
    }
}

void player_display_stats(const Player *player)
{
    if (player == NULL)
    {
        print_error("Pointeur player NULL dans player_display_stats");
        return;
    }

    clear_screen();
    // Affichage de l'ASCII art du joueur

    display_player_ascii();

    print_title("STATISTIQUES DU JOUEUR");

    printf(COLOR_BOLD "Nom: " COLOR_RESET "%s\n", player->name);
    printf(COLOR_BOLD "Niveau: " COLOR_RESET "%d\n", player->level);
    printf(COLOR_BOLD "Expérience: " COLOR_RESET "%d\n", player->experience);
    printf(COLOR_BOLD "Perles: " COLOR_RESET COLOR_YELLOW "%d" COLOR_RESET "\n", player->pearls);

    printf("\n" COLOR_BOLD "ÉTAT PHYSIQUE:" COLOR_RESET "\n");

    printf(COLOR_BOLD "Santé: " COLOR_RESET);
    print_progress_bar(player->hp, player->max_hp, 20);
    printf("\n");

    printf(COLOR_BOLD "Oxygène: " COLOR_RESET);
    print_progress_bar(player->oxygen, player->max_oxygen, 20);
    printf("\n");

    print_separator('-', 50);
}

int player_take_damage(Player *player, int damage)
{
    if (player == NULL)
    {
        print_error("Pointeur player NULL dans player_take_damage");
        return 0;
    }

    if (damage <= 0)
    {
        return 1;
    }

    player->hp -= damage;
    if (player->hp < 0)
    {
        player->hp = 0;
    }

    printf(COLOR_RED "%s subit %d points de dégâts!" COLOR_RESET "\n",
           player->name, damage);

    if (player->hp == 0)
    {
        printf(COLOR_RED "%s est inconscient!" COLOR_RESET "\n", player->name);
        return 0;
    }

    return 1;
}

void player_heal(Player *player, int heal_amount)
{
    if (player == NULL)
    {
        print_error("Pointeur player NULL dans player_heal");
        return;
    }

    if (heal_amount <= 0)
    {
        return;
    }

    int old_hp = player->hp;
    player->hp += heal_amount;

    if (player->hp > player->max_hp)
    {
        player->hp = player->max_hp;
    }

    int actual_heal = player->hp - old_hp;

    if (actual_heal > 0)
    {
        printf(COLOR_GREEN "%s récupère %d points de santé!" COLOR_RESET "\n",
               player->name, actual_heal);
    }
    else
    {
        printf(COLOR_YELLOW "%s est déjà en pleine santé!" COLOR_RESET "\n",
               player->name);
    }
}

// Consomme de l'oxygène
int player_use_oxygen(Player *player, int oxygen_cost)
{
    if (player == NULL)
    {
        print_error("Pointeur player NULL dans player_use_oxygen");
        return 0;
    }

    if (oxygen_cost <= 0)
    {
        return 1;
    }

    player->oxygen -= oxygen_cost;
    
    // Bloquer l'oxygène à 0 minimum (pas de valeurs négatives)
    if (player->oxygen < 0)
    {
        player->oxygen = 0;
    }

    if (player->oxygen == 0)
    {
        printf(COLOR_RED "ATTENTION: Vous n'avez plus d'oxygène!" COLOR_RESET "\n");
    }
    else if (player->oxygen <= 10)
    {
        printf(COLOR_RED "ATTENTION: Niveau d'oxygène critique! (%d%%)" COLOR_RESET "\n",
               player->oxygen);
    }

    return 1;  // On retourne toujours 1 pour permettre l'action
}

void player_restore_oxygen(Player *player, int oxygen_amount)
{
    if (player == NULL)
    {
        print_error("Pointeur player NULL dans player_restore_oxygen");
        return;
    }

    if (oxygen_amount <= 0)
    {
        return;
    }

    int old_oxygen = player->oxygen;
    player->oxygen += oxygen_amount;

    if (player->oxygen > player->max_oxygen)
    {
        player->oxygen = player->max_oxygen;
    }

    int actual_restore = player->oxygen - old_oxygen;

    if (actual_restore > 0)
    {
        printf(COLOR_CYAN "%s récupère %d%% d'oxygène!" COLOR_RESET "\n",
               player->name, actual_restore);
    }
}

void player_move(Player *player, int delta_x, int delta_y)
{
    if (player == NULL)
    {
        print_error("Pointeur player NULL dans player_move");
        return;
    }

    player->x += delta_x;
    player->y += delta_y;

    // Mise à jour de la zone basée sur Y
    player->current_zone = get_zone_from_depth(player->y * 10);
    if (player->current_zone < 0)
    {
        player->current_zone = 0;
    }

    // Consommation d'oxygène réduite
    int oxygen_cost = 1 + (player->current_zone * 5 / 100); // Réduit de 10 à 5
    player_use_oxygen(player, oxygen_cost);

    printf(COLOR_BLUE "%s se déplace à la position (%d, %d) - Zone: %d" COLOR_RESET "\n",
           player->name, player->x, player->y, player->current_zone);
}

int player_is_alive(const Player *player)
{
    if (player == NULL)
    {
        return 0;
    }

    return (player->hp > 0) ? 1 : 0;
}

void player_add_experience(Player *player, int exp)
{
    if (player == NULL || exp <= 0)
    {
        return;
    }

    player->experience += exp;
    printf(COLOR_GREEN "%s gagne %d points d'expérience!" COLOR_RESET "\n",
           player->name, exp);

    int required_exp = player->level * 100;

    while (player->experience >= required_exp)
    {
        player->level++;
        player->experience -= required_exp;

        int hp_bonus = 15;
        int oxygen_bonus = 10;

        player->max_hp += hp_bonus;
        player->hp = player->max_hp;
        player->max_oxygen += oxygen_bonus;
        player->oxygen = player->max_oxygen;

        printf(COLOR_YELLOW "🎉 %s monte au niveau %d!" COLOR_RESET "\n",
               player->name, player->level);
        printf(COLOR_GREEN "Bonus: +%d HP max, +%d Oxygène max" COLOR_RESET "\n",
               hp_bonus, oxygen_bonus);

        required_exp = player->level * 100;
    }
}

// Ajoute des perles
void player_add_pearls(Player *player, int pearls)
{
    if (player == NULL || pearls <= 0)
    {
        return;
    }

    player->pearls += pearls;
    printf(COLOR_YELLOW "%s trouve %d perles! (Total: %d)" COLOR_RESET "\n",
           player->name, pearls, player->pearls);
}

void player_update_cooldowns(Player *player)
{
    if (player == NULL)
    {
        return;
    }

    for (int i = 0; i < SKILL_COUNT; i++)
    {
        if (player->skills[i].cooldown_current > 0)
        {
            player->skills[i].cooldown_current--;
        }
    }
}

int get_zone_from_depth(int depth)
{
    if (depth <= 0)
    {
        return 0; // Surface
    }
    if (depth <= 50)
    {
        return 1; // Zone 1
    }
    if (depth <= 150)
    {
        return 2; // Zone 2
    }
    return 3; // Zone 3 (abysse) pour toutes les autres profondeurs > 150
}