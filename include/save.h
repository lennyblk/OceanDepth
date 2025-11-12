#ifndef SAVE_H
#define SAVE_H

#include "types.h"

#define SAVE_FILE_NAME "oceandepth_save.dat"

int save_game(const Player *player, const Map *map, int game_time);

int load_game(Player *player, Map *map, int *game_time);

int save_exists(void);

int delete_save(void);

#endif