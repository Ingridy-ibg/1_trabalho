#include "unity.h"
#include "../src/processadorQRY.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Substitua pelo include real da sua estrutura, se necessário
#include "../src/formas.h" 

// Variáveis globais para os testes
Formas bancoFormas;
FILE *arq_svg;
FILE *arq_txt;
const char *NOME_ARQ_QRY = "temp_test.qry";
const char *NOME_ARQ_SVG = "temp_test.svg";
const char *NOME_ARQ_TXT = "temp_test.txt";

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

// Cria um arquivo .qry temporário com o conteúdo desejado
static void criarArquivoQry(const char* conteudo) {
    FILE* f = fopen(NOME_ARQ_QRY, "w");
    if (f) {
        fprintf(f, "%s", conteudo);
        fclose(f);
    }
}

// Verifica se uma determinada string existe dentro de um arquivo gerado
static bool arquivoContemString(const char* caminho_arquivo, const char* texto_esperado) {
    FILE* f = fopen(caminho_arquivo, "r");
    if (!f) return false;

    // Descobre o tamanho do arquivo
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (tamanho <= 0) {
        fclose(f);
        return false;
    }

    // Lê o arquivo todo para a memória
    char* buffer = (char*)malloc(tamanho + 1);
    fread(buffer, 1, tamanho, f);
    buffer[tamanho] = '\0';
    fclose(f);

    // Verifica se o texto existe no buffer
    bool contem = (strstr(buffer, texto_esperado) != NULL);
    free(buffer);
    
    return contem;
}

// ============================================================================
// SETUP E TEARDOWN (Executados antes e depois de CADA teste)
// ============================================================================

void setUp(void) {
    // Inicializa o banco (substitua pela sua função real de inicialização)
    bancoFormas = criaFormas(); 
    
    // Abre os arquivos no modo leitura/escrita para atualização ("w+")
    arq_svg = fopen(NOME_ARQ_SVG, "w+");
    arq_txt = fopen(NOME_ARQ_TXT, "w+");
}

void tearDown(void) {
    // Libera a memória do banco de dados
    if (bancoFormas) {
        destroiFormas(bancoFormas); 
    }
    
    if (arq_svg) fclose(arq_svg);
    if (arq_txt) fclose(arq_txt);
    
    // Remove os arquivos temporários para não sujar o diretório
    remove(NOME_ARQ_QRY);
    remove(NOME_ARQ_SVG);
    remove(NOME_ARQ_TXT);
}

// ============================================================================
// CASOS DE TESTE
// ============================================================================

void test_ProcessaQry_DeveRetornarFalsoParaArquivoInexistente(void) {
    // Tenta ler um arquivo que não foi criado
    bool resultado = processaQry("arquivo_que_nao_existe.qry", bancoFormas, arq_svg, arq_txt);
    TEST_ASSERT_FALSE_MESSAGE(resultado, "A função deve retornar false se não conseguir abrir o arquivo .qry.");
}

void test_ProcessaQry_DeveIgnorarComentariosELinhasEmBranco(void) {
    // Monta um arquivo só com lixo ignorável
    criarArquivoQry("\n\n# Isto é um comentário\n\n# Outro comentário\n");
    
    bool resultado = processaQry(NOME_ARQ_QRY, bancoFormas, arq_svg, arq_txt);
    
    TEST_ASSERT_TRUE_MESSAGE(resultado, "Deve processar o arquivo com sucesso, ignorando o lixo.");
    
    // Verifica se os arquivos de saída continuam vazios (tamanho 0)
    fseek(arq_txt, 0, SEEK_END);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ftell(arq_txt), "O arquivo TXT deveria estar vazio.");
}

