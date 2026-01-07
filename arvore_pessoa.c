#include "arvore_pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para criar um novo nó da árvore
NoArvore* criar_no_arvore(Pessoa* p) {
    NoArvore* novo = (NoArvore*) malloc(sizeof(NoArvore));
    if (novo != NULL) {
        novo->dado = p; // Apenas aponta para o dado existente na lista
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

// Insere na árvore usando o NOME como critério (strcmp)
NoArvore* inserir_por_nome(NoArvore* raiz, Pessoa* p) {
    if (raiz == NULL) {
        return criar_no_arvore(p);
    }

    // Compara o nome da pessoa 'p' com o nome do nó atual 'raiz->dado'
    // Se p->nome < raiz->dado->nome, vai para a esquerda
    if (strcmp(p->nome, raiz->dado->nome) < 0) {
        raiz->esq = inserir_por_nome(raiz->esq, p);
    } 
    // Caso contrário (maior ou igual), vai para a direita
    else {
        raiz->dir = inserir_por_nome(raiz->dir, p);
    }

    return raiz;
}

// Percurso Em-Ordem (Esquerda -> Raiz -> Direita) para imprimir alfabeticamente
void imprimir_em_ordem(NoArvore* raiz) {
    if (raiz != NULL) {
        imprimir_em_ordem(raiz->esq);
        
        // Imprime os dados da pessoa apontada por este nó
        printf("COD: %d | NOME: %s | FONE: %s | NASC: %s | END: %s\n", 
               raiz->dado->codigo, 
               raiz->dado->nome, 
               raiz->dado->fone, 
               raiz->dado->data_nascimento, 
               raiz->dado->endereco);
               
        imprimir_em_ordem(raiz->dir);
    }
}

// Libera apenas os nós da árvore, SEM apagar as Pessoas (pois elas pertencem à Lista)
void liberar_arvore(NoArvore* raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz); // Libera o nó da estrutura de árvore
    }
}

// --- FUNÇÃO PRINCIPAL DE INTEGRAÇÃO ---
// Esta é a função que você chamará quando encontrar o comando "order by nome"
void exibir_pessoas_ordenado_por_nome(Pessoa* lista_inicio) {
    if (lista_inicio == NULL) {
        printf("Lista vazia. Nada a ordenar.\n");
        return;
    }

    NoArvore* raiz = NULL;
    Pessoa* atual = lista_inicio;

    // 1. Percorre a lista duplamente encadeada e popula a árvore
    while (atual != NULL) {
        raiz = inserir_por_nome(raiz, atual);
        atual = atual->prox;
    }

    // 2. Imprime o relatório ordenado
    printf("--- RELATORIO DE PESSOAS (ORDENADO POR NOME) ---\n");
    imprimir_em_ordem(raiz);
    printf("------------------------------------------------\n");

    // 3. Limpa a árvore temporária da memória
    liberar_arvore(raiz);
}