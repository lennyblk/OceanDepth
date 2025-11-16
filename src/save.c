#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/save.h"
#include "../include/types.h"

int save_game(const Player *player, const Map *map, int game_time)
{
    FILE *file = NULL;
    SaveData save_data;
    memset(&save_data, 0, sizeof(SaveData));
    size_t written = 0;
    int close_status = 0;
    Zone current_zone;
    assert(player != NULL);
    assert(map != NULL);

    if (player == NULL || map == NULL)
    {
        fprintf(stderr, "Erreur: Pointeur NULL dans save_game\n");
        return 0;
    }
    assert(player->current_zone >= 0 && player->current_zone < 100);

    current_zone = map->zones[player->current_zone];
    if (!current_zone.can_save)
    {
        fprintf(stderr, "\n⚠️  SAUVEGARDE IMPOSSIBLE ⚠️\n");
        fprintf(stderr, "Vous ne pouvez sauvegarder qu'en surface ou dans une grotte sécurisée.\n");
        fprintf(stderr, "Zone actuelle: %s - Sauvegarde non autorisée.\n", current_zone.name);
        return 0;
    }

    file = fopen(SAVE_FILE_NAME, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur: Impossible de créer le fichier de sauvegarde\n");
        return 0;
    }

    save_data.game_time = game_time;
    save_data.save_version = 1; // Version de sauvegarde pour compatibilité future
    memcpy(&save_data.player, player, sizeof(Player));
    memcpy(&save_data.map, map, sizeof(Map));

    written = fwrite(&save_data, sizeof(SaveData), 1, file);

    close_status = fclose(file);
    if (close_status != 0)
    {
        fprintf(stderr, "Avertissement: Erreur lors de la fermeture du fichier\n");
    }

    if (written != 1)
    {
        fprintf(stderr, "Erreur: Échec de l'écriture de la sauvegarde\n");
        return 0;
    }

    printf("\n=== Partie sauvegardée avec succès ===\n");
    return 1;
}

int load_game(Player *player, Map *map, int *game_time)
{
    FILE *file = NULL;
    SaveData save_data;
    size_t read_count = 0;
    int close_status = 0;

    assert(player != NULL);
    assert(map != NULL);
    assert(game_time != NULL);

    if (player == NULL || map == NULL || game_time == NULL)
    {
        fprintf(stderr, "Erreur: Pointeur NULL dans load_game\n");
        return 0;
    }

    if (!save_exists())
    {
        fprintf(stderr, "Erreur: Aucune sauvegarde trouvée\n");
        return 0;
    }

    file = fopen(SAVE_FILE_NAME, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier de sauvegarde\n");
        return 0;
    }

    read_count = fread(&save_data, sizeof(SaveData), 1, file);

    close_status = fclose(file);
    if (close_status != 0)
    {
        fprintf(stderr, "Avertissement: Erreur lors de la fermeture du fichier\n");
    }

    if (read_count != 1)
    {
        fprintf(stderr, "Erreur: Échec de la lecture de la sauvegarde\n");
        return 0;
    }

    memcpy(player, &save_data.player, sizeof(Player));
    memcpy(map, &save_data.map, sizeof(Map));
    *game_time = save_data.game_time;
    map->current_zone_index = player->current_zone;

    printf("\n=== Partie chargée avec succès ===\n");
    printf("Joueur: %s - Niveau %d\n", player->name, player->level);
    printf("Zone actuelle: %s\n", map->zones[player->current_zone].name);
    return 1;
}

int save_exists(void)
{
    FILE *file = NULL;
    int close_status = 0;
    assert(SAVE_FILE_NAME != NULL);

    file = fopen(SAVE_FILE_NAME, "rb");
    if (file != NULL)
    {
        close_status = fclose(file);
        if (close_status != 0)
        {
            fprintf(stderr, "Avertissement: Erreur fermeture dans save_exists\n");
        }
        return 1;
    }
    return 0;
}

int delete_save(void)
{
    int remove_status = 0;
    assert(SAVE_FILE_NAME != NULL);

    if (!save_exists())
    {
        fprintf(stderr, "Aucune sauvegarde à supprimer\n");
        return 0;
    }
    remove_status = remove(SAVE_FILE_NAME);
    if (remove_status == 0)
    {
        printf("Sauvegarde supprimée avec succès\n");
        return 1;
    }
    else
    {
        fprintf(stderr, "Erreur: Impossible de supprimer la sauvegarde\n");
        return 0;
    }
}