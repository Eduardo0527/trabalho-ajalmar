#ifndef ARVORE_PESSOA_H
#define ARVORE_PESSOA_H

#include "pessoa.h" // Precisamos conhecer a struct Pessoa

// Estrutura do Nó da Árvore
// Note que armazenamos um PONTEIRO para a pessoa, não duplicamos os dados.
typedef struct no_arvore {
    Pessoa* dado;            // Aponta para a pessoa que está na lista encadeada
    struct no_arvore* esq;
    struct no_arvore* dir;
} NoArvore;

// Funções da Árvore
NoArvore* inserir_por_nome(NoArvore* raiz, Pessoa* p);
void imprimir_em_ordem(NoArvore* raiz);
void liberar_arvore(NoArvore* raiz);

// Função "Ponte" (Recebe a lista e gera o relatório)
void exibir_pessoas_ordenado_por_nome(Pessoa* lista_inicio);

#endif