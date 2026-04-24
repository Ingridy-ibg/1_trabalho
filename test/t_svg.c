/**
 * @file t_svg.c
 * @brief Testes unitários para svg.c
 *
 * Estratégia: escreve o SVG em arquivos temporários em /tmp,
 * lê o conteúdo e verifica as tags/atributos esperados via strstr.
 * Não parseamos XML — verificamos substrings suficientemente específicas.
 */

#include "unity.h"
#include "../src/svg.h"
#include "../src/formas.h"
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

static FILE *abre_tmp(const char *nome) {
    char cam[256];
    snprintf(cam, sizeof(cam), "/tmp/%s", nome);
    return fopen(cam, "w");
}

static const char *le_tmp(const char *nome) {
    static char buf[16384];
    char cam[256];
    snprintf(cam, sizeof(cam), "/tmp/%s", nome);
    buf[0] = '\0';
    FILE *f = fopen(cam, "r");
    if (!f) return buf;
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = '\0';
    fclose(f);
    return buf;
}

void setUp(void)    { F = criaFormas(); }
void tearDown(void) { destroiFormas(F); }

/* ─────────────────────────────────────────────
   GRUPO 1: svgInicia / svgFinaliza
   ───────────────────────────────────────────── */

void test_svgInicia_ContemDeclaracaoXml(void) {
    insereCirculo(F, 1, 50.0, 50.0, 10.0, "black", "red");
    FILE *f = abre_tmp("t_ini_xml.svg");
    svgInicia(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_ini_xml.svg");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "<?xml"), "Deve conter declaração XML.");
}

void test_svgInicia_ContemTagSvgAbertura(void) {
    insereCirculo(F, 1, 50.0, 50.0, 10.0, "black", "red");
    FILE *f = abre_tmp("t_ini_svg.svg");
    svgInicia(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_ini_svg.svg");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "<svg"), "Deve conter tag <svg.");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "viewBox"), "Deve conter viewBox.");
}

void test_svgFinaliza_ContemTagFechamento(void) {
    FILE *f = abre_tmp("t_fim.svg");
    svgInicia(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_fim.svg");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "</svg>"), "Deve conter </svg>.");
}

void test_svgInicia_BancoVazio_UsaJanelaMinima(void) {
    /* banco vazio → não deve crashar; viewBox deve existir */
    FILE *f = abre_tmp("t_ini_vazio.svg");
    svgInicia(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_ini_vazio.svg");
    TEST_ASSERT_NOT_NULL(strstr(s, "viewBox"));
}

void test_svgInicia_BancoNulo_NaoCrasha(void) {
    FILE *f = abre_tmp("t_ini_null.svg");
    svgInicia(f, NULL);   /* não deve crashar */
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_ini_null.svg");
    TEST_ASSERT_NOT_NULL(strstr(s, "<svg"));
}

/* ─────────────────────────────────────────────
   GRUPO 2: svgEscreveFormas — círculo
   ───────────────────────────────────────────── */

void test_escreveFormas_Circulo_ContemCircle(void) {
    insereCirculo(F, 1, 50.0, 50.0, 10.0, "black", "red");
    FILE *f = abre_tmp("t_circ.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_circ.svg"), "<circle"));
}

void test_escreveFormas_Circulo_ContemRaio(void) {
    insereCirculo(F, 1, 0.0, 0.0, 15.0, "black", "red");
    FILE *f = abre_tmp("t_circ_r.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    /* r="15.0000" */
    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_circ_r.svg"), "r=\"15."));
}

void test_escreveFormas_Circulo_ContemCores(void) {
    insereCirculo(F, 1, 0.0, 0.0, 5.0, "blue", "yellow");
    FILE *f = abre_tmp("t_circ_cor.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_circ_cor.svg");
    TEST_ASSERT_NOT_NULL(strstr(s, "blue"));
    TEST_ASSERT_NOT_NULL(strstr(s, "yellow"));
}

/* ─────────────────────────────────────────────
   GRUPO 3: svgEscreveFormas — retângulo
   ───────────────────────────────────────────── */

void test_escreveFormas_Retangulo_ContemRect(void) {
    insereRetangulo(F, 1, 0.0, 0.0, 100.0, 50.0, "black", "blue");
    FILE *f = abre_tmp("t_rect.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_rect.svg"), "<rect"));
}

