#include "tipo_pet.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct tipo_de_pet* criar_lista_tipo_pet() {
    return NULL;
}

struct tipo_de_pet* buscar_tipo_pet(struct tipo_de_pet* inicio, int codigo) {
    struct tipo_de_pet* atual = inicio;
    while (atual != NULL) {
        if (atual->codigo == codigo) return atual;
        atual = atual->prox;
    }
    return NULL;
}

int inserir_tipo_pet(struct tipo_de_pet** inicio, int codigo, char* nome) {
    if (nome == NULL || strlen(nome) == 0) return 0;
    if (buscar_tipo_pet(*inicio, codigo) != NULL) return -1;

    struct tipo_de_pet* novo = (struct tipo_de_pet*) malloc(sizeof(struct tipo_de_pet));
    novo->codigo = codigo;
    novo->nome = strdup(nome);
    novo->prox = *inicio; 
    novo->ant = NULL;

    if (*inicio != NULL) (*inicio)->ant = novo;
    *inicio = novo; 
    return 1;
}

void carregar_tipos_pet_arquivo(struct tipo_de_pet** inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "r");
    if (arq == NULL) return;
    
    int cod; char n[100];
    while (fscanf(arq, "%d;%[^\n]\n", &cod, n) != EOF) {
        inserir_tipo_pet(inicio, cod, n);
    }
    fclose(arq);
    printf("Tipos carregados.\n");
}

int excluir_tipo_pet(struct tipo_de_pet** inicio, int codigo) {
    struct tipo_de_pet* atual = buscar_tipo_pet(*inicio, codigo);
    if (atual == NULL) return 0;

    if (atual->ant != NULL) atual->ant->prox = atual->prox;
    else *inicio = atual->prox;

    if (atual->prox != NULL) atual->prox->ant = atual->ant;

    free(atual->nome);
    free(atual);
    return 1;
}

// FUNÇÃO DE ALTERAR
int alterar_tipo_pet(struct tipo_de_pet* inicio, int codigo, char* nova_descricao) {
    // 1. Busca
    struct tipo_de_pet* atual = buscar_tipo_pet(inicio, codigo); // Supondo que exista essa função

    if (atual == NULL) {
        printf("[ERRO] TipoPet %d nao encontrado.\n", codigo);
        return 0;
    }

    // 2. Altera (apenas se não for nulo)
    if (nova_descricao != NULL && strlen(nova_descricao) > 0) {
        strcpy(atual->nome, nova_descricao);
    }
    
    return 1;
}

// FUNÇÃO DE SALVAR
void salvar_tipos_pet_arquivo(struct tipo_de_pet* inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel salvar tipos em %s\n", nome_arquivo);
        return;
    }

    struct tipo_de_pet* p = inicio;
    while (p != NULL) {
        fprintf(arq, "%d;%s\n", p->codigo, p->nome);
        p = p->prox;
    }
    fclose(arq);
}

void listar_tipo_pet(struct tipo_de_pet* inicio, int codigo) {
    struct tipo_de_pet* atual = inicio;
    while (atual != NULL && atual->codigo != codigo) {
        atual = atual->prox;
    }
    if(atual == NULL){
        printf("Não existe tipo de pet com o codigo: %d para ser exibida", codigo);
    }else{
        printf("ID: %-4d | Tipo: %s\n", atual->codigo, atual->nome);
    }
}

// FUNÇÃO DE LISTAGEM
void listar_todos_tipos(struct tipo_de_pet* inicio) {
    if (inicio == NULL) {
        printf("Nenhum tipo cadastrado.\n");
        return;
    }
    struct tipo_de_pet* p = inicio;
    while (p != NULL) {
        printf("ID: %-4d | Tipo: %s\n", p->codigo, p->nome);
        p = p->prox;
    }
}