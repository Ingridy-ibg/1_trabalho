/**
 * @file t_processadorQRY.c
 * @brief Testes unitários para processadorQRY.c
 *
 * Estratégia:
 *   - Escreve arquivos .qry temporários em /tmp.
 *   - Popula o banco de formas diretamente (sem passar pelo processadorGEO).
 *   - Chama processaQry() com SVG e TXT também em /tmp.
 *   - Verifica o estado do banco e o conteúdo dos arquivos de saída.
 *
 * Cada teste é independente: setUp() recria o banco e reinicia os polígonos.
 */

#include "unity.h"
#include "../src/processadorQRY.h"
#include "../src/formas.h"
#include "../src/poligono.h"
#include "../src/circulo.h"
#include "../src/retangulo.h"
#include "../src/linha.h"
#include "../src/texto.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ─────────────────────────────────────────────
   Helpers
   ───────────────────────────────────────────── */

static Formas F;

/** Escreve conteúdo em /tmp/<nome> e retorna o caminho. */
static const char *qry(const char *nome, const char *conteudo) {
    static char cam[256];
    snprintf(cam, sizeof(cam), "/tmp/qry_%s", nome);
    FILE *f = fopen(cam, "w");
    if (f) { fputs(conteudo, f); fclose(f); }
    return cam;
}

/** Lê /tmp/<nome> inteiro para buffer estático. */
static const char *le(const char *nome) {
    static char buf[16384];
    char cam[256];
    snprintf(cam, sizeof(cam), "/tmp/qry_%s", nome);
    buf[0] = '\0';
    FILE *f = fopen(cam, "r");
    if (!f) return buf;
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = '\0';
    fclose(f);
    return buf;
}

/** Abre /tmp/<nome> para escrita e retorna o FILE*. */
static FILE *abre(const char *nome) {
    char cam[256];
    snprintf(cam, sizeof(cam), "/tmp/qry_%s", nome);
    return fopen(cam, "w");
}

/**
 * Roda processaQry com um .qry temporário e SVG/TXT em /tmp.
 * Retorna true se processaQry retornou true.
 */
static bool roda(const char *qry_path,
                 const char *svg_nome,
                 const char *txt_nome) {
    FILE *svg = abre(svg_nome);
    FILE *txt = abre(txt_nome);
    bool ok = processaQry(qry_path, F, svg, txt);
    fclose(svg);
    fclose(txt);
    return ok;
}

void setUp(void) {
    inicializaPoligonos();
    F = criaFormas();
}

void tearDown(void) {
    destroiFormas(F);
    destroiPoligonos();
}

/* ═══════════════════════════════════════════════
   GRUPO 1: Defensivos / arquivo
   ═══════════════════════════════════════════════ */

void test_qry_ArquivoInexistente_RetornaFalse(void) {
    FILE *svg = abre("def_svg.svg");
    FILE *txt = abre("def_txt.txt");
    bool ok = processaQry("/tmp/__nao_existe__.qry", F, svg, txt);
    fclose(svg); fclose(txt);
    TEST_ASSERT_FALSE_MESSAGE(ok, "Arquivo inexistente deve retornar false.");
}

void test_qry_CaminhoNulo_RetornaFalse(void) {
    FILE *svg = abre("nul_svg.svg");
    FILE *txt = abre("nul_txt.txt");
    bool ok = processaQry(NULL, F, svg, txt);
    fclose(svg); fclose(txt);
    TEST_ASSERT_FALSE(ok);
}

void test_qry_BancoNulo_RetornaFalse(void) {
    const char *c = qry("banconulo.qry", "sel 0 0 100 100\n");
    FILE *svg = abre("bn_svg.svg");
    FILE *txt = abre("bn_txt.txt");
    bool ok = processaQry(c, NULL, svg, txt);
    fclose(svg); fclose(txt);
    TEST_ASSERT_FALSE(ok);
}

void test_qry_SvgNulo_RetornaFalse(void) {
    const char *c = qry("svgnulo.qry", "sel 0 0 100 100\n");
    FILE *txt = abre("sn_txt.txt");
    bool ok = processaQry(c, F, NULL, txt);
    fclose(txt);
    TEST_ASSERT_FALSE(ok);
}

