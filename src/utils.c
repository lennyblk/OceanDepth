#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include "../include/utils.h"
#include "../include/constants.h"

void init_random(void)
{
    srand((unsigned int)time(NULL));
}

int random_range(int min, int max)
{
    if (min > max)
    {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min + 1);
}

void clear_screen(void)
{
    int system_result = 0;

#ifdef _WIN32
    system_result = system("cls");
#else
    system_result = system("clear");
#endif

    if (system_result != 0)
    {
    }
}

void get_string_input(char *buffer, int max_length)
{
    assert(buffer != NULL);
    assert(max_length > 0);

    if (fgets(buffer, max_length, stdin) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }
    }
    else
    {
        buffer[0] = '\0';
    }
}

int get_int_input(int min, int max)
{
    char buffer[MAX_INPUT_LENGTH];
    int value = min - 1;
    char *endptr = NULL;

    while (1)
    {
        printf("Entrez un nombre entre %d et %d: ", min, max);
        get_string_input(buffer, MAX_INPUT_LENGTH);

        value = (int)strtol(buffer, &endptr, 10);

        if (*endptr == '\0' && value >= min && value <= max)
        {
            return value;
        }

        printf(COLOR_RED "%s" COLOR_RESET "\n", ERROR_INVALID_INPUT);
    }
}

char get_char_input(void)
{
    char buffer[MAX_INPUT_LENGTH];
    get_string_input(buffer, MAX_INPUT_LENGTH);

    if (strlen(buffer) == 1)
    {
        return (char)tolower((unsigned char)buffer[0]);
    }

    return '\0';
}

void print_separator(char character, int length)
{
    assert(length > 0);
    for (int i = 0; i < length; i++)
    {
        (void)putchar(character);
    }
    (void)putchar('\n');
}

void print_error(const char *message)
{
    assert(message != NULL);
    (void)printf(COLOR_RED "ERREUR: %s" COLOR_RESET "\n", message);
}

void print_info(const char *message)
{
    assert(message != NULL);
    (void)printf(COLOR_BLUE "INFO: %s" COLOR_RESET "\n", message);
}

void pause_screen(void)
{
    (void)printf(COLOR_YELLOW "\nAppuyez sur Entrée pour continuer..." COLOR_RESET);
    int c = getchar();

    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    if (c == EOF && feof(stdin))
    {
        clearerr(stdin);
    }
}

int confirm_action(const char *message)
{
    assert(message != NULL);
    char response = '\0';

    while (1)
    {
        (void)printf("%s (o/n): ", message);
        response = get_char_input();

        if (response == 'o' || response == 'y')
        {
            return 1;
        }
        else if (response == 'n')
        {
            return 0;
        }

        (void)printf(COLOR_RED "Répondez par 'o' (oui) ou 'n' (non)." COLOR_RESET "\n");
    }
}

void print_progress_bar(int current, int max, int width)
{
    assert(width > 0);
    assert(max >= 0);
    assert(current >= 0);

    float percentage = 0.0f;
    if (max > 0)
    {
        if (current > max)
        {
            current = max;
        }
        percentage = (float)current / (float)max;
    }

    int filled = (int)(percentage * (float)width);

    (void)printf("[");
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
        {
            (void)printf(COLOR_GREEN "█" COLOR_RESET);
        }
        else
        {
            (void)printf("░");
        }
    }
    (void)printf("] %d/%d (%.1f%%)", current, max, percentage * 100.0f);
}

void print_success(const char *message)
{
    assert(message != NULL);
    (void)printf(COLOR_GREEN "✓ %s" COLOR_RESET "\n", message);
}

void print_title(const char *title)
{
    assert(title != NULL);
    print_separator('=', 60);
    (void)printf(COLOR_CYAN COLOR_BOLD "  %s  " COLOR_RESET "\n", title);
    print_separator('=', 60);
}