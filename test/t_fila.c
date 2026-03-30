#include "unity.h"
#include "../src/fila.h"

#define CAPACIDADE 5

Fila F;
int dado1 = 10, dado2 = 20, dado3 = 30;

// O Unity roda esta função ANTES de cada teste individual
void setUp(void)
{
    F = criaFila(CAPACIDADE);
}

// O Unity roda esta função DEPOIS de cada teste individual
void tearDown(void)
{
    destroiFila(F, NULL);
}

/* ─────────────────────────────────────────────
   criaFila
   ───────────────────────────────────────────── */

void test_criaFila(void)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(F, "A fila criada nao deve ser NULL.");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(F), "A fila recem-criada deve estar vazia.");
    TEST_ASSERT_FALSE_MESSAGE(isFullFila(F), "A fila recem-criada nao deve estar cheia.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthFila(F), "O tamanho inicial deve ser 0.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(CAPACIDADE, capacidadeFila(F), "A capacidade deve ser a informada na criacao.");
    TEST_ASSERT_NULL_MESSAGE(peek(F), "peek em fila vazia deve retornar NULL.");
}

void test_criaFila_CapacidadeInvalida(void)
{
    Fila fi = criaFila(0);
    TEST_ASSERT_NULL_MESSAGE(fi, "Criar fila com capacidade 0 deve retornar NULL.");

    Fila fn = criaFila(-1);
    TEST_ASSERT_NULL_MESSAGE(fn, "Criar fila com capacidade negativa deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   enqueue
   ───────────────────────────────────────────── */

void test_enqueue_FilaVazia(void)
{
    bool ok = enqueue(F, &dado1);

    TEST_ASSERT_TRUE_MESSAGE(ok, "enqueue deve retornar true em fila valida.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthFila(F), "Tamanho deve ser 1 apos insercao.");
    TEST_ASSERT_FALSE_MESSAGE(isEmptyFila(F), "Fila nao deve estar vazia apos insercao.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, peek(F), "peek deve retornar o primeiro elemento inserido.");
}

void test_enqueue_MultiplosElementos(void)
{
    enqueue(F, &dado1);
    enqueue(F, &dado2);
    enqueue(F, &dado3);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, lengthFila(F), "Tamanho deve ser 3.");
    // A ordem FIFO garante que o primeiro inserido é o primeiro a sair
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, peek(F), "peek deve retornar o dado1 (primeiro inserido).");
}

void test_enqueue_FilaCheia(void)
{
    int extras[CAPACIDADE];
    for (int i = 0; i < CAPACIDADE; i++) {
        extras[i] = i;
        enqueue(F, &extras[i]);
    }

    TEST_ASSERT_TRUE_MESSAGE(isFullFila(F), "Fila deve estar cheia.");

    // Tentar inserir em fila cheia deve retornar false
    bool ok = enqueue(F, &dado1);
    TEST_ASSERT_FALSE_MESSAGE(ok, "enqueue em fila cheia deve retornar false.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(CAPACIDADE, lengthFila(F), "Tamanho nao deve ultrapassar a capacidade.");
}

void test_enqueue_FilaNula(void)
{
    bool ok = enqueue(NULL, &dado1);
    TEST_ASSERT_FALSE_MESSAGE(ok, "enqueue em fila NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   dequeue
   ───────────────────────────────────────────── */

void test_dequeue_UnicoElemento(void)
{
    enqueue(F, &dado1);

    Item removido = dequeue(F);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, removido, "dequeue deve retornar o dado1.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthFila(F), "Tamanho deve ser 0 apos remocao.");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(F), "Fila deve estar vazia apos remocao do unico elemento.");
    TEST_ASSERT_NULL_MESSAGE(peek(F), "peek em fila vazia deve retornar NULL.");
}

void test_dequeue_OrdemFIFO(void)
{
    enqueue(F, &dado1);
    enqueue(F, &dado2);
    enqueue(F, &dado3);

    Item r1 = dequeue(F);
    Item r2 = dequeue(F);
    Item r3 = dequeue(F);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, r1, "Primeiro removido deve ser dado1.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado2, r2, "Segundo removido deve ser dado2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado3, r3, "Terceiro removido deve ser dado3.");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(F), "Fila deve estar vazia apos remover todos.");
}

void test_dequeue_FilaVazia(void)
{
    Item removido = dequeue(F);
    TEST_ASSERT_NULL_MESSAGE(removido, "dequeue em fila vazia deve retornar NULL.");
}

void test_dequeue_FilaNula(void)
{
    Item removido = dequeue(NULL);
    TEST_ASSERT_NULL_MESSAGE(removido, "dequeue em fila NULL deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   Comportamento circular
   ───────────────────────────────────────────── */

void test_Circular_EnqueueDequeueIntercalados(void)
{
    // Preenche a fila até a metade
    enqueue(F, &dado1);
    enqueue(F, &dado2);
    enqueue(F, &dado3);

    // Remove dois elementos — libera slots no início
    dequeue(F);
    dequeue(F);

    // Insere mais dois — deve reutilizar os slots do início (comportamento circular)
    int dado4 = 40, dado5 = 50;
    bool ok4 = enqueue(F, &dado4);
    bool ok5 = enqueue(F, &dado5);

    TEST_ASSERT_TRUE_MESSAGE(ok4, "Deve inserir dado4 reutilizando slot circular.");
    TEST_ASSERT_TRUE_MESSAGE(ok5, "Deve inserir dado5 reutilizando slot circular.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, lengthFila(F), "Tamanho deve ser 3.");

    // Ordem esperada: dado3, dado4, dado5
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado3, dequeue(F), "Primeiro deve ser dado3.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado4, dequeue(F), "Segundo deve ser dado4.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado5, dequeue(F), "Terceiro deve ser dado5.");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(F), "Fila deve estar vazia ao final.");
}

void test_Circular_EncheEsvaziaMuitasVezes(void)
{
    // Enche e esvazia a fila 3 vezes para garantir a circularidade
    for (int volta = 0; volta < 3; volta++) {
        int dados[CAPACIDADE];
        for (int i = 0; i < CAPACIDADE; i++) {
            dados[i] = i + volta * 10;
            bool ok = enqueue(F, &dados[i]);
            TEST_ASSERT_TRUE_MESSAGE(ok, "enqueue deve ter sucesso ao preencher a fila.");
        }
        TEST_ASSERT_TRUE_MESSAGE(isFullFila(F), "Fila deve estar cheia.");

        for (int i = 0; i < CAPACIDADE; i++) {
            Item r = dequeue(F);
            TEST_ASSERT_EQUAL_PTR_MESSAGE(&dados[i], r, "Ordem FIFO deve ser mantida.");
        }
        TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(F), "Fila deve estar vazia apos esvaziar.");
    }
}

/* ─────────────────────────────────────────────
   peek
   ───────────────────────────────────────────── */

void test_peek_NaoRemove(void)
{
    enqueue(F, &dado1);
    enqueue(F, &dado2);

    // peek não deve alterar o tamanho
    Item p1 = peek(F);
    Item p2 = peek(F);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, p1, "peek deve retornar o dado1.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, p2, "peek repetido deve retornar o mesmo elemento.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, lengthFila(F), "Tamanho nao deve mudar apos peek.");
}

void test_peek_FilaNula(void)
{
    TEST_ASSERT_NULL_MESSAGE(peek(NULL), "peek em fila NULL deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   isEmptyFila / isFullFila
   ───────────────────────────────────────────── */

void test_isEmptyFila(void)
{
    TEST_ASSERT_TRUE(isEmptyFila(F));
    enqueue(F, &dado1);
    TEST_ASSERT_FALSE(isEmptyFila(F));
    dequeue(F);
    TEST_ASSERT_TRUE(isEmptyFila(F));
}

void test_isFullFila(void)
{
    TEST_ASSERT_FALSE(isFullFila(F));

    int dados[CAPACIDADE];
    for (int i = 0; i < CAPACIDADE; i++) {
        dados[i] = i;
        enqueue(F, &dados[i]);
    }

    TEST_ASSERT_TRUE(isFullFila(F));
    dequeue(F);
    TEST_ASSERT_FALSE(isFullFila(F));
}

void test_isEmptyFila_Nula(void)
{
    TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(NULL), "isEmptyFila(NULL) deve retornar true.");
}

void test_isFullFila_Nula(void)
{
    TEST_ASSERT_FALSE_MESSAGE(isFullFila(NULL), "isFullFila(NULL) deve retornar false.");
}

/* ─────────────────────────────────────────────
   lengthFila / capacidadeFila
   ───────────────────────────────────────────── */

void test_lengthFila(void)
{
    TEST_ASSERT_EQUAL_INT(0, lengthFila(F));
    enqueue(F, &dado1);
    TEST_ASSERT_EQUAL_INT(1, lengthFila(F));
    enqueue(F, &dado2);
    TEST_ASSERT_EQUAL_INT(2, lengthFila(F));
    dequeue(F);
    TEST_ASSERT_EQUAL_INT(1, lengthFila(F));
}

void test_lengthFila_Nula(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthFila(NULL), "lengthFila(NULL) deve retornar 0.");
}

void test_capacidadeFila(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(CAPACIDADE, capacidadeFila(F), "Capacidade deve ser a definida.");
}

void test_capacidadeFila_Nula(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, capacidadeFila(NULL), "capacidadeFila(NULL) deve retornar 0.");
}

/* ─────────────────────────────────────────────
   clearFila
   ───────────────────────────────────────────── */

int destruicoes = 0;

void contaDestruicao(Item info)
{
    destruicoes++;
}

void test_clearFila_ComCallback(void)
{
    enqueue(F, &dado1);
    enqueue(F, &dado2);
    enqueue(F, &dado3);

    destruicoes = 0;
    clearFila(F, contaDestruicao);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, destruicoes, "O callback deve ser chamado para cada item.");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(F), "Fila deve estar vazia apos clear.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthFila(F), "Tamanho deve ser 0 apos clear.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(CAPACIDADE, capacidadeFila(F), "Capacidade nao deve mudar apos clear.");
}