void test_qry_TxtNulo_RetornaFalse(void) {
    const char *c = qry("txtnulo.qry", "sel 0 0 100 100\n");
    FILE *svg = abre("tn_svg.svg");
    bool ok = processaQry(c, F, svg, NULL);
    fclose(svg);
    TEST_ASSERT_FALSE(ok);
}

void test_qry_ArquivoVazio_RetornaTrue(void) {
    const char *c = qry("vazio.qry", "");
    bool ok = roda(c, "vazio.svg", "vazio.txt");
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(0, totalFormas(F));
}

void test_qry_SoComentarios_NaoAlteraBanco(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("coments.qry",
        "# linha 1\n"
        "# linha 2\n"
        "\n");
    roda(c, "coments.svg", "coments.txt");
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
}

void test_qry_ComandoDesconhecido_Ignorado(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("unk.qry", "xyz 99 ignorado\n");
    roda(c, "unk.svg", "unk.txt");
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
}

/* ═══════════════════════════════════════════════
   GRUPO 2: inp
   ═══════════════════════════════════════════════ */

void test_qry_Inp_InsereNaFila(void) {
    insereCirculo(F, 1, 30.0, 40.0, 5.0, "black", "red");
    const char *c = qry("inp1.qry", "inp 1 1\n");
    roda(c, "inp1.svg", "inp1.txt");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(1),
        "Poligono 1 deve ter 1 ponto.");
}

void test_qry_Inp_CoordenadasCorretas(void) {
    insereCirculo(F, 1, 30.0, 40.0, 5.0, "black", "red");
    const char *c = qry("inp2.qry", "inp 1 1\n");
    roda(c, "inp2.svg", "inp2.txt");

    double px, py;
    TEST_ASSERT_TRUE(peekPoligono(1, &px, &py));
    TEST_ASSERT_EQUAL_DOUBLE(30.0, px);
    TEST_ASSERT_EQUAL_DOUBLE(40.0, py);
}

void test_qry_Inp_TxtContemPiIeTipo(void) {
    insereRetangulo(F, 2, 5.0, 5.0, 10.0, 10.0, "black", "blue");
    const char *c = qry("inp3.qry", "inp 1 2\n");
    roda(c, "inp3.svg", "inp3.txt");

    const char *s = le("inp3.txt");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "inp 1 2"), "TXT deve conter 'inp 1 2'.");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "retangulo"), "TXT deve conter tipo.");
}

void test_qry_Inp_MultiplosPoligonos(void) {
    insereCirculo(F, 1, 10.0, 10.0, 5.0, "black", "red");
    insereCirculo(F, 2, 20.0, 20.0, 5.0, "black", "red");
    const char *c = qry("inp4.qry",
        "inp 1 1\n"
        "inp 2 2\n"
        "inp 1 2\n");
    roda(c, "inp4.svg", "inp4.txt");
    TEST_ASSERT_EQUAL_INT(2, lengthPoligono(1));
    TEST_ASSERT_EQUAL_INT(1, lengthPoligono(2));
}

void test_qry_Inp_IdInexistente_NaoInsere(void) {
    /* id 99 não existe no banco */
    const char *c = qry("inp5.qry", "inp 1 99\n");
    roda(c, "inp5.svg", "inp5.txt");
    TEST_ASSERT_EQUAL_INT(0, lengthPoligono(1));
}

void test_qry_Inp_AncoraDeLinha_MenorX(void) {
    /* Linha: âncora é a extremidade de menor x */
    insereLinha(F, 3, 80.0, 50.0, 20.0, 30.0, "black");
    const char *c = qry("inp6.qry", "inp 1 3\n");
    roda(c, "inp6.svg", "inp6.txt");

    double px, py;
    peekPoligono(1, &px, &py);
    TEST_ASSERT_EQUAL_DOUBLE(20.0, px);
    TEST_ASSERT_EQUAL_DOUBLE(30.0, py);
}

/* ═══════════════════════════════════════════════
   GRUPO 3: rmp
   ═══════════════════════════════════════════════ */

