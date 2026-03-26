#include "unity.h"
#include "../src/lista.h"

Lista L;
int dado1 = 10, dado2 = 20, dado3 = 30;

// O Unity corre esta função ANTES de cada teste individual
void setUp(void)
{
    L = criaLista();
}

// O Unity corre esta função DEPOIS de cada teste individual
void tearDown(void)
{
    // Limpa a lista para não haver fugas de memória entre os testes
    destroiLista(L, NULL);
}

void test_criaLista(void)
{
    // A função setUp() já chamou L = criaLista() antes deste teste começar.

    // 1. Verifica se a memória foi alocada (ponteiro não é nulo)
    TEST_ASSERT_NOT_NULL_MESSAGE(L, "A lista criada não deve ser NULL.");

    // 2. Verifica as propriedades de uma lista vazia
    TEST_ASSERT_TRUE_MESSAGE(isEmptyLista(L), "A lista recém-criada deve estar vazia (isEmptyLista == true).");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthLista(L), "O tamanho inicial da lista deve ser exatamente 0.");

    // 3. Verifica se os ponteiros de controle não têm "lixo" de memória
    TEST_ASSERT_NULL_MESSAGE(getFirst(L), "O getFirst de uma lista vazia deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(getLast(L), "O getLast de uma lista vazia deve retornar NULL.");
}

void test_insereFinalLista_ListaVazia(void)
{

    Posic p1 = insereFinalLista(L, &dado1);

    TEST_ASSERT_NOT_NULL_MESSAGE(p1, "A posicao retornada nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthLista(L), "O tamanho da lista deve ser 1 após a insercao.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getFirst(L), "O unico no deve ser o primeiro.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getLast(L), "O unico no deve ser o ultimo.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, getLista(L, p1), "O item armazenado deve ser o mesmo passado.");
}

void test_insereFinalLista_MultiplosElementos(void)
{
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);
    Posic p3 = insereFinalLista(L, &dado3);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, lengthLista(L), "O tamanho da lista deve ser 3.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getFirst(L), "O primeiro elemento inserido deve continuar sendo o getFirst.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p3, getLast(L), "O ultimo elemento inserido deve ser o getLast.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getNext(L, p1), "O proximo depois de p1 deve ser p2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p3, getNext(L, p2), "O proximo depois de p2 deve ser p3.");
    TEST_ASSERT_NULL_MESSAGE(getNext(L, p3), "O proximo depois do ultimo deve ser NULL.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getPrevious(L, p3), "O anterior a p3 deve ser p2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getPrevious(L, p2), "O anterior a p2 deve ser p1.");
    TEST_ASSERT_NULL_MESSAGE(getPrevious(L, p1), "O anterior ao primeiro deve ser NULL.");
}

void test_insereFinalLista_ListaNula(void)
{
    Posic p = insereFinalLista(NULL, &dado1);
    TEST_ASSERT_NULL_MESSAGE(p, "Inserir em uma lista NULL deve retornar NULL com seguranca.");
}

void test_insereInicioLista_ListaVazia(void)
{
    Posic p1 = insereInicioLista(L, &dado1);

    TEST_ASSERT_NOT_NULL_MESSAGE(p1, "A posicao retornada nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthLista(L), "O tamanho da lista deve ser 1.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getFirst(L), "O unico no deve ser o primeiro.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getLast(L), "O unico no deve ser o ultimo.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, getLista(L, p1), "O dado armazenado deve ser o dado1.");
}

void test_insereInicioLista_MultiplosElementos(void)
{
    // Inserindo na ordem: dado1, depois dado2, depois dado3.
    // Como é no INÍCIO, a lista final deve ficar: p3(dado3) -> p2(dado2) -> p1(dado1)
    Posic p1 = insereInicioLista(L, &dado1);
    Posic p2 = insereInicioLista(L, &dado2);
    Posic p3 = insereInicioLista(L, &dado3);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, lengthLista(L), "O tamanho da lista deve ser 3.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p3, getFirst(L), "O ULTIMO inserido no inicio deve ser o getFirst.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getLast(L), "O PRIMEIRO inserido no inicio deve ser o getLast.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getNext(L, p3), "O proximo depois do p3 deve ser o p2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getNext(L, p2), "O proximo depois do p2 deve ser o p1.");
    TEST_ASSERT_NULL_MESSAGE(getNext(L, p1), "O proximo depois do ultimo (p1) deve ser NULL.");

    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getPrevious(L, p1), "O anterior ao p1 deve ser o p2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p3, getPrevious(L, p2), "O anterior ao p2 deve ser o p3.");
    TEST_ASSERT_NULL_MESSAGE(getPrevious(L, p3), "O anterior ao primeiro (p3) deve ser NULL.");
}

