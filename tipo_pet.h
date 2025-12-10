#include <stdio.h>

struct tipo_de_pet{
    int codigo;
    char* nome;

    struct tipo_de_pet* prox;
    struct tipo_de_pet* ant;
} TipoPet;

struct tipo_de_pet* criar_lista_tipo_pet();

void inserir_tipo_pet(struct tipo_de_pet** inicio, int codigo, char* nome);
void carregar_tipos_pet_arquivo(struct tipo_de_pet** inicio, char* nome_arquivo);