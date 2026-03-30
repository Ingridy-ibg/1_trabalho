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

src/fila.o: src/fila.c src/fila.h

src/poligono.o: src/poligono.c src/poligono.h src/fila.h

test/t_lista.o: test/t_lista.c src/lista.h Unity/src/unity.h

test/t_fila.o: test/t_fila.c src/fila.h Unity/src/unity.h

test/t_poligono.o: test/t_poligono.c src/poligono.h Unity/src/unity.h

Unity/src/unity.o: Unity/src/unity.c Unity/src/unity.h

# ─── Testes unitários ────────────────────────────────────────────
t_lista: test/t_lista.o src/lista.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_lista.o src/lista.o Unity/src/unity.o \
	    -o test/t_lista
	./test/t_lista

t_fila: test/t_fila.o src/fila.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_fila.o src/fila.o Unity/src/unity.o \
	    -o test/t_fila
	./test/t_fila

t_poligono: test/t_poligono.o src/poligono.o src/fila.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_poligono.o src/poligono.o src/fila.o \
	    Unity/src/unity.o -o test/t_poligono
	./test/t_poligono

tstall: t_lista t_fila t_poligono

# ─── Utilitários ─────────────────────────────────────────────────
clean:
	rm -f src/lista.o src/fila.o src/poligono.o \
	      Unity/src/unity.o \
	      test/t_lista.o test/t_fila.o test/t_poligono.o \
	      test/t_lista test/t_fila test/t_poligono \
	      src/$(PROJ_NAME)

run: $(PROJ_NAME)
	./src/$(PROJ_NAME)