void test_insereInicioLista_ListaNula(void)
{
    Posic p = insereInicioLista(NULL, &dado1);
    TEST_ASSERT_NULL_MESSAGE(p, "Inserir no inicio de uma lista NULL deve retornar NULL.");
}

void test_insertBefore_PrimeiroElemento(void)
{
    Posic p1 = insereFinalLista(L, &dado1);

    // Insere dado2 antes de p1 (que é o primeiro)
    Posic p_new = insertBefore(L, p1, &dado2);

    TEST_ASSERT_NOT_NULL_MESSAGE(p_new, "A posicao retornada nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, lengthLista(L), "O tamanho da lista deve ser 2.");

    // O novo nó deve ter se tornado o primeiro da lista
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p_new, getFirst(L), "O novo no deve ser o primeiro.");

    // Testa as ligações entre p_new e p1
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getNext(L, p_new), "O proximo do novo no deve ser p1.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p_new, getPrevious(L, p1), "O anterior de p1 deve ser o novo no.");
    TEST_ASSERT_NULL_MESSAGE(getPrevious(L, p_new), "O anterior do primeiro (p_new) deve ser NULL.");
}

void test_insertBefore_ElementoMeio(void)
{
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p3 = insereFinalLista(L, &dado3);

    // Insere dado2 antes de p3 (vai ficar no meio: p1 -> p2 -> p3)
    Posic p2 = insertBefore(L, p3, &dado2);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, lengthLista(L), "O tamanho da lista deve ser 3.");

    // Testa as ligações do lado esquerdo (p1 <-> p2)
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getNext(L, p1), "O proximo de p1 deve ser p2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getPrevious(L, p2), "O anterior de p2 deve ser p1.");

    // Testa as ligações do lado direito (p2 <-> p3)
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p3, getNext(L, p2), "O proximo de p2 deve ser p3.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getPrevious(L, p3), "O anterior de p3 deve ser p2.");
}

void test_insertBefore_Defensivo(void)
{
    // Tenta inserir em lista NULL
    Posic p_lista_nula = insertBefore(NULL, NULL, &dado1);
    TEST_ASSERT_NULL_MESSAGE(p_lista_nula, "Inserir em lista NULL deve retornar NULL.");

    // Tenta inserir com posição de referência NULL em lista válida
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p_pos_nula = insertBefore(L, NULL, &dado2);
    TEST_ASSERT_NULL_MESSAGE(p_pos_nula, "Inserir com posicao de referencia NULL deve retornar NULL.");
}

void test_insertAfter_UltimoElemento(void)
{
    Posic p1 = insereFinalLista(L, &dado1);

    // Insere dado2 depois de p1 (que é o último)
    Posic p_new = insertAfter(L, p1, &dado2);

    TEST_ASSERT_NOT_NULL_MESSAGE(p_new, "A posicao retornada nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, lengthLista(L), "O tamanho da lista deve ser 2.");

    // O novo nó deve ter se tornado o último da lista
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p_new, getLast(L), "O novo no deve ser o ultimo.");

    // Testa as ligações entre p1 e p_new
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p_new, getNext(L, p1), "O proximo de p1 deve ser o novo no.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getPrevious(L, p_new), "O anterior do novo no deve ser p1.");
    TEST_ASSERT_NULL_MESSAGE(getNext(L, p_new), "O proximo do ultimo (p_new) deve ser NULL.");
}

