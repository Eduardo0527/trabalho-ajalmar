#include "arvore_pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NoArvore* criar_no_arvore(Pessoa* p) {
    NoArvore* novo = (NoArvore*) malloc(sizeof(NoArvore));
    if (novo != NULL) {
        novo->dado = p;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

// Insere na árvore usando o NOME como critério
NoArvore* inserir_por_nome(NoArvore* raiz, Pessoa* p) {
    if (raiz == NULL) {
        return criar_no_arvore(p);
    }

    if (strcmp(p->nome, raiz->dado->nome) < 0) {
        raiz->esq = inserir_por_nome(raiz->esq, p);
    } 
    else {
        raiz->dir = inserir_por_nome(raiz->dir, p);
    }

    return raiz;
}

// Imprimir alfabeticamente
void imprimir_em_ordem(NoArvore* raiz) {
    if (raiz != NULL) {
        imprimir_em_ordem(raiz->esq);
        
        printf("COD: %d | NOME: %s | FONE: %s | NASC: %s | END: %s\n", 
               raiz->dado->codigo, 
               raiz->dado->nome, 
               raiz->dado->fone, 
               raiz->dado->data_nascimento, 
               raiz->dado->endereco);
               
        imprimir_em_ordem(raiz->dir);
    }
}

void liberar_arvore(NoArvore* raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz); 
    }
}

void exibir_pessoas_ordenado_por_nome(Pessoa* lista_inicio) {
    if (lista_inicio == NULL) {
        printf("Lista vazia. Nada a ordenar.\n");
        return;
    }

    NoArvore* raiz = NULL;
    Pessoa* atual = lista_inicio;

    while (atual != NULL) {
        raiz = inserir_por_nome(raiz, atual);
        atual = atual->prox;
    }

    printf("--- RELATORIO DE PESSOAS (ORDENADO POR NOME) ---\n");
    imprimir_em_ordem(raiz);
    printf("------------------------------------------------\n");

    liberar_arvore(raiz);
}