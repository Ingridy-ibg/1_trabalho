# TED — Trabalho I de Estrutura de Dados I (2026)

Programa em C que lê descrições de formas geométricas em um plano cartesiano,
processa consultas (seleções, polígonos, modificações) e produz visualizações
em SVG e relatórios em texto.

**Aluna:** Ingridy Gimenes
**Disciplina:** Estrutura de Dados I — 2026

---

## Como compilar

```bash
make
```

O executável é gerado em `src/ted`. A compilação usa `gcc` com:
- `-std=c99` (padrão C99)
- `-fstack-protector-all` (proteção de pilha)
- `-Werror=implicit-function-declaration` (declarações implícitas viram erro)

Para limpar:
```bash
make clean
```

---

## Como rodar

```bash
./src/ted -f arq.geo -o dir_saida [-e dir_entrada] [-q arq.qry]
```

| Flag | Obrigatório | Descrição |
|------|-------------|-----------|
| `-e path` | Não | Diretório-base de entrada (BED). Default: `.` |
| `-f arq.geo` | **Sim** | Arquivo com a descrição das formas |
| `-o path` | **Sim** | Diretório-base de saída (BSD) |
| `-q arq.qry` | Não | Arquivo de consultas |

### Arquivos produzidos

| Entrada | Saída |
|---------|-------|
| Só `-f arq.geo` | `arq.svg` |
| `-f arq.geo -q arqcons.qry` | `arq.svg`, `arq-arqcons.svg`, `arq-arqcons.txt` |

---

## Comandos suportados

### Arquivo `.geo` (descrição das formas)

| Comando | Parâmetros | Descrição |
|---------|------------|-----------|
| `c` | `i x y r corb corp` | Círculo de centro `(x,y)` e raio `r` |
| `r` | `i x y w h corb corp` | Retângulo (âncora no canto inf. esquerdo) |
| `l` | `i x1 y1 x2 y2 cor` | Linha entre dois pontos |
| `t` | `i x y corb corp a txto` | Texto (`a` = `i`/`m`/`f` para âncora) |
| `ts` | `fFamily fWeight fSize` | Estilo dos textos seguintes |

### Arquivo `.qry` (consultas)

| Comando | Parâmetros | Descrição |
|---------|------------|-----------|
| `inp` | `p i` | Insere âncora da figura `i` no polígono `p` |
| `rmp` | `p` | Remove a coordenada mais antiga do polígono `p` |
| `pol` | `p i d corb corp` | Gera bordas + preenchimento por scanline |
| `clp` | `p` | Limpa o polígono `p` |
| `sel` | `x y w h` | Seleciona figuras inteiramente dentro da região |
| `dels` | — | Remove do banco as figuras selecionadas |
| `mcs` | `dx dy corb corp` | Translada e recolore as selecionadas |

---

## Estrutura do projeto
1_trabalho/
├── Makefile
├── README.md
├── Unity/                # Framework de teste (3rd party)
├── src/                  # Código-fonte
│   ├── lista.{h,c}       # TAD Lista (dinâmica, duplo encadeamento)
│   ├── fila.{h,c}        # TAD Fila (estática, circular)
│   ├── circulo.{h,c}     # Forma: círculo
│   ├── retangulo.{h,c}   # Forma: retângulo
│   ├── linha.{h,c}       # Forma: linha (com stroke-width adaptativo)
│   ├── texto.{h,c}       # Forma: texto
│   ├── formas.{h,c}      # Banco de formas + lista de selecionadas
│   ├── poligono.{h,c}    # Polígonos (filas de coordenadas)
│   ├── svg.{h,c}         # Geração de SVG
│   ├── processadorGEO.{h,c}  # Parser do .geo
│   ├── processadorQRY.{h,c}  # Parser do .qry
│   ├── args.{h,c}        # Parsing dos argumentos de CLI
│   ├── main.{h,c}        # Entry point (int main)
│   └── ted               # Executável (gerado pelo make)
├── test/                 # Testes unitários (Unity)
│   ├── t_lista.c
│   ├── t_fila.c
│   ├── ... (12 arquivos no total)
│   └── t_main.c
├── testes/t1/            # Casos de teste fornecidos pelo professor
└── saida/                # Saídas geradas

### Decisões de implementação

- **Lista**: implementação dinâmica com encadeamento duplo, conforme TAD em
  aula. Sentinelas (cabeça/cauda) facilitam inserção/remoção.
- **Fila**: array estático circular (tamanho fixo definido em compilação).
- **Polígonos**: armazenados em filas estáticas (uma por polígono, até 10).
- **Formas**: armazenadas em uma `Lista` (banco). Uma segunda `Lista` mantém
  as figuras selecionadas pelo `sel` (conforme a spec exige).
- **Encapsulamento**: todos os módulos usam ponteiros opacos (`typedef void *`).
  Nenhum `struct` é definido em arquivo `.h`.
- **Hachuras adaptativas**: o `pol` aplica `stroke-width = d/4` às linhas de
  preenchimento e `d/2` às bordas, garantindo que as hachuras fiquem
  visualmente separadas independente do valor de `d`.

---

## Testes unitários

O projeto usa o framework [Unity](http://www.throwtheswitch.org/unity).
Cada módulo tem seu arquivo de teste em `test/t_<modulo>.c`.

Para compilar e executar todos os testes:
```bash
make tstall
```

Para rodar um teste específico:
```bash
make t_lista
make t_fila
make t_poligono
# ... etc
```

Os testes cobrem casos normais, condições de borda e tratamento de NULL.

---

## Algoritmo de preenchimento do polígono

O comando `pol` usa o algoritmo **scanline horizontal**:

1. Insere `n` linhas de borda (uma para cada lado, fechando o polígono).
2. Calcula `ymin` e `ymax` dos vértices.
3. Para cada `yscan = ymin + d, ymin + 2d, ..., < ymax`:
   - Acha as interseções de `y = yscan` com as bordas.
   - Ordena por `x`.
   - Insere uma linha de preenchimento entre cada par consecutivo de
     interseções (algoritmo padrão de paridade — funciona com polígonos
     côncavos, não-convexos e auto-intersectantes).
4. As bordas recebem `stroke-width = d/2` e as hachuras `d/4`.

O número de linhas inseridas é reportado no TXT.

---

## Notas sobre a especificação

- A funcionalidade `arq-arqcons-sufx.[svg|txt]` mencionada na tabela
  "Resumo dos arquivos produzidos" não foi implementada porque a spec não
  define qual comando do `.qry` recebe o sufixo, e nenhum dos casos de teste
  fornecidos exercita esse cenário. Aparenta ser um item para o Trabalho 2.
- O comando `pol` produz, além das linhas inseridas no banco, uma linha de
  resumo no TXT no formato:
pol p: N lados + M linhas preenchimento = (N+M) linhas inseridas (ids i..j)
  Isso não é exigido pela spec; é informação extra para depuração e
  validação.

---

## Compilação resumida

```bash
make            # gera src/ted
make tstall     # roda todos os testes unitários
make rodatestes # processa todos os arquivos em testes/t1 e gera saídas em saida/
make clean      # remove .o e binários
```