void test_qry_Rmp_RemovePonto(void) {
    insereCirculo(F, 1, 10.0, 20.0, 5.0, "black", "red");
    const char *c = qry("rmp1.qry", "inp 2 1\nrmp 2\n");
    roda(c, "rmp1.svg", "rmp1.txt");
    TEST_ASSERT_EQUAL_INT(0, lengthPoligono(2));
}

void test_qry_Rmp_TxtContemCoordenada(void) {
    insereCirculo(F, 1, 10.0, 20.0, 5.0, "black", "red");
    const char *c = qry("rmp2.qry", "inp 2 1\nrmp 2\n");
    roda(c, "rmp2.svg", "rmp2.txt");

    const char *s = le("rmp2.txt");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "rmp 2"), "TXT deve conter 'rmp 2'.");
}

void test_qry_Rmp_FilaVazia_NaoEscreve(void) {
    /* rmp em fila vazia não deve escrever nada no TXT */
    const char *c = qry("rmp3.qry", "rmp 1\n");
    roda(c, "rmp3.svg", "rmp3.txt");

    const char *s = le("rmp3.txt");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, (int)strlen(s),
        "TXT deve estar vazio para rmp em fila vazia.");
}

void test_qry_Rmp_OrdemFIFO(void) {
    insereCirculo(F, 1, 10.0, 10.0, 5.0, "black", "red");
    insereCirculo(F, 2, 20.0, 20.0, 5.0, "black", "red");
    /* inp 1, inp 2 → rmp deve remover 1 primeiro */
    const char *c = qry("rmp4.qry",
        "inp 3 1\n"
        "inp 3 2\n"
        "rmp 3\n");
    roda(c, "rmp4.svg", "rmp4.txt");
    TEST_ASSERT_EQUAL_INT(1, lengthPoligono(3));

    double px, py;
    peekPoligono(3, &px, &py);
    /* o ponto que sobrou é o da figura 2 */
    TEST_ASSERT_EQUAL_DOUBLE(20.0, px);
}

/* ═══════════════════════════════════════════════
   GRUPO 4: clp
   ═══════════════════════════════════════════════ */

void test_qry_Clp_LimpaPoligono(void) {
    insereCirculo(F, 1, 10.0, 10.0, 3.0, "black", "red");
    insereCirculo(F, 2, 20.0, 20.0, 3.0, "black", "red");
    const char *c = qry("clp1.qry",
        "inp 4 1\n"
        "inp 4 2\n"
        "clp 4\n");
    roda(c, "clp1.svg", "clp1.txt");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyPoligono(4), "Poligono 4 deve estar vazio.");
}

void test_qry_Clp_NaoAfetaOutroPoligono(void) {
    insereCirculo(F, 1, 10.0, 10.0, 3.0, "black", "red");
    const char *c = qry("clp2.qry",
        "inp 1 1\n"
        "inp 2 1\n"
        "clp 1\n");
    roda(c, "clp2.svg", "clp2.txt");
    TEST_ASSERT_TRUE(isEmptyPoligono(1));
    TEST_ASSERT_EQUAL_INT(1, lengthPoligono(2));
}

/* ═══════════════════════════════════════════════
   GRUPO 5: sel
   ═══════════════════════════════════════════════ */

void test_qry_Sel_SelecionaFormaDentro(void) {
    insereRetangulo(F, 1, 10.0, 10.0, 50.0, 50.0, "black", "blue");
    const char *c = qry("sel1.qry", "sel 0.0 0.0 200.0 200.0\n");
    roda(c, "sel1.svg", "sel1.txt");
    TEST_ASSERT_EQUAL_INT(1, totalFormasSelecionadas(F));
}

void test_qry_Sel_NaoSelecionaFormaFora(void) {
    insereCirculo(F, 1, 500.0, 500.0, 10.0, "black", "red");
    const char *c = qry("sel2.qry", "sel 0.0 0.0 100.0 100.0\n");
    roda(c, "sel2.svg", "sel2.txt");
    TEST_ASSERT_EQUAL_INT(0, totalFormasSelecionadas(F));
}

void test_qry_Sel_TxtContemIdETipo(void) {
    insereCirculo(F, 7, 50.0, 50.0, 10.0, "black", "red");
    const char *c = qry("sel3.qry", "sel 0.0 0.0 300.0 300.0\n");
    roda(c, "sel3.svg", "sel3.txt");

    const char *s = le("sel3.txt");
    TEST_ASSERT_NOT_NULL(strstr(s, "7"));
    TEST_ASSERT_NOT_NULL(strstr(s, "circulo"));
}

