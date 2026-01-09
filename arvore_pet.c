#include "arvore_pet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NoArvorePet* criar_no_pet(struct pet* p) {
    NoArvorePet* novo = (NoArvorePet*) malloc(sizeof(NoArvorePet));
    if (novo != NULL) {
        novo->dado = p;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

NoArvorePet* inserir_pet_por_nome(NoArvorePet* raiz, struct pet* p) {
    if (raiz == NULL) {
        return criar_no_pet(p);
    }

    if (strcmp(p->nome, raiz->dado->nome) < 0) {
        raiz->esq = inserir_pet_por_nome(raiz->esq, p);
    } 
    else {
        raiz->dir = inserir_pet_por_nome(raiz->dir, p);
    }

    return raiz;
}

void imprimir_pet_em_ordem(NoArvorePet* raiz) {
    if (raiz != NULL) {
        imprimir_pet_em_ordem(raiz->esq);
        
        struct pet* p = raiz->dado;
        printf("ID: %-4d | Nome: %-15s | Tipo: %d | Dono: %d\n", 
               p->codigo, p->nome, p->codigo_tipo, p->codigo_pes);
        
        imprimir_pet_em_ordem(raiz->dir);
    }
}

// Limpa a memória da árvore
void liberar_arvore_pet(NoArvorePet* raiz) {
    if (raiz != NULL) {
        liberar_arvore_pet(raiz->esq);
        liberar_arvore_pet(raiz->dir);
        free(raiz);
    }
}

void exibir_pets_ordenado_por_nome(struct pet* lista_inicio) {
    if (lista_inicio == NULL) {
        printf("Nenhum pet cadastrado para ordenar.\n");
        return;
    }

    NoArvorePet* raiz = NULL;
    struct pet* atual = lista_inicio;

    while (atual != NULL) {
        raiz = inserir_pet_por_nome(raiz, atual);
        atual = atual->prox;
    }

    printf("\n--- RELATORIO DE PETS (ORDEM ALFABETICA) ---\n");
    imprimir_pet_em_ordem(raiz);
    printf("--------------------------------------------\n");

    liberar_arvore_pet(raiz);
}