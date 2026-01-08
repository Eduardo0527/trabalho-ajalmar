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

// --- FUNÇÃO DE ALTERAR (UPDATE) ---
int alterar_tipo_pet(struct tipo_de_pet* inicio, int codigo, char* novo_nome) {
    struct tipo_de_pet* atual = buscar_tipo_pet(inicio, codigo);
    
    if (atual == NULL) {
        return 0; // Não encontrado
    }

    if (novo_nome != NULL && strlen(novo_nome) > 0) {
        free(atual->nome); // Libera o antigo
        atual->nome = strdup(novo_nome); // Copia o novo
    }
    
    return 1; // Sucesso
}

// --- FUNÇÃO DE SALVAR (PERSISTÊNCIA) ---
void salvar_tipos_pet_arquivo(struct tipo_de_pet* inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel salvar tipos em %s\n", nome_arquivo);
        return;
    }

    struct tipo_de_pet* p = inicio;
    while (p != NULL) {
        // Formato: CODIGO;NOME
        fprintf(arq, "%d;%s\n", p->codigo, p->nome);
        p = p->prox;
    }
    fclose(arq);
}

// --- FUNÇÃO AUXILIAR DE LISTAGEM ---
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