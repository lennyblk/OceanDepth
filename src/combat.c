#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/combat.h"
#include "../include/utils.h"
#include "../include/player.h"
#include "../include/constants.h"

static int compare_creatures_by_speed(const void *a, const void *b)
{
    Creature *creatureA = (Creature *)a;
    Creature *creatureB = (Creature *)b;

    return creatureB->speed - creatureA->speed;
}

static void handle_kraken_attack(Player *player, Creature *creature)
{
    int damage = 0;
    int oxygen_drain = 1;  // Réduit de random_range(1, 2) à 1

    printf(COLOR_MAGENTA "Étreinte tentaculaire du Kraken !\n" COLOR_RESET);
    damage = calculate_creature_damage(creature, player);
    player_take_damage(player, damage);
    player_use_oxygen(player, oxygen_drain);

    if (player_is_alive(player))
    {
        printf(COLOR_MAGENTA "Le Kraken frappe à nouveau !\n" COLOR_RESET);
        damage = calculate_creature_damage(creature, player);
        player_take_damage(player, damage);
        player_use_oxygen(player, oxygen_drain);
    }
}

static void handle_shark_attack(Player *player, Creature *creature)
{
    int oxygen_drain = 1;  // Réduit de random_range(1, 2) à 1
    int damage = calculate_creature_damage(creature, player);

    if (creature->hp_current < (creature->hp_max / 2))
    {
        printf(COLOR_RED "Frénésie sanguinaire du Requin !\n" COLOR_RESET);
        damage = (int)((float)damage * 1.20f);  // Réduit de 1.30 à 1.20
    }
    printf("%s attaque ! Vous perdez %d PV.\n", creature->name, damage);
    player_take_damage(player, damage);
    player_use_oxygen(player, oxygen_drain);
}

static void handle_jellyfish_attack(Player *player, Creature *creature)
{
    int oxygen_drain = 1;
    
    // Vérifier si le joueur est déjà paralysé OU si cette méduse a déjà appliqué son effet
    if (!player->is_paralyzed && creature->effect_duration == 0)
    {
        printf(COLOR_CYAN "Piqûre paralysante de la Méduse !\n" COLOR_RESET);
        int damage = calculate_creature_damage(creature, player);
        player_take_damage(player, damage);
        player_use_oxygen(player, oxygen_drain);
        
        player->is_paralyzed = 1;
        player->paralysis_turns_left = 1;
        creature->effect_duration = 1;  // Marquer que l'effet a été utilisé
        printf(COLOR_YELLOW "Vous êtes paralysé pour 1 tour !\n" COLOR_RESET);
    }
    else
    {
        // Attaque normale sans effet de statut
        printf(COLOR_CYAN "La Méduse vous frappe !\n" COLOR_RESET);
        int damage = calculate_creature_damage(creature, player);
        player_take_damage(player, damage);
        player_use_oxygen(player, oxygen_drain);
    }
}

static void handle_swordfish_attack(Player *player, Creature *creature)
{
    int oxygen_drain = 1;  // Réduit de random_range(1, 2) à 1
    printf(COLOR_BLUE "Charge perforante du Poisson-Épée !\n" COLOR_RESET);
    int defense_ignored = 2;
    int attack_power = random_range(creature->attack_min, creature->attack_max);
    int damage = attack_power - (player->equipped_armor.defense - defense_ignored);
    damage = (damage > 0) ? damage : 1;

    player_take_damage(player, damage);
    player_use_oxygen(player, oxygen_drain);
}

static void handle_default_attack(Player *player, Creature *creature)
{
    int oxygen_drain = 1;  // Réduit de random_range(1, 2) à 1
    int damage = calculate_creature_damage(creature, player);
    printf("%s attaque ! Vous perdez %d PV.\n", creature->name, damage);
    player_take_damage(player, damage);
    player_use_oxygen(player, oxygen_drain);
}

