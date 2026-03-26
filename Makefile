PROJ_NAME = ted

ALUNO = IngridyGimenes
LIBS = 

# Objetos do projeto
OBJETOS = src/lista.o test/t_lista.o Unity/src/unity.o

# compilador
CC = gcc 

# Flags
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -I./src -I./Unity/src
LDFLAGS = -O0 

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

#
# Dependências
#

src/lista.o: src/lista.c src/lista.h

test/t_lista.o: test/t_lista.c src/lista.h Unity/src/unity.h

Unity/src/unity.o: Unity/src/unity.c Unity/src/unity.h

clean:
	rm -f $(OBJETOS) $(PROJ_NAME)

run: $(PROJ_NAME)
	./$(PROJ_NAME)