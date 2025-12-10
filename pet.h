#include <stdio.h>

struct pet{
    int codigo;
    int codigo_pet;
    char* nome;
    int codigo_tipo;

    struct pet* prox;
    struct pet* ant;
} Pet;

struct pet* criar_lista_pet();

void inserir_pet(struct pet** inicio, int codigo, int codigo_pet, char* nome, int codigo_tipo);     
void carregar_pets_arquivo(struct pet** inicio, char* nome_arquivo);