# Nom de l'exécutable
EXEC = metro

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g

# Fichiers sources
SRCS = main.c graphe.c

# Fichiers objets (remplacent l'extension .c par .o)
OBJS = $(SRCS:.c=.o)

# Règle principale : compiler l'exécutable
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Règle pour compiler les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(EXEC)
