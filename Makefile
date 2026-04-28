PROJ_NAME = ted
ALUNO     = IngridyGimenes
LIBS      =

OBJETOS = src/lista.o src/fila.o src/poligono.o \
          src/circulo.o src/retangulo.o src/linha.o src/texto.o \
          src/formas.o src/svg.o \
          src/processadorGEO.o src/processadorQRY.o \
          src/args.o src/main.o

CC      = gcc
CFLAGS  = -ggdb -O0 -std=c99 -fstack-protector-all \
          -Werror=implicit-function-declaration \
          -I./src -I./Unity/src \
          -DUNITY_INCLUDE_DOUBLE
LDFLAGS = -O0

# ─── Executável principal ───────────────────────────────────────
$(PROJ_NAME): $(OBJETOS)
	$(CC) -o src/$(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS) -lm

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

src/texto.o: src/texto.c src/texto.h

test/t_texto.o: test/t_texto.c src/texto.h Unity/src/unity.h

src/formas.o: src/formas.c src/formas.h

src/args.o: src/args.c src/main.h

src/main.o: src/main.c src/main.h src/formas.h

src/processadorGEO.o: src/processadorGEO.c src/processadorGEO.h src/formas.h src/texto.h

src/processadorQRY.o: src/processadorQRY.c src/processadorQRY.h src/formas.h src/poligono.h

test/t_formas.o: test/t_formas.c src/formas.h src/circulo.h src/retangulo.h src/linha.h src/texto.h Unity/src/unity.h

test/t_main.o: test/t_main.c src/main.h Unity/src/unity.h

test/t_processadorGEO.o: test/t_processadorGEO.c src/processadorGEO.h src/formas.h src/texto.h Unity/src/unity.h

test/t_processadorQRY.o: test/t_processadorQRY.c src/processadorQRY.h src/formas.h Unity/src/unity.h

src/svg.o: src/svg.c src/svg.h

test/t_svg.o: test/t_svg.c src/svg.h Unity/src/unity.h



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
	$(CC) $(LDFLAGS) test/t_circulo.o src/circulo.o Unity/src/unity.o -lm \
		-o test/t_circulo
	./test/t_circulo

t_linha: test/t_linha.o src/linha.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_linha.o src/linha.o Unity/src/unity.o -lm \
		-o test/t_linha
	./test/t_linha

t_texto: test/t_texto.o src/texto.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_texto.o src/texto.o Unity/src/unity.o -lm \
		-o test/t_texto
	./test/t_texto

t_formas: test/t_formas.o src/formas.o src/circulo.o src/retangulo.o src/linha.o src/texto.o src/lista.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_formas.o src/formas.o src/circulo.o \
		src/retangulo.o src/linha.o src/texto.o src/lista.o Unity/src/unity.o -lm \
		-o test/t_formas
	./test/t_formas

t_main: test/t_main.o src/args.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_main.o src/args.o Unity/src/unity.o \
		-o test/t_main
	./test/t_main

t_processadorGEO: test/t_processadorGEO.o src/processadorGEO.o src/formas.o src/circulo.o src/retangulo.o src/linha.o src/texto.o src/lista.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_processadorGEO.o src/processadorGEO.o src/formas.o \
		src/circulo.o src/retangulo.o src/linha.o src/texto.o src/lista.o Unity/src/unity.o -lm \
		-o test/t_processadorGEO
	./test/t_processadorGEO


t_processadorQRY: test/t_processadorQRY.o src/processadorQRY.o src/svg.o src/formas.o src/poligono.o src/circulo.o src/retangulo.o src/linha.o src/texto.o src/fila.o src/lista.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_processadorQRY.o src/processadorQRY.o src/svg.o src/formas.o src/poligono.o \
		src/circulo.o src/retangulo.o src/linha.o src/texto.o src/fila.o src/lista.o \
		Unity/src/unity.o -lm -o test/t_processadorQRY
	./test/t_processadorQRY

t_svg: test/t_svg.o src/svg.o src/formas.o src/circulo.o src/retangulo.o \
       src/linha.o src/texto.o src/lista.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_svg.o src/svg.o src/formas.o src/circulo.o \
		src/retangulo.o src/linha.o src/texto.o src/lista.o \
		Unity/src/unity.o -lm -o test/t_svg
	./test/t_svg

tstall: t_lista t_fila t_poligono t_retangulo t_circulo t_linha t_texto \
        t_formas t_processadorGEO t_processadorQRY t_svg t_main

# ─── Utilitários ─────────────────────────────────────────────────
clean:
	rm -f src/lista.o src/fila.o src/poligono.o src/retangulo.o \
		  src/circulo.o src/linha.o src/texto.o src/formas.o src/args.o src/main.o \
		  src/processadorGEO.o src/processadorQRY.o src/svg.o Unity/src/unity.o \
		  test/t_lista.o test/t_fila.o test/t_poligono.o \
		  test/t_retangulo.o test/t_circulo.o test/t_linha.o test/t_texto.o test/t_formas.o \
		  test/t_processadorGEO.o test/t_processadorQRY.o test/t_svg.o \
		  test/t_lista test/t_fila test/t_poligono \
		  test/t_retangulo test/t_circulo test/t_linha test/t_texto test/t_formas \
		  test/t_processadorGEO test/t_processadorQRY test/t_svg test/t_main.o test/t_main  \
		  src/$(PROJ_NAME)

rodatestes: ted
	@mkdir -p saida
	@count=0; \
	for geo in $$(find testes/ -name "*.geo"); do \
	    dir=$$(dirname $$geo); \
	    stem=$$(basename $$geo .geo); \
	    qrydir=$$dir/$$stem; \
	    if [ -d "$$qrydir" ]; then \
	        for qry in $$qrydir/*.qry; do \
	            qrystem=$$(basename $$qry .qry); \
	            echo "[GEO+QRY] $$stem + $$qrystem"; \
	            ./src/ted -e $$dir -f $$stem.geo -q $$stem/$$qrystem.qry -o ./saida; \
	            count=$$((count + 1)); \
	        done; \
	    else \
	        echo "[GEO]     $$stem"; \
	        ./src/ted -e $$dir -f $$stem.geo -o ./saida; \
	        count=$$((count + 1)); \
	    fi; \
	done; \
	echo ""; \
	echo "$$count arquivo(s) processado(s). Gerados em saida/:"; \
	ls saida/
	
run: $(PROJ_NAME)
	./src/$(PROJ_NAME)