void test_insertAfter_ElementoMeio(void)
{
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p3 = insereFinalLista(L, &dado3);

    // Insere dado2 depois de p1 (vai ficar no meio: p1 -> p2 -> p3)
    Posic p2 = insertAfter(L, p1, &dado2);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, lengthLista(L), "O tamanho da lista deve ser 3.");

    // Testa as ligações do lado esquerdo (p1 <-> p2)
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getNext(L, p1), "O proximo de p1 deve ser p2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getPrevious(L, p2), "O anterior de p2 deve ser p1.");

    // Testa as ligações do lado direito (p2 <-> p3)
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p3, getNext(L, p2), "O proximo de p2 deve ser p3.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getPrevious(L, p3), "O anterior de p3 deve ser p2.");
}

void test_insertAfter_Defensivo(void)
{
    // Tenta inserir em lista NULL
    Posic p_lista_nula = insertAfter(NULL, NULL, &dado1);
    TEST_ASSERT_NULL_MESSAGE(p_lista_nula, "Inserir em lista NULL deve retornar NULL.");

    // Tenta inserir com posição de referência NULL
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p_pos_nula = insertAfter(L, NULL, &dado2);
    TEST_ASSERT_NULL_MESSAGE(p_pos_nula, "Inserir com posicao de referencia NULL deve retornar NULL.");
}

void test_removeLista_UnicoElemento(void)
{
    Posic p1 = insereFinalLista(L, &dado1);

    // Removendo o único nó da lista
    Item removido = removeLista(L, p1);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, removido, "O item retornado deve ser o dado1.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, lengthLista(L), "O tamanho da lista deve ser 0.");
    TEST_ASSERT_TRUE_MESSAGE(isEmptyLista(L), "A lista deve estar vazia.");
    TEST_ASSERT_NULL_MESSAGE(getFirst(L), "getFirst deve ser NULL.");
    TEST_ASSERT_NULL_MESSAGE(getLast(L), "getLast deve ser NULL.");
}

void test_removeLista_PrimeiroDeVarios(void)
{
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);

    Item removido = removeLista(L, p1);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, removido, "Deve retornar o dado1.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, lengthLista(L), "Tamanho deve ser 1.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, getFirst(L), "O novo primeiro deve ser o p2.");
    TEST_ASSERT_NULL_MESSAGE(getPrevious(L, p2), "O anterior do novo primeiro deve ser NULL.");
}

void test_removeLista_UltimoDeVarios(void)
{
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);

    Item removido = removeLista(L, p2);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado2, removido, "Deve retornar o dado2.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getLast(L), "O novo ultimo deve ser o p1.");
    TEST_ASSERT_NULL_MESSAGE(getNext(L, p1), "O proximo do novo ultimo deve ser NULL.");
}

void test_removeLista_Meio(void)
{
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);
    Posic p3 = insereFinalLista(L, &dado3);

    Item removido = removeLista(L, p2);

    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado2, removido, "Deve retornar o dado2.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, lengthLista(L), "Tamanho deve ser 2.");

    // Verifica a nova ponte entre p1 e p3
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p3, getNext(L, p1), "O proximo de p1 deve saltar para p3.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, getPrevious(L, p3), "O anterior de p3 deve saltar para p1.");
}

void test_removeLista_Defensivo(void)
{
    TEST_ASSERT_NULL_MESSAGE(removeLista(NULL, NULL), "Remover de lista NULL deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(removeLista(L, NULL), "Remover posicao NULL deve retornar NULL.");
}

void test_MetodosDeAcesso_e_Navegacao(void)
{
    // Teste com lista vazia
    TEST_ASSERT_EQUAL_INT(0, lengthLista(L));
    TEST_ASSERT_NULL(getFirst(L));
    TEST_ASSERT_NULL(getLast(L));

    // Montando lista: p1(10) -> p2(20) -> p3(30)
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);
    Posic p3 = insereFinalLista(L, &dado3);

    // Teste lengthLista
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, lengthLista(L), "Tamanho deve ser 3.");

    // Teste getLista
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado1, getLista(L, p1), "getLista(p1) deve ser 10.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado2, getLista(L, p2), "getLista(p2) deve ser 20.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&dado3, getLista(L, p3), "getLista(p3) deve ser 30.");

    // Teste getFirst e getLast
    TEST_ASSERT_EQUAL_PTR(p1, getFirst(L));
    TEST_ASSERT_EQUAL_PTR(p3, getLast(L));

    // Teste getNext
    TEST_ASSERT_EQUAL_PTR(p2, getNext(L, p1));
    TEST_ASSERT_EQUAL_PTR(p3, getNext(L, p2));
    TEST_ASSERT_NULL(getNext(L, p3));

    // Teste getPrevious
    TEST_ASSERT_EQUAL_PTR(p2, getPrevious(L, p3));
    TEST_ASSERT_EQUAL_PTR(p1, getPrevious(L, p2));
    TEST_ASSERT_NULL(getPrevious(L, p1));
}

