PROJ_NAME = ted
ALUNO     = IngridyGimenes
LIBS      =

OBJETOS = src/lista.o

CC      = gcc
CFLAGS  = -ggdb -O0 -std=c99 -fstack-protector-all \
          -Werror=implicit-function-declaration \
          -I./src -I./Unity/src \
          -DUNITY_INCLUDE_DOUBLE
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

src/retangulo.o: src/retangulo.c src/retangulo.h

src/circulo.o: src/circulo.c src/circulo.h

src/linha.o: src/linha.c src/linha.h

test/t_lista.o: test/t_lista.c src/lista.h Unity/src/unity.h

test/t_fila.o: test/t_fila.c src/fila.h Unity/src/unity.h

test/t_poligono.o: test/t_poligono.c src/poligono.h Unity/src/unity.h

test/t_retangulo.o: test/t_retangulo.c src/retangulo.h Unity/src/unity.h

test/t_circulo.o: test/t_circulo.c src/circulo.h Unity/src/unity.h

test/t_linha.o: test/t_linha.c src/linha.h Unity/src/unity.h

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

t_retangulo: test/t_retangulo.o src/retangulo.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_retangulo.o src/retangulo.o Unity/src/unity.o \
	    -o test/t_retangulo
	./test/t_retangulo

t_circulo: test/t_circulo.o src/circulo.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_circulo.o src/circulo.o Unity/src/unity.o \
	    -o test/t_circulo
	./test/t_circulo

t_linha: test/t_linha.o src/linha.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_linha.o src/linha.o Unity/src/unity.o -lm \
	    -o test/t_linha
	./test/t_linha

tstall: t_lista t_fila t_poligono t_retangulo t_circulo t_linha

# ─── Utilitários ─────────────────────────────────────────────────
clean:
	rm -f src/lista.o src/fila.o src/poligono.o \
	      Unity/src/unity.o \
	      test/t_lista.o test/t_fila.o test/t_poligono.o \
		test/t_retangulo.o test/t_circulo.o test/t_linha.o \
	      test/t_lista test/t_fila test/t_poligono \
		  test/t_retangulo test/t_retangulo\
		  test/t_circulo test/t_circulo\
		  test/t_linha test/t_linha\

		  
	      src/$(PROJ_NAME)

run: $(PROJ_NAME)
	./src/$(PROJ_NAME)