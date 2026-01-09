#include "arvore_tipo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cria nó auxiliar
NoArvoreTipo* criar_no_tipo(struct tipo_de_pet* t) {
    NoArvoreTipo* novo = (NoArvoreTipo*) malloc(sizeof(NoArvoreTipo));
    if (novo) {
        novo->dado = t;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

// Insere na árvore (Recursivo)
NoArvoreTipo* inserir_tipo_por_nome(NoArvoreTipo* raiz, struct tipo_de_pet* t) {
    if (raiz == NULL) return criar_no_tipo(t);

    if (strcmp(t->nome, raiz->dado->nome) < 0) {
        raiz->esq = inserir_tipo_por_nome(raiz->esq, t);
    } else {
        raiz->dir = inserir_tipo_por_nome(raiz->dir, t);
    }
    return raiz;
}

// Imprime Em-Ordem (A-Z)
void imprimir_tipo_em_ordem(NoArvoreTipo* raiz) {
    if (raiz != NULL) {
        imprimir_tipo_em_ordem(raiz->esq);
        printf("ID: %-4d | Tipo: %s\n", raiz->dado->codigo, raiz->dado->nome);
        imprimir_tipo_em_ordem(raiz->dir);
    }
}

// Limpa memória da árvore
void liberar_arvore_tipo(NoArvoreTipo* raiz) {
    if (raiz != NULL) {
        liberar_arvore_tipo(raiz->esq);
        liberar_arvore_tipo(raiz->dir);
        free(raiz);
    }
}

// --- FUNÇÃO PRINCIPAL ---
void exibir_tipos_ordenado_por_nome(struct tipo_de_pet* lista_inicio) {
    if (lista_inicio == NULL) {
        printf("Nenhum tipo para ordenar.\n");
        return;
    }

    NoArvoreTipo* raiz = NULL;
    struct tipo_de_pet* atual = lista_inicio;

    // 1. Popula a árvore
    while (atual != NULL) {
        raiz = inserir_tipo_por_nome(raiz, atual);
        atual = atual->prox;
    }

    // 2. Exibe
    printf("\n--- TIPOS DE PET (ORDEM ALFABETICA) ---\n");
    imprimir_tipo_em_ordem(raiz);
    printf("---------------------------------------\n");

    // 3. Limpa
    liberar_arvore_tipo(raiz);
}