void test_MetodosDeAcesso_Defensivo(void)
{
    TEST_ASSERT_EQUAL_INT(0, lengthLista(NULL));
    TEST_ASSERT_NULL(getLista(NULL, NULL));
    TEST_ASSERT_NULL(getNext(NULL, NULL));
    TEST_ASSERT_NULL(getPrevious(L, NULL));
}

int destruicoes = 0;

// Função de callback para contar quantas vezes o item foi liberado
void contaDestruicao(Item info)
{
    destruicoes++;
}

void test_destroiLista_ComCallback(void)
{
    Lista tempL = criaLista();
    insereFinalLista(tempL, &dado1);
    insereFinalLista(tempL, &dado2);
    insereFinalLista(tempL, &dado3);

    destruicoes = 0;
    // Chama a destruição passando o callback
    destroiLista(tempL, contaDestruicao);

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, destruicoes, "O callback de destruicao deve ser chamado para cada item da lista.");
}

void test_destroiLista_SemCallback(void)
{
    Lista tempL = criaLista();
    insereFinalLista(tempL, &dado1);

    // Não deve crashar ao passar NULL
    destroiLista(tempL, NULL);
    TEST_ASSERT_TRUE(true);
}

void test_isEmptyLista(void)
{
    TEST_ASSERT_TRUE(isEmptyLista(L));
    insereFinalLista(L, &dado1);
    TEST_ASSERT_FALSE(isEmptyLista(L));
}

void test_buscaLista(void)
{
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);

    // Busca item existente
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p1, buscaLista(L, &dado1), "Deve encontrar a posicao do dado1.");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(p2, buscaLista(L, &dado2), "Deve encontrar a posicao do dado2.");

    // Busca item inexistente
    TEST_ASSERT_NULL_MESSAGE(buscaLista(L, &dado3), "Nao deve encontrar um item que nao esta na lista.");

    // Busca em lista vazia ou NULL
    Lista vazia = criaLista();
    TEST_ASSERT_NULL(buscaLista(vazia, &dado1));
    TEST_ASSERT_NULL(buscaLista(NULL, &dado1));
    destroiLista(vazia, NULL);
}

void test_Navegacao_e_Acesso(void)
{
    // Montando: p1(10) <-> p2(20) <-> p3(30)
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);
    Posic p3 = insereFinalLista(L, &dado3);

    // Teste getLista
    TEST_ASSERT_EQUAL_PTR(&dado1, getLista(L, p1));
    TEST_ASSERT_EQUAL_PTR(&dado2, getLista(L, p2));
    TEST_ASSERT_EQUAL_PTR(&dado3, getLista(L, p3));

    // Teste extremos
    TEST_ASSERT_EQUAL_PTR(p1, getFirst(L));
    TEST_ASSERT_EQUAL_PTR(p3, getLast(L));

    // Teste encadeamento para frente
    TEST_ASSERT_EQUAL_PTR(p2, getNext(L, p1));
    TEST_ASSERT_EQUAL_PTR(p3, getNext(L, p2));
    TEST_ASSERT_NULL(getNext(L, p3));

    // Teste encadeamento para trás
    TEST_ASSERT_EQUAL_PTR(p2, getPrevious(L, p3));
    TEST_ASSERT_EQUAL_PTR(p1, getPrevious(L, p2));
    TEST_ASSERT_NULL(getPrevious(L, p1));
}