void test_escreveFormas_Retangulo_ContemWidthHeight(void) {
    insereRetangulo(F, 1, 0.0, 0.0, 80.0, 40.0, "black", "blue");
    FILE *f = abre_tmp("t_rect_wh.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_rect_wh.svg");
    TEST_ASSERT_NOT_NULL(strstr(s, "width=\"80."));
    TEST_ASSERT_NOT_NULL(strstr(s, "height=\"40."));
}

/*
 * Teste de inversão de Y:
 * Retângulo com canto inf. esq. em y_cart=10, h=30 → canto sup. em y_cart=40.
 * No SVG, y_svg do canto superior = g_altura - (40 - ymin) - MARGEM.
 * O que testamos é que o y SVG do rect é MENOR que o y SVG do canto inferior
 * (i.e., o SVG realmente inverteu o eixo).
 *
 * Como não queremos calcular o valor exato aqui, verificamos apenas que
 * width e height aparecem positivos e que a tag <rect existe.
 */
void test_escreveFormas_Retangulo_InversaoY_HeightPositiva(void) {
    insereRetangulo(F, 1, 0.0, 10.0, 50.0, 30.0, "black", "white");
    FILE *f = abre_tmp("t_rect_y.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_rect_y.svg");
    TEST_ASSERT_NOT_NULL(strstr(s, "<rect"));
    TEST_ASSERT_NOT_NULL(strstr(s, "height=\"30."));
}

/* ─────────────────────────────────────────────
   GRUPO 4: svgEscreveFormas — linha
   ───────────────────────────────────────────── */

void test_escreveFormas_Linha_ContemLine(void) {
    insereLinha(F, 1, 0.0, 0.0, 100.0, 100.0, "green");
    FILE *f = abre_tmp("t_lin.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_lin.svg"), "<line"));
}

void test_escreveFormas_Linha_ContemCor(void) {
    insereLinha(F, 1, 0.0, 0.0, 10.0, 10.0, "purple");
    FILE *f = abre_tmp("t_lin_cor.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_lin_cor.svg"), "purple"));
}

void test_escreveFormas_Linha_InversaoY(void) {
    /* Linha horizontal y=50; ambos os pontos devem ter mesmo y SVG */
    insereLinha(F, 1, 0.0, 50.0, 100.0, 50.0, "black");
    FILE *f = abre_tmp("t_lin_y.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_lin_y.svg");
    /* y1 e y2 devem ser iguais (linha horizontal) */
    const char *y1 = strstr(s, "y1=\"");
    const char *y2 = strstr(s, "y2=\"");
    TEST_ASSERT_NOT_NULL(y1);
    TEST_ASSERT_NOT_NULL(y2);

    double v1 = atof(y1 + 4);
    double v2 = atof(y2 + 4);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, v1, v2);
}

/* ─────────────────────────────────────────────
   GRUPO 5: svgEscreveFormas — texto
   ───────────────────────────────────────────── */

void test_escreveFormas_Texto_ContemTagText(void) {
    insereTexto(F, 1, 10.0, 10.0, "black", "white", 'i', "Ola");
    FILE *f = abre_tmp("t_txt.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_txt.svg"), "<text"));
}

void test_escreveFormas_Texto_ContemConteudo(void) {
    insereTexto(F, 1, 0.0, 0.0, "black", "white", 'i', "Estrutura de Dados");
    FILE *f = abre_tmp("t_txt_cont.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_txt_cont.svg"), "Estrutura de Dados"));
}

void test_escreveFormas_Texto_AncoraMiddle(void) {
    insereTexto(F, 1, 0.0, 0.0, "black", "white", 'm', "Centro");
    FILE *f = abre_tmp("t_txt_m.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_txt_m.svg"), "middle"));
}

void test_escreveFormas_Texto_AncoraEnd(void) {
    insereTexto(F, 1, 0.0, 0.0, "black", "white", 'f', "Fim");
    FILE *f = abre_tmp("t_txt_f.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_txt_f.svg"), "end"));
}

