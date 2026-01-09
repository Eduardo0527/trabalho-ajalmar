#ifndef ARVORE_TIPO_H
#define ARVORE_TIPO_H

#include "tipo_pet.h"

typedef struct no_arvore_tipo {
    struct tipo_de_pet* dado;
    struct no_arvore_tipo* esq;
    struct no_arvore_tipo* dir;
} NoArvoreTipo;

void exibir_tipos_ordenado_por_nome(struct tipo_de_pet* lista_inicio);

#endif