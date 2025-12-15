#include <stdio.h>
#include <stdlib.h>
#include "pessoa.h"

// Função auxiliar apenas para visualizar a lista no teste
void imprimir_lista(Pessoa* inicio) {
    printf("\n=== ESTADO ATUAL DA LISTA ===\n");
    Pessoa* p = inicio;
    if (p == NULL) {
        printf("[ Lista Vazia ]\n");
    }
    while (p != NULL) {
        printf("-> ID: %d | Nome: %s | Fone: %s\n", p->codigo, p->nome, p->fone);
        p = p->prox;
    }
    printf("=============================\n\n");
}

// Função para criar um txt fake só para o teste não falhar
void criar_arquivo_teste() {
    FILE* f = fopen("pessoas.txt", "w");
    if (f) {
        fprintf(f, "100;Carlos Arquivo;859999;01/01/1980;Rua Arquivo, 10\n");
        fprintf(f, "101;Ana Arquivo;858888;02/02/1990;Av Arquivo, 20\n");
        // Vou colocar um duplicado de proposito no arquivo para testar sua validação
        fprintf(f, "100;Carlos Clone;000000;00/00/0000;Rua Fake\n");
        fclose(f);
        printf("Arquivo 'pessoas.txt' criado para teste.\n");
    }
}

int main() {
    // 0. Preparação
    criar_arquivo_teste();
    Pessoa* lista = criar_lista_pessoa();
    int resultado;

    // --- TESTE 1: CARREGAR ARQUIVO ---
    printf("--- TESTE 1: Carregando do Arquivo ---\n");
    // Deve carregar Carlos e Ana, e avisar que o Clone é duplicado
    carregar_pessoas_arquivo(&lista, "pessoas.txt");
    imprimir_lista(lista);

    // --- TESTE 2: INSERÇÃO MANUAL (SUCESSO) ---
    printf("--- TESTE 2: Insercao Manual Valida ---\n");
    resultado = inserir_pessoa(&lista, 10, "Joao Manual", "999-999", "10/10/2000", "Rua Manual");
    if (resultado == 1) printf("Sucesso: Joao inserido.\n");
    else printf("Falha ao inserir Joao.\n");

    // --- TESTE 3: INSERÇÃO DUPLICADA (ERRO) ---
    printf("--- TESTE 3: Validacao de Duplicidade ---\n");
    // Tentando inserir ID 10 de novo
    resultado = inserir_pessoa(&lista, 10, "Joao Clone", "000", "00/00", "Rua X");
    if (resultado == -1) printf("Sucesso no Teste: O sistema bloqueou o ID duplicado.\n");
    else printf("Falha: O sistema permitiu duplicidade!\n");

    // --- TESTE 4: INSERÇÃO SEM NOME (ERRO) ---
    printf("--- TESTE 4: Validacao de Nome Vazio ---\n");
    resultado = inserir_pessoa(&lista, 50, "", "000", "00/00", "Rua Y");
    if (resultado == 0) printf("Sucesso no Teste: O sistema bloqueou nome vazio.\n");
    else printf("Falha: O sistema aceitou nome vazio!\n");

    imprimir_lista(lista);

    // --- TESTE 5: ALTERAÇÃO ---
    printf("--- TESTE 5: Alteracao de Dados ---\n");
    // Vamos mudar o nome do ID 100 (Carlos Arquivo)
    printf("Alterando ID 100...\n");
    resultado = alterar_pessoa(lista, 100, "Carlos ALTERADO", "000-000", "01/01/1980", "Nova Casa");
    if (resultado == 1) printf("Alterado com sucesso.\n");
    
    // Tentando alterar alguém que não existe
    printf("Tentando alterar ID 999 (Inexistente)...\n");
    alterar_pessoa(lista, 999, "Fantasma", "000", "00", "00");

    imprimir_lista(lista);

    // --- TESTE 6: EXCLUSÃO ---
    printf("--- TESTE 6: Exclusao ---\n");
    // Removendo Joao (ID 10)
    printf("Removendo ID 10...\n");
    if (excluir_pessoa(&lista, 10)) printf("ID 10 removido.\n");

    // Removendo o primeiro da lista (teste de ponteiro inicio)
    // Se a inserção foi no início, o último inserido (ID 100 ou 101) está na ponta
    printf("Removendo ID 101...\n");
    if (excluir_pessoa(&lista, 101)) printf("ID 101 removido.\n");

    imprimir_lista(lista);

    // Limpeza final (opcional para o teste, mas boa prática)
    // Aqui você faria um loop de free, mas o SO limpa ao fechar.
    
    return 0;
}