void test_clearFila_SemCallback(void)
{
    enqueue(F, &dado1);
    clearFila(F, NULL);

    TEST_ASSERT_TRUE_MESSAGE(isEmptyFila(F), "Fila deve estar vazia apos clear sem callback.");
}

void test_clearFila_ReutilizacaoAposClear(void)
{
    enqueue(F, &dado1);
    enqueue(F, &dado2);
    clearFila(F, NULL);

    // Após o clear, a fila deve ser reutilizável normalmente
    bool ok = enqueue(F, &dado3);
    TEST_ASSERT_TRUE_MESSAGE(ok, "Deve conseguir inserir apos clear.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado3, peek(F), "peek deve retornar dado3 apos clear e nova insercao.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthFila(F), "Tamanho deve ser 1 apos clear e uma insercao.");
}

/* ─────────────────────────────────────────────
   destroiFila
   ───────────────────────────────────────────── */

void test_destroiFila_ComCallback(void)
{
    Fila tempF = criaFila(CAPACIDADE);
    enqueue(tempF, &dado1);
    enqueue(tempF, &dado2);
    enqueue(tempF, &dado3);

    destruicoes = 0;
    destroiFila(tempF, contaDestruicao);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, destruicoes, "O callback deve ser chamado para cada item ao destruir.");
}

