#include "unity.h"
#include "../src/poligono.h"

// O Unity roda esta função ANTES de cada teste individual
void setUp(void)
{
    inicializaPoligonos();
}

// O Unity roda esta função DEPOIS de cada teste individual
void tearDown(void)
{
    destroiPoligonos();
}

/* ─────────────────────────────────────────────
   inicializaPoligonos
   ───────────────────────────────────────────── */

void test_inicializa_TodosVazios(void)
{
    for (int p = 1; p <= MAX_POLIGONOS; p++) {
        TEST_ASSERT_TRUE_MESSAGE(isEmptyPoligono(p), "Todo poligono deve iniciar vazio.");
        TEST_ASSERT_FALSE_MESSAGE(isFullPoligono(p), "Nenhum poligono deve iniciar cheio.");
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthPoligono(p), "Tamanho inicial deve ser 0.");
    }
}

/* ─────────────────────────────────────────────
   identificadores inválidos
   ───────────────────────────────────────────── */

void test_IdentificadorInvalido(void)
{
    double x, y;
    int n;
    double xs[MAX_PONTOS_POLIGONO], ys[MAX_PONTOS_POLIGONO];

    // p = 0 (abaixo do mínimo)
    TEST_ASSERT_FALSE_MESSAGE(inserePoligono(0, 1.0, 2.0),  "p=0 deve retornar false em insere.");
    TEST_ASSERT_FALSE_MESSAGE(removePoligono(0, &x, &y),    "p=0 deve retornar false em remove.");
    TEST_ASSERT_FALSE_MESSAGE(peekPoligono(0, &x, &y),      "p=0 deve retornar false em peek.");
    TEST_ASSERT_FALSE_MESSAGE(limpaPoligono(0),              "p=0 deve retornar false em limpa.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, lengthPoligono(0),    "p=0 deve retornar -1 em length.");
    TEST_ASSERT_FALSE_MESSAGE(isEmptyPoligono(0),            "p=0 deve retornar false em isEmpty.");
    TEST_ASSERT_FALSE_MESSAGE(isFullPoligono(0),             "p=0 deve retornar false em isFull.");
    TEST_ASSERT_FALSE_MESSAGE(getPontosPoligono(0, xs, ys, &n), "p=0 deve retornar false em getPontos.");

    // p = MAX_POLIGONOS + 1 (acima do máximo)
    int inv = MAX_POLIGONOS + 1;
    TEST_ASSERT_FALSE_MESSAGE(inserePoligono(inv, 1.0, 2.0), "p invalido deve retornar false em insere.");
    TEST_ASSERT_FALSE_MESSAGE(removePoligono(inv, &x, &y),   "p invalido deve retornar false em remove.");
    TEST_ASSERT_FALSE_MESSAGE(peekPoligono(inv, &x, &y),     "p invalido deve retornar false em peek.");
    TEST_ASSERT_FALSE_MESSAGE(limpaPoligono(inv),             "p invalido deve retornar false em limpa.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, lengthPoligono(inv),   "p invalido deve retornar -1 em length.");
    TEST_ASSERT_FALSE_MESSAGE(isEmptyPoligono(inv),           "p invalido deve retornar false em isEmpty.");
    TEST_ASSERT_FALSE_MESSAGE(isFullPoligono(inv),            "p invalido deve retornar false em isFull.");
    TEST_ASSERT_FALSE_MESSAGE(getPontosPoligono(inv, xs, ys, &n), "p invalido deve retornar false em getPontos.");
}

/* ─────────────────────────────────────────────
   inserePoligono
   ───────────────────────────────────────────── */

void test_insere_UmPonto(void)
{
    bool ok = inserePoligono(1, 10.0, 20.0);

    TEST_ASSERT_TRUE_MESSAGE(ok, "inserePoligono deve retornar true.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(1), "Tamanho deve ser 1 apos insercao.");
    TEST_ASSERT_FALSE_MESSAGE(isEmptyPoligono(1), "Poligono nao deve estar vazio apos insercao.");
}

void test_insere_MultiplosPontos(void)
{
    inserePoligono(1, 0.0,  0.0);
    inserePoligono(1, 10.0, 0.0);
    inserePoligono(1, 10.0, 10.0);
    inserePoligono(1, 0.0,  10.0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(4, lengthPoligono(1), "Tamanho deve ser 4 apos 4 insercoes.");
}

void test_insere_PoligonosIndependentes(void)
{
    // Inserções em polígonos diferentes não devem interferir
    inserePoligono(1, 1.0, 1.0);
    inserePoligono(2, 2.0, 2.0);
    inserePoligono(3, 3.0, 3.0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(1), "Poligono 1 deve ter 1 ponto.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(2), "Poligono 2 deve ter 1 ponto.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(3), "Poligono 3 deve ter 1 ponto.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthPoligono(4), "Poligono 4 deve continuar vazio.");
}

void test_insere_Cheio(void)
{
    for (int i = 0; i < MAX_PONTOS_POLIGONO; i++) {
        TEST_ASSERT_TRUE(inserePoligono(1, (double)i, (double)i));
    }

    TEST_ASSERT_TRUE_MESSAGE(isFullPoligono(1), "Poligono deve estar cheio.");

    bool ok = inserePoligono(1, 99.0, 99.0);
    TEST_ASSERT_FALSE_MESSAGE(ok, "Inserir em poligono cheio deve retornar false.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(MAX_PONTOS_POLIGONO, lengthPoligono(1), "Tamanho nao deve ultrapassar o maximo.");
}

/* ─────────────────────────────────────────────
   removePoligono
   ───────────────────────────────────────────── */

void test_remove_OrdemFIFO(void)
{
    inserePoligono(1, 1.0, 2.0);
    inserePoligono(1, 3.0, 4.0);
    inserePoligono(1, 5.0, 6.0);

    double x, y;

    removePoligono(1, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, x, "x do primeiro removido deve ser 1.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, y, "y do primeiro removido deve ser 2.0.");

    removePoligono(1, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, x, "x do segundo removido deve ser 3.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(4.0, y, "y do segundo removido deve ser 4.0.");

    removePoligono(1, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0, x, "x do terceiro removido deve ser 5.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(6.0, y, "y do terceiro removido deve ser 6.0.");

    TEST_ASSERT_TRUE_MESSAGE(isEmptyPoligono(1), "Poligono deve estar vazio apos remover tudo.");
}

void test_remove_AtualizaTamanho(void)
{
    inserePoligono(1, 1.0, 1.0);
    inserePoligono(1, 2.0, 2.0);

    double x, y;
    removePoligono(1, &x, &y);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(1), "Tamanho deve ser 1 apos uma remocao.");
}

void test_remove_Vazio(void)
{
    double x = -1.0, y = -1.0;
    bool ok = removePoligono(1, &x, &y);

    TEST_ASSERT_FALSE_MESSAGE(ok, "Remover de poligono vazio deve retornar false.");
}

/* ─────────────────────────────────────────────
   peekPoligono
   ───────────────────────────────────────────── */

void test_peek_NaoRemove(void)
{
    inserePoligono(1, 7.0, 8.0);
    inserePoligono(1, 9.0, 10.0);

    double x, y;

    peekPoligono(1, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7.0, x, "peek deve retornar o x do primeiro ponto.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(8.0, y, "peek deve retornar o y do primeiro ponto.");

    // peek repetido deve retornar o mesmo ponto
    peekPoligono(1, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7.0, x, "peek repetido deve retornar o mesmo x.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(8.0, y, "peek repetido deve retornar o mesmo y.");

    TEST_ASSERT_EQUAL_INT_MESSAGE(2, lengthPoligono(1), "Tamanho nao deve mudar apos peek.");
}

void test_peek_Vazio(void)
{
    double x, y;
    bool ok = peekPoligono(1, &x, &y);
    TEST_ASSERT_FALSE_MESSAGE(ok, "peek em poligono vazio deve retornar false.");
}

/* ─────────────────────────────────────────────
   limpaPoligono
   ───────────────────────────────────────────── */

void test_limpa_EsvaziaTudo(void)
{
    inserePoligono(1, 1.0, 1.0);
    inserePoligono(1, 2.0, 2.0);
    inserePoligono(1, 3.0, 3.0);

    bool ok = limpaPoligono(1);

    TEST_ASSERT_TRUE_MESSAGE(ok, "limpaPoligono deve retornar true.");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyPoligono(1), "Poligono deve estar vazio apos limpa.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthPoligono(1), "Tamanho deve ser 0 apos limpa.");
}

void test_limpa_ReutilizacaoAposLimpa(void)
{
    inserePoligono(1, 1.0, 2.0);
    limpaPoligono(1);

    // Deve conseguir inserir normalmente após limpar
    bool ok = inserePoligono(1, 5.0, 6.0);
    TEST_ASSERT_TRUE_MESSAGE(ok, "Deve conseguir inserir apos limpar.");

    double x, y;
    peekPoligono(1, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0, x, "x apos limpar e reinserir deve ser 5.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(6.0, y, "y apos limpar e reinserir deve ser 6.0.");
}

void test_limpa_NaoAfetaOutrosPoligonos(void)
{
    inserePoligono(1, 1.0, 1.0);
    inserePoligono(2, 2.0, 2.0);

    limpaPoligono(1);

    TEST_ASSERT_TRUE_MESSAGE(isEmptyPoligono(1), "Poligono 1 deve estar vazio apos limpa.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(2), "Poligono 2 nao deve ser afetado.");
}

/* ─────────────────────────────────────────────
   getPontosPoligono
   ───────────────────────────────────────────── */

void test_getPontos_OrdemCorreta(void)
{
    inserePoligono(1, 1.0, 2.0);
    inserePoligono(1, 3.0, 4.0);
    inserePoligono(1, 5.0, 6.0);

    double xs[MAX_PONTOS_POLIGONO], ys[MAX_PONTOS_POLIGONO];
    int n;
    bool ok = getPontosPoligono(1, xs, ys, &n);

    TEST_ASSERT_TRUE_MESSAGE(ok, "getPontosPoligono deve retornar true.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, n, "Deve retornar 3 pontos.");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, xs[0], "xs[0] deve ser 1.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, ys[0], "ys[0] deve ser 2.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, xs[1], "xs[1] deve ser 3.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(4.0, ys[1], "ys[1] deve ser 4.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0, xs[2], "xs[2] deve ser 5.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(6.0, ys[2], "ys[2] deve ser 6.0.");
}

void test_getPontos_NaoConsomeFila(void)
{
    inserePoligono(1, 1.0, 2.0);
    inserePoligono(1, 3.0, 4.0);

    double xs[MAX_PONTOS_POLIGONO], ys[MAX_PONTOS_POLIGONO];
    int n;
    getPontosPoligono(1, xs, ys, &n);

    // A fila não deve ter sido consumida
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, lengthPoligono(1), "Tamanho nao deve mudar apos getPontos.");

    double x, y;
    peekPoligono(1, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, x, "Primeiro ponto ainda deve ser (1.0, 2.0) apos getPontos.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, y, "Primeiro ponto ainda deve ser (1.0, 2.0) apos getPontos.");
}

void test_getPontos_FilaVazia(void)
{
    double xs[MAX_PONTOS_POLIGONO], ys[MAX_PONTOS_POLIGONO];
    int n;
    bool ok = getPontosPoligono(1, xs, ys, &n);

    TEST_ASSERT_TRUE_MESSAGE(ok, "getPontos em fila vazia deve retornar true.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, n, "n deve ser 0 para fila vazia.");
}

/* ─────────────────────────────────────────────
   comportamento circular da fila interna
   ───────────────────────────────────────────── */

void test_Circular_InsereRemoveIntercalados(void)
{
    // Enche metade, remove metade, enche de novo
    // para verificar a circularidade da fila interna
    for (int i = 0; i < MAX_PONTOS_POLIGONO / 2; i++) {
        inserePoligono(1, (double)i, (double)i);
    }

    double x, y;
    for (int i = 0; i < MAX_PONTOS_POLIGONO / 2; i++) {
        removePoligono(1, &x, &y);
    }

    TEST_ASSERT_TRUE_MESSAGE(isEmptyPoligono(1), "Poligono deve estar vazio apos remover tudo.");

    // Insere novamente para usar os slots liberados
    for (int i = 0; i < MAX_PONTOS_POLIGONO; i++) {
        bool ok = inserePoligono(1, (double)i * 2, (double)i * 2);
        TEST_ASSERT_TRUE_MESSAGE(ok, "Deve inserir corretamente reutilizando slots circulares.");
    }

    TEST_ASSERT_TRUE_MESSAGE(isFullPoligono(1), "Poligono deve estar cheio apos reinserir tudo.");
}

/* ─────────────────────────────────────────────
   todos os polígonos simultaneamente
   ───────────────────────────────────────────── */

void test_TodosPoligonosSimultaneos(void)
{
    // Insere um ponto diferente em cada polígono
    for (int p = 1; p <= MAX_POLIGONOS; p++) {
        inserePoligono(p, (double)p, (double)p * 10);
    }

    // Verifica cada polígono de forma independente
    for (int p = 1; p <= MAX_POLIGONOS; p++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthPoligono(p), "Cada poligono deve ter 1 ponto.");

        double x, y;
        peekPoligono(p, &x, &y);
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE((double)p,       x, "x deve corresponder ao poligono p.");
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE((double)p * 10,  y, "y deve corresponder ao poligono p.");
    }
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_inicializa_TodosVazios);

    RUN_TEST(test_IdentificadorInvalido);

    RUN_TEST(test_insere_UmPonto);
    RUN_TEST(test_insere_MultiplosPontos);
    RUN_TEST(test_insere_PoligonosIndependentes);
    RUN_TEST(test_insere_Cheio);

    RUN_TEST(test_remove_OrdemFIFO);
    RUN_TEST(test_remove_AtualizaTamanho);
    RUN_TEST(test_remove_Vazio);

    RUN_TEST(test_peek_NaoRemove);
    RUN_TEST(test_peek_Vazio);

    RUN_TEST(test_limpa_EsvaziaTudo);
    RUN_TEST(test_limpa_ReutilizacaoAposLimpa);
    RUN_TEST(test_limpa_NaoAfetaOutrosPoligonos);

    RUN_TEST(test_getPontos_OrdemCorreta);
    RUN_TEST(test_getPontos_NaoConsomeFila);
    RUN_TEST(test_getPontos_FilaVazia);

    RUN_TEST(test_Circular_InsereRemoveIntercalados);

    RUN_TEST(test_TodosPoligonosSimultaneos);

    return UNITY_END();
}