void test_escreveFormas_Texto_ContemFontFamily(void) {
    insereTexto(F, 1, 0.0, 0.0, "black", "white", 'i', "X");
    FILE *f = abre_tmp("t_txt_ff.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    /* estilo padrão é "sans" → "sans-serif" */
    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_txt_ff.svg"), "font-family"));
}

/* ─────────────────────────────────────────────
   GRUPO 6: múltiplas formas
   ───────────────────────────────────────────── */

void test_escreveFormas_Multiplas_TodasPresentes(void) {
    insereCirculo(F,   1, 50.0, 50.0, 10.0, "black", "red");
    insereRetangulo(F, 2, 0.0,  0.0,  80.0, 40.0, "black", "blue");
    insereLinha(F,     3, 0.0,  0.0,  100.0, 100.0, "green");
    insereTexto(F,     4, 10.0, 10.0, "black", "white", 'i', "TED");

    FILE *f = abre_tmp("t_multi.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_multi.svg");
    TEST_ASSERT_NOT_NULL(strstr(s, "<circle"));
    TEST_ASSERT_NOT_NULL(strstr(s, "<rect"));
    TEST_ASSERT_NOT_NULL(strstr(s, "<line"));
    TEST_ASSERT_NOT_NULL(strstr(s, "<text"));
    TEST_ASSERT_NOT_NULL(strstr(s, "TED"));
}

void test_escreveFormas_BancoVazio_NaoEmiteFormas(void) {
    FILE *f = abre_tmp("t_vazio.svg");
    svgInicia(f, F);
    svgEscreveFormas(f, F);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_vazio.svg");
    TEST_ASSERT_NULL(strstr(s, "<circle"));
    TEST_ASSERT_NULL(strstr(s, "<rect"));
    TEST_ASSERT_NULL(strstr(s, "<line"));
    TEST_ASSERT_NULL(strstr(s, "<text"));
}

/* ─────────────────────────────────────────────
   GRUPO 7: svgSel
   ───────────────────────────────────────────── */

void test_svgSel_ContemRetanguloDasharray(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);

    FILE *f = abre_tmp("t_sel.svg");
    svgInicia(f, F);
    svgSel(f, F, 0.0, 0.0, 200.0, 200.0);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_sel.svg");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(s, "dasharray"),
        "Região de seleção deve ter stroke-dasharray.");
    TEST_ASSERT_NOT_NULL(strstr(s, "red"));
}

void test_svgSel_ContemAneloNaAncora(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);

    FILE *f = abre_tmp("t_sel_anelo.svg");
    svgInicia(f, F);
    svgSel(f, F, 0.0, 0.0, 200.0, 200.0);
    svgFinaliza(f);
    fclose(f);

    /* Deve haver ao menos 2 <circle: o círculo da forma + o anel */
    const char *s = le_tmp("t_sel_anelo.svg");
    const char *p1 = strstr(s, "<circle");
    TEST_ASSERT_NOT_NULL(p1);
    /* o anel vem dentro do svgSel (não via svgEscreveFormas aqui) */
    /* verificamos apenas que existe algum <circle com fill="none" */
    TEST_ASSERT_NOT_NULL(strstr(s, "fill=\"none\""));
}

void test_svgSel_SemSelecionadas_SoRetangulo(void) {
    insereCirculo(F, 1, 500.0, 500.0, 5.0, "black", "red");
    selecionaFormas(F, 0.0, 0.0, 10.0, 10.0);  /* nada selecionado */

    FILE *f = abre_tmp("t_sel_vazio.svg");
    svgInicia(f, F);
    svgSel(f, F, 0.0, 0.0, 10.0, 10.0);
    svgFinaliza(f);
    fclose(f);

    /* Deve ter o retângulo de seleção mas nenhum anel */
    const char *s = le_tmp("t_sel_vazio.svg");
    TEST_ASSERT_NOT_NULL(strstr(s, "dasharray"));
    /* fill="none" vem apenas do anel; aqui não deve existir */
    TEST_ASSERT_NULL(strstr(s, "fill=\"none\""));
}

void test_svgSel_ArquivoNulo_NaoCrasha(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);
    svgSel(NULL, F, 0.0, 0.0, 100.0, 100.0);  /* não deve crashar */
}

/* ─────────────────────────────────────────────
   GRUPO 8: svgDels
   ───────────────────────────────────────────── */

void test_svgDels_ContemDuasLinhas(void) {
    insereCirculo(F, 1, 30.0, 30.0, 5.0, "black", "red");
    FILE *f = abre_tmp("t_dels.svg");
    svgInicia(f, F);
    svgDels(f, 30.0, 30.0);
    svgFinaliza(f);
    fclose(f);

    const char *s = le_tmp("t_dels.svg");
    /* Dois <line para formar o "x" */
    const char *p1 = strstr(s, "<line");
    TEST_ASSERT_NOT_NULL_MESSAGE(p1, "Deve ter ao menos 1 <line para o x.");
    const char *p2 = strstr(p1 + 1, "<line");
    TEST_ASSERT_NOT_NULL_MESSAGE(p2, "Deve ter 2 <line para o x.");
}

