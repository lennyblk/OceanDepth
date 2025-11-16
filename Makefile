CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I include
SRC = $(wildcard src/*.c)
OBJDIR = obj
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))
EXEC = oceandepth
LOGFILE = valgrind.log

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(EXEC) $(LOGFILE)

valgrind: $(EXEC)
	valgrind --leak-check=full --log-file=$(LOGFILE) ./$(EXEC)

.PHONY: all clean valgrind