void test_Acesso_Invalido(void)
{
    // Casos com NULL
    TEST_ASSERT_NULL(getLista(NULL, NULL));
    TEST_ASSERT_NULL(getFirst(NULL));
    TEST_ASSERT_NULL(getLast(NULL));
    TEST_ASSERT_NULL(getNext(L, NULL));
    TEST_ASSERT_NULL(getPrevious(L, NULL));
}

void test_Consistencia_Remocao_Insercao_Mista(void)
{
    // 1. Insere p1, p2, p3
    Posic p1 = insereFinalLista(L, &dado1);
    Posic p2 = insereFinalLista(L, &dado2);
    Posic p3 = insereFinalLista(L, &dado3);

    // 2. Remove o do meio (p2)
    removeLista(L, p2);
    TEST_ASSERT_EQUAL_INT(2, lengthLista(L));

    // 3. Insere um novo no início
    int novo_dado = 100;
    Posic p_new = insereInicioLista(L, &novo_dado);

    // Estado esperado: p_new(100) <-> p1(10) <-> p3(30)
    TEST_ASSERT_EQUAL_INT(3, lengthLista(L));
    TEST_ASSERT_EQUAL_PTR(p_new, getFirst(L));
    TEST_ASSERT_EQUAL_PTR(p1, getNext(L, p_new));
    TEST_ASSERT_EQUAL_PTR(p3, getNext(L, p1));
    TEST_ASSERT_NULL(getNext(L, p3));

    // 4. Esvazia a lista um por um
    removeLista(L, p1);
    removeLista(L, p3);
    removeLista(L, p_new);

    TEST_ASSERT_TRUE(isEmptyLista(L));
    TEST_ASSERT_EQUAL_INT(0, lengthLista(L));
    TEST_ASSERT_NULL(getFirst(L));
    TEST_ASSERT_NULL(getLast(L));
}

void test_GetLista_Alteracao_Item(void)
{
    Posic p1 = insereFinalLista(L, &dado1);

    // Verifica se conseguimos acessar e se o ponteiro é o mesmo
    int *recuperado = (int *)getLista(L, p1);
    TEST_ASSERT_EQUAL_INT(10, *recuperado);

    // Simula alteração do dado externo
    *recuperado = 99;
    TEST_ASSERT_EQUAL_INT(99, *(int *)getLista(L, p1));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criaLista);

    RUN_TEST(test_insereFinalLista_ListaVazia);
    RUN_TEST(test_insereFinalLista_MultiplosElementos);
    RUN_TEST(test_insereFinalLista_ListaNula);

    RUN_TEST(test_insereInicioLista_ListaVazia);
    RUN_TEST(test_insereInicioLista_MultiplosElementos);
    RUN_TEST(test_insereInicioLista_ListaNula);

    RUN_TEST(test_insertBefore_PrimeiroElemento);
    RUN_TEST(test_insertBefore_ElementoMeio);
    RUN_TEST(test_insertBefore_Defensivo);

    RUN_TEST(test_insertAfter_UltimoElemento);
    RUN_TEST(test_insertAfter_ElementoMeio);
    RUN_TEST(test_insertAfter_Defensivo);

    RUN_TEST(test_removeLista_UnicoElemento);
    RUN_TEST(test_removeLista_PrimeiroDeVarios);
    RUN_TEST(test_removeLista_UltimoDeVarios);
    RUN_TEST(test_removeLista_Meio);
    RUN_TEST(test_removeLista_Defensivo);

    RUN_TEST(test_MetodosDeAcesso_e_Navegacao);
    RUN_TEST(test_MetodosDeAcesso_Defensivo);

    RUN_TEST(test_destroiLista_ComCallback);
    RUN_TEST(test_destroiLista_SemCallback);

    RUN_TEST(test_isEmptyLista);

    RUN_TEST(test_buscaLista);

    RUN_TEST(test_Navegacao_e_Acesso);
    RUN_TEST(test_Acesso_Invalido);

    RUN_TEST(test_Consistencia_Remocao_Insercao_Mista);
    RUN_TEST(test_GetLista_Alteracao_Item);

    return UNITY_END();
}