static int validate_skill_use(Player *player, Skill *skill)
{
    if (!skill->is_unlocked)
    {
        print_error("Compétence non débloquée.");
        return 0;
    }

    if (skill->cooldown_current > 0)
    {
        printf(COLOR_RED "Compétence en recharge pour %d tour(s).\n" COLOR_RESET, skill->cooldown_current);
        return 0;
    }

    if (player->oxygen < skill->oxygen_cost)
    {
        printf(COLOR_RED "Pas assez d'oxygène (%d requis).\n" COLOR_RESET, skill->oxygen_cost);
        return 0;
    }

    return 1;
}

static void execute_skill_breath(Player *player, Skill *skill)
{
    printf(COLOR_CYAN "Vous utilisez '%s' !\n" COLOR_RESET, skill->name);
    player_restore_oxygen(player, skill->oxygen_restore);
}

static void execute_skill_discharge(Skill *skill, Creature creatures[], int creature_count)
{
    printf(COLOR_YELLOW "Vous lancez '%s' !\n" COLOR_RESET, skill->name);
    int damage = random_range(skill->damage_min, skill->damage_max);
    printf("Toutes les créatures subissent %d dégâts.\n", damage);
    for (int i = 0; i < creature_count; i++)
    {
        if (creatures[i].is_alive)
        {
            creatures[i].hp_current -= damage;
            printf("%s subit %d points de dégâts.\n", creatures[i].name, damage);
            if (creatures[i].hp_current <= 0)
            {
                creatures[i].hp_current = 0;
                creatures[i].is_alive = 0;
                printf(COLOR_GREEN "%s a été vaincu !\n" COLOR_RESET, creatures[i].name);
            }
        }
    }
}

static void execute_skill_unimplemented(Skill *skill)
{
    printf("Compétence '%s' non implémentée.\n", skill->name);
    skill->cooldown_current = 0;
}

int start_combat(Player *player, Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);
    assert(creature_count > 0);

    int turn = 1;
    int combat_result = 0;
    player->fatigue = 0;

    while (combat_result == 0)
    {
        clear_screen();
        print_title("COMBAT SOUS-MARIN");
        display_combat_status(player, creatures, creature_count);

        if (player->is_paralyzed)
        {
            printf(COLOR_YELLOW "Vous êtes paralysé et ne pouvez pas bouger !\n" COLOR_RESET);
            player->paralysis_turns_left--;
            if (player->paralysis_turns_left <= 0)
            {
                player->is_paralyzed = 0;
                printf(COLOR_GREEN "Vous pouvez de nouveau bouger !\n" COLOR_RESET);
            }
        }
        else
        {
            if (player_turn(player, creatures, creature_count) == 0)
            {
                combat_result = -1;
                break;
            }
        }

        combat_result = check_combat_end(player, creatures, creature_count);
        if (combat_result != 0)
            break;

        creatures_turn(player, creatures, creature_count);

        combat_result = check_combat_end(player, creatures, creature_count);
        if (combat_result != 0)
            break;

        int oxygen_loss = random_range(1, 3) + player->current_zone;  // Réduit de (2, 5) + zone*2 à (1, 3) + zone
        player_use_oxygen(player, oxygen_loss);
        if (player->oxygen <= 0)
        {
            printf(COLOR_RED "Vous n'avez plus d'oxygène !\n" COLOR_RESET);
            if (player_is_alive(player))
                player_take_damage(player, 3);  // Réduit de 5 à 3
        }

        if (player->fatigue < 5)
            player->fatigue++;

        player_update_cooldowns(player);
        turn++;
        pause_screen();
    }
    return combat_result;
}

