CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I include
SRC = $(wildcard src/*.c)
OBJDIR = obj
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))
EXEC = oceandepths.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: src/%.c
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)
	@if exist $(EXEC) del $(EXEC)

valgrind: $(EXEC)
	@echo "Valgrind is not natively available on Windows."
	@echo "Run under WSL or Linux for memory checking."

.PHONY: all clean valgrind