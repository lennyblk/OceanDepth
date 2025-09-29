#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/ascii_art.h"
// pour forcer la console windows en utf-8
#ifdef _WIN32
#include <windows.h>
#endif

int main() {

  #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
  #endif

  display_player_ascii();
  printf("\n");
  display_giant_crab_ascii();
  printf("\n");
  display_shark_ascii();
  printf("\n");
  display_swordfish_ascii();
  printf("\n");
  display_jellyfish_ascii();

    return 0;
}