void test_qry_Sel_SvgContemDasharray(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("sel4.qry", "sel 0.0 0.0 200.0 200.0\n");
    roda(c, "sel4.svg", "sel4.txt");

    TEST_ASSERT_NOT_NULL(strstr(le("sel4.svg"), "dasharray"));
}

void test_qry_Sel_SvgContemAnelo(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("sel5.qry", "sel 0.0 0.0 200.0 200.0\n");
    roda(c, "sel5.svg", "sel5.txt");

    /* Anel = <circle ... fill="none" */
    TEST_ASSERT_NOT_NULL(strstr(le("sel5.svg"), "fill=\"none\""));
}

void test_qry_Sel_DescartaSelecaoAnterior(void) {
    insereCirculo(F,   1, 50.0,  50.0,  5.0, "black", "red");
    insereCirculo(F,   2, 500.0, 500.0, 5.0, "black", "red");
    /* Primeira sel pega ambos; segunda sel pega só o 1 */
    const char *c = qry("sel6.qry",
        "sel 0.0 0.0 9999.0 9999.0\n"
        "sel 0.0 0.0 100.0 100.0\n");
    roda(c, "sel6.svg", "sel6.txt");
    TEST_ASSERT_EQUAL_INT(1, totalFormasSelecionadas(F));
}

void test_qry_Sel_MultiplasFormas(void) {
    insereCirculo(F,   1, 10.0, 10.0, 5.0, "black", "red");
    insereRetangulo(F, 2, 20.0, 20.0, 10.0, 10.0, "black", "blue");
    insereLinha(F,     3, 30.0, 30.0, 40.0, 40.0, "green");
    const char *c = qry("sel7.qry", "sel 0.0 0.0 200.0 200.0\n");
    roda(c, "sel7.svg", "sel7.txt");
    TEST_ASSERT_EQUAL_INT(3, totalFormasSelecionadas(F));
}

/* ═══════════════════════════════════════════════
   GRUPO 6: dels
   ═══════════════════════════════════════════════ */

