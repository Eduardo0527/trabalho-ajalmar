#ifndef ARVORE_TIPO_H
#define ARVORE_TIPO_H

#include "tipo_pet.h"

// Nó da Árvore Binária para Tipos
typedef struct no_arvore_tipo {
    struct tipo_de_pet* dado; // Ponteiro para a lista original
    struct no_arvore_tipo* esq;
    struct no_arvore_tipo* dir;
} NoArvoreTipo;

// Funções
void exibir_tipos_ordenado_por_nome(struct tipo_de_pet* lista_inicio);

#endif