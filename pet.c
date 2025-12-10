#include "pet.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct pet* criar_lista_pet() {
    return NULL;
}

void inserir_pet(struct pet** inicio, int codigo, int codigo_pet, char* nome, int codigo_tipo) {
    struct pet* novo = (struct pet*) malloc(sizeof(struct pet));
    
    novo->codigo = codigo;
    novo->codigo_pet = codigo_pet;
    
    novo->nome = (char*) malloc(strlen(nome) + 1);
    strcpy(novo->nome, nome);
    
    novo->codigo_tipo = codigo_tipo;

    novo->prox = *inicio; 

    *inicio = novo; 
}

void carregar_pets_arquivo(struct pet** inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "r");
    
    if (arq == NULL) return;
    
    int cod, cod_pet, cod_tipo;
    char n[100];

    while (fscanf(arq, "%d;%d;%[^;];%d\n", &cod, &cod_pet, n, &cod_tipo) != EOF) {
        
        inserir_pet(inicio, cod, cod_pet, n, cod_tipo);
    }

    fclose(arq);
}