void test_ProcessaQry_ComandosDePoligono_inp_rmp_clp(void) {
    // Testa a inserção, remoção e limpeza de coordenadas do polígono
    // Presume-se que o comando escreva um log no TXT (ajuste o texto_esperado conforme sua implementação real)
    criarArquivoQry("inp 1 100\nrmp 1\nclp 1\n");
    
    bool resultado = processaQry(NOME_ARQ_QRY, bancoFormas, arq_svg, arq_txt);
    TEST_ASSERT_TRUE(resultado);
    
    // Obs: Se seus comandos inp/rmp/clp geram log no txt, você pode testar assim:
    // fflush(arq_txt); // Garante que a escrita foi pro disco
    // TEST_ASSERT_TRUE_MESSAGE(arquivoContemString(NOME_ARQ_TXT, "inp 1 100"), "TXT deve registrar o comando inp.");
}

void test_ProcessaQry_ComandoPol_DeveGerarSaidaSvgETxt(void) {
    // Testa o comando 'pol p i d corb corp'
    criarArquivoQry("pol 1 10 5.5 red blue\n");
    
    bool resultado = processaQry(NOME_ARQ_QRY, bancoFormas, arq_svg, arq_txt);
    TEST_ASSERT_TRUE(resultado);
    
    // Força a gravação dos buffers nos arquivos temporários
    fflush(arq_svg);
    fflush(arq_txt);
    
    // Verifica se as cores foram enviadas para o SVG/TXT (ajuste conforme o formato real do seu SVG)
    TEST_ASSERT_TRUE_MESSAGE(arquivoContemString(NOME_ARQ_SVG, "red"), "A cor de borda (red) não foi encontrada no SVG.");
    TEST_ASSERT_TRUE_MESSAGE(arquivoContemString(NOME_ARQ_SVG, "blue"), "A cor de preenchimento (blue) não foi encontrada no SVG.");
}

void test_ProcessaQry_ComandosDeSelecao_sel_dels_mcs(void) {
    // Testa o fluxo de seleção, translação e deleção
    criarArquivoQry("sel 10 10 100 100\nmcs 5 5 green yellow\ndels\n");
    
    bool resultado = processaQry(NOME_ARQ_QRY, bancoFormas, arq_svg, arq_txt);
    TEST_ASSERT_TRUE(resultado);
    
    fflush(arq_txt);
    
    // Se o comando 'sel' ou 'dels' gerar um relatório no TXT dizendo quais itens foram afetados, teste isso.
    // Exemplo: TEST_ASSERT_TRUE(arquivoContemString(NOME_ARQ_TXT, "removido"));
}

void test_ProcessaQry_ArquivoMistoCompleto(void) {
    // Testa o parser contra um arquivo grande com vários casos simulados
    const char* cenario_complexo = 
        "# Inicializando ancoras\n"
        "inp 1 10\n"
        "inp 1 20\n"
        "\n" // Linha em branco
        "pol 1 5 2.0 black white\n"
        "sel 0 0 500 500\n"
        "mcs 10 10 cyan magenta\n"
        "dels\n"
        "clp 1\n";
        
    criarArquivoQry(cenario_complexo);
    
    bool resultado = processaQry(NOME_ARQ_QRY, bancoFormas, arq_svg, arq_txt);
    TEST_ASSERT_TRUE_MESSAGE(resultado, "Deve processar o arquivo completo sem falhas críticas.");
}

// ============================================================================
// MAIN RUNNER
// ============================================================================

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_ProcessaQry_DeveRetornarFalsoParaArquivoInexistente);
    RUN_TEST(test_ProcessaQry_DeveIgnorarComentariosELinhasEmBranco);
    RUN_TEST(test_ProcessaQry_ComandosDePoligono_inp_rmp_clp);
    RUN_TEST(test_ProcessaQry_ComandoPol_DeveGerarSaidaSvgETxt);
    RUN_TEST(test_ProcessaQry_ComandosDeSelecao_sel_dels_mcs);
    RUN_TEST(test_ProcessaQry_ArquivoMistoCompleto);
    
    return UNITY_END();
}