void test_destroiFila_SemCallback(void)
{
    Fila tempF = criaFila(CAPACIDADE);
    enqueue(tempF, &dado1);

    // Nao deve crashar
    destroiFila(tempF, NULL);
    TEST_ASSERT_TRUE(true);
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criaFila);
    RUN_TEST(test_criaFila_CapacidadeInvalida);

    RUN_TEST(test_enqueue_FilaVazia);
    RUN_TEST(test_enqueue_MultiplosElementos);
    RUN_TEST(test_enqueue_FilaCheia);
    RUN_TEST(test_enqueue_FilaNula);

    RUN_TEST(test_dequeue_UnicoElemento);
    RUN_TEST(test_dequeue_OrdemFIFO);
    RUN_TEST(test_dequeue_FilaVazia);
    RUN_TEST(test_dequeue_FilaNula);

    RUN_TEST(test_Circular_EnqueueDequeueIntercalados);
    RUN_TEST(test_Circular_EncheEsvaziaMuitasVezes);

    RUN_TEST(test_peek_NaoRemove);
    RUN_TEST(test_peek_FilaNula);

    RUN_TEST(test_isEmptyFila);
    RUN_TEST(test_isFullFila);
    RUN_TEST(test_isEmptyFila_Nula);
    RUN_TEST(test_isFullFila_Nula);

    RUN_TEST(test_lengthFila);
    RUN_TEST(test_lengthFila_Nula);
    RUN_TEST(test_capacidadeFila);
    RUN_TEST(test_capacidadeFila_Nula);

    RUN_TEST(test_clearFila_ComCallback);
    RUN_TEST(test_clearFila_SemCallback);
    RUN_TEST(test_clearFila_ReutilizacaoAposClear);

    RUN_TEST(test_destroiFila_ComCallback);
    RUN_TEST(test_destroiFila_SemCallback);

    return UNITY_END();
}