#include "tipo_pet.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct tipo_de_pet* criar_lista_tipo_pet() {
    return NULL;
}

void inserir_tipo_pet(struct tipo_de_pet** inicio, int codigo, char* nome) {
    struct tipo_de_pet* novo = (struct tipo_de_pet*) malloc(sizeof(struct tipo_de_pet));
    
    novo->codigo = codigo;
    
    novo->nome = (char*) malloc(strlen(nome) + 1);
    strcpy(novo->nome, nome);

    novo->prox = *inicio; 
    novo->ant = NULL;

    if (*inicio != NULL) {
        (*inicio)->ant = novo;
    }
    *inicio = novo; 
}

void carregar_tipos_pet_arquivo(struct tipo_de_pet** inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "r");
    
    if (arq == NULL) return;
    
    int cod;
    char n[100];

    while (fscanf(arq, "%d;%[^\n]\n", &cod, n) != EOF) {
        
        inserir_tipo_pet(inicio, cod, n);
    }

    fclose(arq);
}