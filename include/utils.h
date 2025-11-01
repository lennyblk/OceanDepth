#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

void init_random();

int random_range(int min, int max);

void clear_screen();
void print_separator(char character, int length);
void print_title(const char *title);
void print_error(const char *message);
void print_success(const char *message);
void print_info(const char *message);
void pause_screen();
void print_progress_bar(int current, int max, int width);

void get_string_input(char *buffer, int max_length);
int get_int_input(int min, int max);
char get_char_input();
int confirm_action(const char *message);

void to_lowercase(char *str);
void trim_string(char *str);

void display_health_bar(int current, int max, int bar_length);
void display_oxygen_bar(int current, int max, int bar_length);

#endif