void test_qry_Dels_RemoveFormasSelecionadas(void) {
    insereRetangulo(F, 1, 10.0, 10.0, 50.0, 50.0, "black", "blue");
    const char *c = qry("dels1.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "dels\n");
    roda(c, "dels1.svg", "dels1.txt");
    TEST_ASSERT_EQUAL_INT(0, totalFormas(F));
}

void test_qry_Dels_NaoRemoveNaoSelecionadas(void) {
    insereCirculo(F, 1, 50.0,  50.0,  5.0, "black", "red");  /* dentro */
    insereCirculo(F, 2, 500.0, 500.0, 5.0, "black", "red");  /* fora   */
    const char *c = qry("dels2.qry",
        "sel 0.0 0.0 100.0 100.0\n"
        "dels\n");
    roda(c, "dels2.svg", "dels2.txt");
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
    TEST_ASSERT_NOT_NULL(buscaFormaPorId(F, 2));
}

void test_qry_Dels_TxtContemIdETipo(void) {
    insereCirculo(F, 5, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("dels3.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "dels\n");
    roda(c, "dels3.svg", "dels3.txt");

    const char *s = le("dels3.txt");
    TEST_ASSERT_NOT_NULL(strstr(s, "5"));
    TEST_ASSERT_NOT_NULL(strstr(s, "circulo"));
}

void test_qry_Dels_TxtContemDadosDaForma(void) {
    insereRetangulo(F, 3, 10.0, 20.0, 30.0, 40.0, "black", "blue");
    const char *c = qry("dels4.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "dels\n");
    roda(c, "dels4.svg", "dels4.txt");

    const char *s = le("dels4.txt");
    TEST_ASSERT_NOT_NULL(strstr(s, "retangulo"));
    TEST_ASSERT_NOT_NULL(strstr(s, "w="));
    TEST_ASSERT_NOT_NULL(strstr(s, "h="));
}

void test_qry_Dels_SvgContemXVermelho(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("dels5.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "dels\n");
    roda(c, "dels5.svg", "dels5.txt");

    /* O "x" é feito de dois <line> */
    const char *s = le("dels5.svg");
    const char *p1 = strstr(s, "<line");
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(strstr(p1 + 1, "<line"));
}

void test_qry_Dels_SemSelecao_NaoRemoveNada(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    /* dels sem sel prévia: nada deve ser removido */
    const char *c = qry("dels6.qry", "dels\n");
    roda(c, "dels6.svg", "dels6.txt");
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
}

/* ═══════════════════════════════════════════════
   GRUPO 7: mcs
   ═══════════════════════════════════════════════ */

void test_qry_Mcs_TransladaX(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("mcs1.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "mcs 10.0 0.0 black red\n");
    roda(c, "mcs1.svg", "mcs1.txt");

    Circulo ci = (Circulo)getDadosForma(F, buscaFormaPorId(F, 1));
    TEST_ASSERT_EQUAL_DOUBLE(60.0, getXCirculo(ci));
}

void test_qry_Mcs_TransladaY(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("mcs2.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "mcs 0.0 20.0 black red\n");
    roda(c, "mcs2.svg", "mcs2.txt");

    Circulo ci = (Circulo)getDadosForma(F, buscaFormaPorId(F, 1));
    TEST_ASSERT_EQUAL_DOUBLE(70.0, getYCirculo(ci));
}

void test_qry_Mcs_MudaCores(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("mcs3.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "mcs 0.0 0.0 blue green\n");
    roda(c, "mcs3.svg", "mcs3.txt");

    Circulo ci = (Circulo)getDadosForma(F, buscaFormaPorId(F, 1));
    TEST_ASSERT_EQUAL_STRING("blue",  getCorbCirculo(ci));
    TEST_ASSERT_EQUAL_STRING("green", getCorpCirculo(ci));
}

void test_qry_Mcs_NaoAfetaNaoSelecionadas(void) {
    insereCirculo(F, 1, 50.0,  50.0,  5.0, "black", "red");
    insereCirculo(F, 2, 500.0, 500.0, 5.0, "black", "red");
    const char *c = qry("mcs4.qry",
        "sel 0.0 0.0 100.0 100.0\n"
        "mcs 10.0 10.0 blue green\n");
    roda(c, "mcs4.svg", "mcs4.txt");

    Circulo c2 = (Circulo)getDadosForma(F, buscaFormaPorId(F, 2));
    TEST_ASSERT_EQUAL_DOUBLE(500.0, getXCirculo(c2));
    TEST_ASSERT_EQUAL_STRING("black", getCorbCirculo(c2));
}

void test_qry_Mcs_RetanguloTransladado(void) {
    insereRetangulo(F, 1, 0.0, 0.0, 10.0, 10.0, "black", "blue");
    const char *c = qry("mcs5.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "mcs 5.0 3.0 red white\n");
    roda(c, "mcs5.svg", "mcs5.txt");

    Retangulo r = (Retangulo)getDadosForma(F, buscaFormaPorId(F, 1));
    TEST_ASSERT_EQUAL_DOUBLE(5.0, getXRetangulo(r));
    TEST_ASSERT_EQUAL_DOUBLE(3.0, getYRetangulo(r));
}

/* ═══════════════════════════════════════════════
   GRUPO 8: pol (scanline)
   ═══════════════════════════════════════════════ */

void test_qry_Pol_QuadradoGeraBordas(void) {
    insereCirculo(F, 1,   0.0,   0.0, 1.0, "black", "red");
    insereCirculo(F, 2, 100.0,   0.0, 1.0, "black", "red");
    insereCirculo(F, 3, 100.0, 100.0, 1.0, "black", "red");
    insereCirculo(F, 4,   0.0, 100.0, 1.0, "black", "red");

    const char *c = qry("pol1.qry",
        "inp 1 1\n"
        "inp 1 2\n"
        "inp 1 3\n"
        "inp 1 4\n"
        "pol 1 10 200.0 black gray\n");
    roda(c, "pol1.svg", "pol1.txt");

    /* 4 círculos + 4 bordas (d=200 > altura 100, sem preenchimento) */
    TEST_ASSERT_EQUAL_INT(8, totalFormas(F));
}

void test_qry_Pol_QuadradoComPreenchimento(void) {
    insereCirculo(F, 1,   0.0,   0.0, 1.0, "black", "red");
    insereCirculo(F, 2, 100.0,   0.0, 1.0, "black", "red");
    insereCirculo(F, 3, 100.0, 100.0, 1.0, "black", "red");
    insereCirculo(F, 4,   0.0, 100.0, 1.0, "black", "red");

    /* d=25 → linhas em y=25, 50, 75 → 3 preenchimentos */
    const char *c = qry("pol2.qry",
        "inp 1 1\n"
        "inp 1 2\n"
        "inp 1 3\n"
        "inp 1 4\n"
        "pol 1 10 25.0 black gray\n");
    roda(c, "pol2.svg", "pol2.txt");

    /* 4 círculos + 4 bordas + 3 preenchimentos = 11 */
    TEST_ASSERT_EQUAL_INT(11, totalFormas(F));
}

void test_qry_Pol_IdsSequenciais(void) {
    insereCirculo(F, 1,   0.0,   0.0, 1.0, "black", "red");
    insereCirculo(F, 2, 100.0,   0.0, 1.0, "black", "red");
    insereCirculo(F, 3, 100.0, 100.0, 1.0, "black", "red");
    insereCirculo(F, 4,   0.0, 100.0, 1.0, "black", "red");

    const char *c = qry("pol3.qry",
        "inp 1 1\n"
        "inp 1 2\n"
        "inp 1 3\n"
        "inp 1 4\n"
        "pol 1 50 200.0 black gray\n");
    roda(c, "pol3.svg", "pol3.txt");

    /* IDs das bordas: 50, 51, 52, 53 */
    TEST_ASSERT_NOT_NULL(buscaFormaPorId(F, 50));
    TEST_ASSERT_NOT_NULL(buscaFormaPorId(F, 51));
    TEST_ASSERT_NOT_NULL(buscaFormaPorId(F, 52));
    TEST_ASSERT_NOT_NULL(buscaFormaPorId(F, 53));
}

void test_qry_Pol_PoligonoSemPontos_NaoInsereLinhas(void) {
    int antes = totalFormas(F);
    const char *c = qry("pol4.qry", "pol 1 10 10.0 black gray\n");
    roda(c, "pol4.svg", "pol4.txt");
    TEST_ASSERT_EQUAL_INT(antes, totalFormas(F));
}

/* ═══════════════════════════════════════════════
   GRUPO 9: sequências combinadas
   ═══════════════════════════════════════════════ */

void test_qry_SelDepoisDeDels_BancoCorreto(void) {
    insereCirculo(F, 1, 50.0,  50.0,  5.0, "black", "red");
    insereCirculo(F, 2, 500.0, 500.0, 5.0, "black", "blue");

    const char *c = qry("combo1.qry",
        "sel 0.0 0.0 100.0 100.0\n"
        "dels\n"
        "sel 0.0 0.0 9999.0 9999.0\n");
    roda(c, "combo1.svg", "combo1.txt");

    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
    TEST_ASSERT_EQUAL_INT(1, totalFormasSelecionadas(F));
    TEST_ASSERT_NOT_NULL(buscaFormaPorId(F, 2));
}

void test_qry_InpRmpClp_Sequencia(void) {
    insereCirculo(F, 1, 10.0, 10.0, 3.0, "black", "red");
    insereCirculo(F, 2, 20.0, 20.0, 3.0, "black", "red");

    const char *c = qry("combo2.qry",
        "inp 1 1\n"
        "inp 1 2\n"
        "rmp 1\n"
        "inp 1 1\n"
        "clp 1\n");
    roda(c, "combo2.svg", "combo2.txt");

    TEST_ASSERT_TRUE(isEmptyPoligono(1));
}

void test_qry_McsDepoisDeSel_TxtVazio(void) {
    /* mcs não produz saída no TXT */
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    const char *c = qry("combo3.qry",
        "sel 0.0 0.0 200.0 200.0\n"
        "mcs 5.0 5.0 blue green\n");
    roda(c, "combo3.svg", "combo3.txt");

    /* TXT deve ter apenas a linha do sel */
    const char *s = le("combo3.txt");
    TEST_ASSERT_NOT_NULL(strstr(s, "circulo"));
    /* não deve ter "mcs" no txt */
    TEST_ASSERT_NULL(strstr(s, "mcs"));
}

/* ═══════════════════════════════════════════════
   Runner
   ═══════════════════════════════════════════════ */

int main(void) {
    UNITY_BEGIN();

    /* Defensivos */
    RUN_TEST(test_qry_ArquivoInexistente_RetornaFalse);
    RUN_TEST(test_qry_CaminhoNulo_RetornaFalse);
    RUN_TEST(test_qry_BancoNulo_RetornaFalse);
    RUN_TEST(test_qry_SvgNulo_RetornaFalse);
    RUN_TEST(test_qry_TxtNulo_RetornaFalse);
    RUN_TEST(test_qry_ArquivoVazio_RetornaTrue);
    RUN_TEST(test_qry_SoComentarios_NaoAlteraBanco);
    RUN_TEST(test_qry_ComandoDesconhecido_Ignorado);

    /* inp */
    RUN_TEST(test_qry_Inp_InsereNaFila);
    RUN_TEST(test_qry_Inp_CoordenadasCorretas);
    RUN_TEST(test_qry_Inp_TxtContemPiIeTipo);
    RUN_TEST(test_qry_Inp_MultiplosPoligonos);
    RUN_TEST(test_qry_Inp_IdInexistente_NaoInsere);
    RUN_TEST(test_qry_Inp_AncoraDeLinha_MenorX);

    /* rmp */
    RUN_TEST(test_qry_Rmp_RemovePonto);
    RUN_TEST(test_qry_Rmp_TxtContemCoordenada);
    RUN_TEST(test_qry_Rmp_FilaVazia_NaoEscreve);
    RUN_TEST(test_qry_Rmp_OrdemFIFO);

    /* clp */
    RUN_TEST(test_qry_Clp_LimpaPoligono);
    RUN_TEST(test_qry_Clp_NaoAfetaOutroPoligono);

    /* sel */
    RUN_TEST(test_qry_Sel_SelecionaFormaDentro);
    RUN_TEST(test_qry_Sel_NaoSelecionaFormaFora);
    RUN_TEST(test_qry_Sel_TxtContemIdETipo);
    RUN_TEST(test_qry_Sel_SvgContemDasharray);
    RUN_TEST(test_qry_Sel_SvgContemAnelo);
    RUN_TEST(test_qry_Sel_DescartaSelecaoAnterior);
    RUN_TEST(test_qry_Sel_MultiplasFormas);

    /* dels */
    RUN_TEST(test_qry_Dels_RemoveFormasSelecionadas);
    RUN_TEST(test_qry_Dels_NaoRemoveNaoSelecionadas);
    RUN_TEST(test_qry_Dels_TxtContemIdETipo);
    RUN_TEST(test_qry_Dels_TxtContemDadosDaForma);
    RUN_TEST(test_qry_Dels_SvgContemXVermelho);
    RUN_TEST(test_qry_Dels_SemSelecao_NaoRemoveNada);

    /* mcs */
    RUN_TEST(test_qry_Mcs_TransladaX);
    RUN_TEST(test_qry_Mcs_TransladaY);
    RUN_TEST(test_qry_Mcs_MudaCores);
    RUN_TEST(test_qry_Mcs_NaoAfetaNaoSelecionadas);
    RUN_TEST(test_qry_Mcs_RetanguloTransladado);

    /* pol */
    RUN_TEST(test_qry_Pol_QuadradoGeraBordas);
    RUN_TEST(test_qry_Pol_QuadradoComPreenchimento);
    RUN_TEST(test_qry_Pol_IdsSequenciais);
    RUN_TEST(test_qry_Pol_PoligonoSemPontos_NaoInsereLinhas);

    /* combinados */
    RUN_TEST(test_qry_SelDepoisDeDels_BancoCorreto);
    RUN_TEST(test_qry_InpRmpClp_Sequencia);
    RUN_TEST(test_qry_McsDepoisDeSel_TxtVazio);

    return UNITY_END();
}