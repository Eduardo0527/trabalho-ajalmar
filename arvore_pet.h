#ifndef ARVORE_PET_H
#define ARVORE_PET_H

#include "pet.h"

typedef struct no_arvore_pet {
    struct pet* dado;
    struct no_arvore_pet* esq;
    struct no_arvore_pet* dir;
} NoArvorePet;

NoArvorePet* inserir_pet_por_nome(NoArvorePet* raiz, struct pet* p);
void imprimir_pet_em_ordem(NoArvorePet* raiz);
void liberar_arvore_pet(NoArvorePet* raiz);
void exibir_pets_ordenado_por_nome(struct pet* lista_inicio);

#endif