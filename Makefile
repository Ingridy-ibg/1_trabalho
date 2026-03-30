PROJ_NAME = ted
ALUNO     = IngridyGimenes
LIBS      =

OBJETOS = src/lista.o

CC      = gcc
CFLAGS  = -ggdb -O0 -std=c99 -fstack-protector-all \
		  -Werror=implicit-function-declaration \
		  -I./src -I./Unity/src
LDFLAGS = -O0

# ─── Executável principal ───────────────────────────────────────
$(PROJ_NAME): $(OBJETOS)
	$(CC) -o src/$(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS)

# ─── Regra genérica de compilação (.o) ──────────────────────────
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# ─── Dependências dos módulos ────────────────────────────────────
src/lista.o: src/lista.c src/lista.h

test/t_lista.o: test/t_lista.c src/lista.h Unity/src/unity.h

Unity/src/unity.o: Unity/src/unity.c Unity/src/unity.h

# ─── Testes unitários ────────────────────────────────────────────
t_lista: test/t_lista.o src/lista.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_lista.o src/lista.o Unity/src/unity.o \
		-o test/t_lista
	./test/t_lista

tstall: t_lista

# ─── Utilitários ─────────────────────────────────────────────────
clean:
	rm -f $(OBJETOS) Unity/src/unity.o test/t_lista.o \
		  test/t_lista src/$(PROJ_NAME)

run: $(PROJ_NAME)
	./src/$(PROJ_NAME)
