#ifndef ARVORE_PESSOA_H
#define ARVORE_PESSOA_H

#include "pessoa.h" 


typedef struct no_arvore {
    Pessoa* dado;
    struct no_arvore* esq;
    struct no_arvore* dir;
} NoArvore;

NoArvore* inserir_por_nome(NoArvore* raiz, Pessoa* p);
void imprimir_em_ordem(NoArvore* raiz);
void liberar_arvore(NoArvore* raiz);
void exibir_pessoas_ordenado_por_nome(Pessoa* lista_inicio);

#endif