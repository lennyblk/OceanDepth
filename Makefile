CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I include
SRC = $(wildcard src/*.c)
OBJDIR = obj
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))
EXEC = oceandepth.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(EXEC)

valgrind: $(EXEC)
	@echo "Valgrind is not natively available on Windows."
	@echo "Run under WSL or Linux for memory checking."

.PHONY: all clean valgrind