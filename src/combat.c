#include "combat.h"
#include "utils.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

        if (player_turn(player, creatures, creature_count) == 0)
        {
            combat_result = -1;
            break;
        }

        combat_result = check_combat_end(player, creatures, creature_count);
        if (combat_result != 0)
            break;

        creatures_turn(player, creatures, creature_count);

        combat_result = check_combat_end(player, creatures, creature_count);
        if (combat_result != 0)
            break;

        player->oxygen -= random_range(2, 5);
        if (player->oxygen < 0)
            player->oxygen = 0;

        if (player->fatigue < 5)
        {
            player->fatigue++;
        }

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
    printf("Vie: %d/%d | Oxygène: %d/%d | Fatigue: %d/5\n", player->hp, player->max_hp, player->oxygen, player->max_oxygen, player->fatigue);
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

int player_turn(Player *player, Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);

    int attacks_left = get_player_attacks_per_turn(player);

    while (attacks_left > 0)
    {
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
                player->oxygen -= random_range(2, 4);
                if (player->oxygen < 0)
                    player->oxygen = 0;
            }
            break;
        }
        case '2':
            printf("Compétences non implémentées.\n");
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
        {
            return 1;
        }
    }
    return 1;
}

void creatures_turn(Player *player, Creature creatures[], int creature_count)
{
    assert(player != NULL);
    assert(creatures != NULL);

    printf("\n--- TOUR DES CRÉATURES ---\n");

    for (int i = 0; i < creature_count; i++)
    {
        if (creatures[i].is_alive)
        {
            int damage = calculate_creature_damage(&creatures[i], player);
            printf("%s attaque ! Vous perdez %d PV.\n", creatures[i].name, damage);
            player_take_damage(player, damage);

            if (!player_is_alive(player))
                break;
        }
    }
}

int get_player_attacks_per_turn(const Player *player)
{
    assert(player != NULL);
    if (player->fatigue <= 1)
        return 3;
    if (player->fatigue <= 3)
        return 2;
    return 1;
}

int attack_creature(Player *player, Creature *target)
{
    assert(player != NULL);
    assert(target != NULL);

    int damage = calculate_player_damage(player, target);
    target->hp_current -= damage;

    printf("Vous attaquez %s et infligez %d points de dégâts.\n", target->name, damage);

    if (target->hp_current <= 0)
    {
        target->hp_current = 0;
        target->is_alive = 0;
        printf("%s a été vaincu !\n", target->name);
    }

    return 1;
}

int calculate_player_damage(const Player *player, const Creature *target)
{
    assert(player != NULL);
    assert(target != NULL);

    int attack_power = random_range(10, 20);
    int damage = attack_power - target->defense;

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
    {
        return -1;
    }

    if (count_alive_creatures(creatures, creature_count) == 0)
    {
        return 1;
    }

    return 0;
}

void display_combat_menu(int attacks_left)
{
    printf("\nActions disponibles (%d restantes) :\n", attacks_left);
    printf("1 - Attaquer avec harpon\n");
    printf("2 - Utiliser compétence marine\n");
    printf("3 - Consommer objet\n");
    printf("4 - Terminer le tour\n");
    printf("> ");
}

Creature *select_target(Creature creatures[], int creature_count)
{
    assert(creatures != NULL);

    int target_choice = -1;
    int alive_creatures = count_alive_creatures(creatures, creature_count);

    if (alive_creatures == 0)
        return NULL;

    while (1)
    {
        printf("Choisissez une cible (1-%d) : ", creature_count);
        target_choice = get_int_input(1, creature_count);

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
        {
            count++;
        }
    }
    return count;
}