void display_combat_status(const Player *player, const Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);

    printf("--- PLONGEUR ---\n");
    printf("Vie: %d/%d | Oxygène: %d/%d | Fatigue: %d/5\n",
           player->hp, player->max_hp, player->oxygen, player->max_oxygen, player->fatigue);

    if (player->is_paralyzed)
    {
        printf(COLOR_YELLOW "[PARALYSÉ: %d tour(s)]\n" COLOR_RESET, player->paralysis_turns_left);
    }
    print_separator('=', 50);
    printf("--- CRÉATURES HOSTILES ---\n");
    for (int i = 0; i < creature_count; i++)
    {
        if (creatures[i].is_alive)
        {
            printf("[%d] %s (PV: %d/%d)\n", i + 1, creatures[i].name, creatures[i].hp_current, creatures[i].hp_max);
        }
    }
    print_separator('=', 50);
}

int get_player_attacks_per_turn(const Player *player)
{
    assert(player != NULL);
    // Le nombre d'actions sera déterminé dynamiquement dans player_turn
    return 1;
}

int player_turn(Player *player, Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);

    // Le joueur a autant d'actions que de créatures vivantes
    int attacks_left = count_alive_creatures(creatures, creature_count);

    while (attacks_left > 0)
    {
        if (check_combat_end(player, creatures, creature_count) != 0)
            return 1;

        display_combat_menu(attacks_left);
        char choice = get_char_input();

        switch (choice)
        {
        case '1':
        {
            Creature *target = select_target(creatures, creature_count);
            if (target != NULL)
            {
                attack_creature(player, target);
                attacks_left--;
                player_use_oxygen(player, player->equipped_weapon.oxygen_cost_per_attack);
            }
            break;
        }
        case '2':
            if (handle_skill_choice(player, creatures, creature_count) == 1)
                attacks_left--;
            pause_screen();
            break;
        case '3':
            printf("Inventaire non implémenté.\n");
            pause_screen();
            break;
        case '4':
            attacks_left = 0;
            break;
        default:
            print_error("Action invalide.");
            break;
        }

        if (check_combat_end(player, creatures, creature_count) != 0)
            attacks_left = 0;
    }
    return 1;
}

void creatures_turn(Player *player, Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);

    printf("\n--- TOUR DES CRÉATURES ---\n");

    qsort(creatures, (size_t)creature_count, sizeof(Creature), compare_creatures_by_speed);

    for (int i = 0; i < creature_count; i++)
    {
        if (!creatures[i].is_alive)
            continue;
        if (!player_is_alive(player))
            break;

        switch (creatures[i].type)
        {
        case CREATURE_KRAKEN:
            handle_kraken_attack(player, &creatures[i]);
            break;
        case CREATURE_SHARK:
            handle_shark_attack(player, &creatures[i]);
            break;
        case CREATURE_JELLYFISH:
            handle_jellyfish_attack(player, &creatures[i]);
            break;
        case CREATURE_SWORDFISH:
            handle_swordfish_attack(player, &creatures[i]);
            break;
        case CREATURE_GIANT_CRAB:
        default:
            handle_default_attack(player, &creatures[i]);
            break;
        }
    }
}

int attack_creature(Player *player, Creature *target)
{
    assert(player != NULL);
    assert(target != NULL);

    int damage = calculate_player_damage(player, target);
    target->hp_current -= damage;

    printf("Vous attaquez %s avec %s et infligez %d points de dégâts.\n",
           target->name, player->equipped_weapon.name, damage);

    if (target->hp_current <= 0)
    {
        target->hp_current = 0;
        target->is_alive = 0;
        printf(COLOR_GREEN "%s a été vaincu !\n" COLOR_RESET, target->name);
    }
    return 1;
}

int calculate_player_damage(const Player *player, const Creature *target)
{
    assert(player != NULL);
    assert(target != NULL);

    int attack_power = random_range(player->equipped_weapon.attack_min,
                                    player->equipped_weapon.attack_max);
    int damage = attack_power - target->defense;

    if (target->type == CREATURE_GIANT_CRAB)
    {
        printf(COLOR_BLUE "La Carapace Durcie du Crabe réduit les dégâts !\n" COLOR_RESET);
        damage = (int)((float)damage * 0.85f); 
    }
    return (damage > 0) ? damage : 1;
}