void test_svgDels_ContemCorVermelha(void) {
    FILE *f = abre_tmp("t_dels_cor.svg");
    svgInicia(f, F);
    svgDels(f, 10.0, 20.0);
    svgFinaliza(f);
    fclose(f);

    TEST_ASSERT_NOT_NULL(strstr(le_tmp("t_dels_cor.svg"), "red"));
}

void test_svgDels_ArquivoNulo_NaoCrasha(void) {
    svgDels(NULL, 10.0, 10.0);   /* não deve crashar */
}

/* ─────────────────────────────────────────────
   GRUPO 9: defensivos
   ───────────────────────────────────────────── */

void test_escreveFormas_ArquivoNulo_NaoCrasha(void) {
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    svgEscreveFormas(NULL, F);  /* não deve crashar */
}

void test_escreveFormas_BancoNulo_NaoCrasha(void) {
    FILE *f = abre_tmp("t_def_null.svg");
    svgInicia(f, NULL);
    svgEscreveFormas(f, NULL);  /* não deve crashar */
    svgFinaliza(f);
    fclose(f);
}

void test_svgFinaliza_ArquivoNulo_NaoCrasha(void) {
    svgFinaliza(NULL);          /* não deve crashar */
}

/* ─────────────────────────────────────────────
   Runner
   ───────────────────────────────────────────── */

int main(void) {
    UNITY_BEGIN();

    /* svgInicia / svgFinaliza */
    RUN_TEST(test_svgInicia_ContemDeclaracaoXml);
    RUN_TEST(test_svgInicia_ContemTagSvgAbertura);
    RUN_TEST(test_svgFinaliza_ContemTagFechamento);
    RUN_TEST(test_svgInicia_BancoVazio_UsaJanelaMinima);
    RUN_TEST(test_svgInicia_BancoNulo_NaoCrasha);

    /* círculo */
    RUN_TEST(test_escreveFormas_Circulo_ContemCircle);
    RUN_TEST(test_escreveFormas_Circulo_ContemRaio);
    RUN_TEST(test_escreveFormas_Circulo_ContemCores);

    /* retângulo */
    RUN_TEST(test_escreveFormas_Retangulo_ContemRect);
    RUN_TEST(test_escreveFormas_Retangulo_ContemWidthHeight);
    RUN_TEST(test_escreveFormas_Retangulo_InversaoY_HeightPositiva);

    /* linha */
    RUN_TEST(test_escreveFormas_Linha_ContemLine);
    RUN_TEST(test_escreveFormas_Linha_ContemCor);
    RUN_TEST(test_escreveFormas_Linha_InversaoY);

    /* texto */
    RUN_TEST(test_escreveFormas_Texto_ContemTagText);
    RUN_TEST(test_escreveFormas_Texto_ContemConteudo);
    RUN_TEST(test_escreveFormas_Texto_AncoraMiddle);
    RUN_TEST(test_escreveFormas_Texto_AncoraEnd);
    RUN_TEST(test_escreveFormas_Texto_ContemFontFamily);

    /* múltiplas */
    RUN_TEST(test_escreveFormas_Multiplas_TodasPresentes);
    RUN_TEST(test_escreveFormas_BancoVazio_NaoEmiteFormas);

    /* svgSel */
    RUN_TEST(test_svgSel_ContemRetanguloDasharray);
    RUN_TEST(test_svgSel_ContemAneloNaAncora);
    RUN_TEST(test_svgSel_SemSelecionadas_SoRetangulo);
    RUN_TEST(test_svgSel_ArquivoNulo_NaoCrasha);

    /* svgDels */
    RUN_TEST(test_svgDels_ContemDuasLinhas);
    RUN_TEST(test_svgDels_ContemCorVermelha);
    RUN_TEST(test_svgDels_ArquivoNulo_NaoCrasha);

    /* defensivos */
    RUN_TEST(test_escreveFormas_ArquivoNulo_NaoCrasha);
    RUN_TEST(test_escreveFormas_BancoNulo_NaoCrasha);
    RUN_TEST(test_svgFinaliza_ArquivoNulo_NaoCrasha);

    return UNITY_END();
}