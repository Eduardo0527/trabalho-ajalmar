#ifndef ARVORE_PET_H
#define ARVORE_PET_H

#include "pet.h" // Necessário para acessar 'struct pet'

// Estrutura do Nó da Árvore de Pets
typedef struct no_arvore_pet {
    struct pet* dado;            // Ponteiro para o pet na lista original
    struct no_arvore_pet* esq;
    struct no_arvore_pet* dir;
} NoArvorePet;

// Funções Auxiliares
NoArvorePet* inserir_pet_por_nome(NoArvorePet* raiz, struct pet* p);
void imprimir_pet_em_ordem(NoArvorePet* raiz);
void liberar_arvore_pet(NoArvorePet* raiz);

// Função Principal (Interface Pública)
void exibir_pets_ordenado_por_nome(struct pet* lista_inicio);

#endif