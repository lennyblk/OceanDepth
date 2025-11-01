CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Iinclude
DEBUG_CFLAGS = -std=c11 -g -Wall -Wextra -Iinclude
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.c)
TARGET = OceanDepth.exe
DEBUG_TARGET = OceanDepth_debug.exe

.PHONY: all clean run debug valgrind

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

$(DEBUG_TARGET):
	$(CC) $(DEBUG_CFLAGS) $(SRCS) -o $(DEBUG_TARGET)

valgrind: $(DEBUG_TARGET)
	@echo "Exécution de Valgrind sur $(DEBUG_TARGET) pour vérifier les erreurs de mémoire. Le rapport est sauvegardé dans valgrind.log."
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./$(DEBUG_TARGET)

run: all
	@./$(TARGET)

clean:
	@rm -f $(TARGET) $(DEBUG_TARGET) $(SRCDIR)/*.o