int calculate_creature_damage(const Creature *attacker, const Player *player)
{
    assert(attacker != NULL);
    assert(player != NULL);

    int attack_power = random_range(attacker->attack_min, attacker->attack_max);
    int damage = attack_power - player->equipped_armor.defense;
    return (damage > 0) ? damage : 1;
}

int check_combat_end(const Player *player, const Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);

    if (!player_is_alive(player))
        return -1;
    if (count_alive_creatures(creatures, creature_count) == 0)
        return 1;
    return 0;
}

void display_combat_menu(int attacks_left)
{
    printf("\nActions disponibles (%d restantes) :\n", attacks_left);
    printf("1 - Attaquer avec %s\n", "Harpon");
    printf("2 - Utiliser compétence marine\n");
    printf("3 - Consommer objet\n");
    printf("4 - Terminer le tour\n");
    printf("> ");
}

Creature *select_target(Creature creatures[], int creature_count)
{
    assert(creatures != NULL);

    int alive_creatures = count_alive_creatures(creatures, creature_count);
    if (alive_creatures == 0)
        return NULL;

    if (alive_creatures == 1)
    {
        for (int i = 0; i < creature_count; i++)
        {
            if (creatures[i].is_alive)
                return &creatures[i];
        }
    }

    while (1)
    {
        printf("Choisissez une cible (1-%d) : ", creature_count);
        int target_choice = get_int_input(1, creature_count);

        if (target_choice > 0 && target_choice <= creature_count && creatures[target_choice - 1].is_alive)
        {
            return &creatures[target_choice - 1];
        }
        print_error("Cible invalide.");
    }
}

int count_alive_creatures(const Creature creatures[], int creature_count)
{
    assert(creatures != NULL);
    int count = 0;
    for (int i = 0; i < creature_count; i++)
    {
        if (creatures[i].is_alive)
            count++;
    }
    return count;
}

void display_skills_menu(const Player *player)
{
    assert(player != NULL);
    clear_screen();
    print_title("COMPÉTENCES MARINES");

    int available_skills = 0;
    for (int i = 0; i < SKILL_COUNT; i++)
    {
        if (player->skills[i].is_unlocked)
        {
            printf("  [%d] %s (Coût O2: %d)", i + 1, player->skills[i].name, player->skills[i].oxygen_cost);
            if (player->skills[i].cooldown_current > 0)
                printf(COLOR_YELLOW " [Recharge: %d tour(s)]\n" COLOR_RESET, player->skills[i].cooldown_current);
            else
                printf(COLOR_GREEN " [DISPONIBLE]\n" COLOR_RESET);
            available_skills++;
        }
    }

    if (available_skills == 0)
        print_info("Vous n'avez débloqué aucune compétence.");

    printf("\n  [0] Annuler\n");
}

int handle_skill_choice(Player *player, Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);

    display_skills_menu(player);
    printf("\nChoisissez une compétence (1-%d) ou 0 pour annuler: ", SKILL_COUNT);
    int choice = get_int_input(0, SKILL_COUNT);

    if (choice == 0)
        return 0;

    int skill_index = choice - 1;
    Skill *skill = &player->skills[skill_index];

    if (validate_skill_use(player, skill) == 0)
        return 0;

    if (player_use_oxygen(player, skill->oxygen_cost) == 0)
        return 0;

    skill->cooldown_current = skill->cooldown_max;

    switch (skill->type)
    {
    case SKILL_EXTENDED_BREATH:
        execute_skill_breath(player, skill);
        break;
    case SKILL_ELECTRIC_DISCHARGE:
        execute_skill_discharge(skill, creatures, creature_count);
        break;
    case SKILL_MARINE_COMMUNICATION:
    case SKILL_WATER_VORTEX:
        execute_skill_unimplemented(skill);
        return 0;
    case SKILL_COUNT:
    default:
        print_error("Erreur de compétence inconnue.");
        skill->cooldown_current = 0;
        return 0;
    }
    return 1;
}