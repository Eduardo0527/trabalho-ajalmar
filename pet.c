#include "pet.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct pet* criar_lista_pet() {
    return NULL;
}

struct pet* buscar_pet(struct pet* inicio, int codigo) {
    struct pet* atual = inicio;
    while (atual != NULL) {
        if (atual->codigo == codigo) return atual;
        atual = atual->prox;
    }
    return NULL;
}

int inserir_pet(struct pet** inicio, int codigo, int codigo_pes, char* nome, int codigo_tipo) {
    if (nome == NULL || strlen(nome) == 0) return 0;
    if (buscar_pet(*inicio, codigo) != NULL) return -1;

    struct pet* novo = (struct pet*) malloc(sizeof(struct pet));
    novo->codigo = codigo;
    novo->codigo_pes = codigo_pes;
    novo->codigo_tipo = codigo_tipo;
    novo->nome = strdup(nome);

    novo->prox = *inicio; 
    novo->ant = NULL;

    if (*inicio != NULL) (*inicio)->ant = novo;
    *inicio = novo; 
    return 1;
}

void carregar_pets_arquivo(struct pet** inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "r");
    if (arq == NULL) return;
    
    int cod, cod_pes, cod_tipo; char n[100];
    while (fscanf(arq, "%d;%d;%[^;];%d\n", &cod, &cod_pes, n, &cod_tipo) != EOF) {
        inserir_pet(inicio, cod, cod_pes, n, cod_tipo);
    }
    fclose(arq);
    printf("Pets carregados.\n");
}

// Exclui um único pet
int excluir_pet(struct pet** inicio, int codigo) {
    struct pet* atual = buscar_pet(*inicio, codigo);
    if (atual == NULL) return 0;

    if (atual->ant != NULL) atual->ant->prox = atual->prox;
    else *inicio = atual->prox;

    if (atual->prox != NULL) atual->prox->ant = atual->ant;

    free(atual->nome);
    free(atual);
    return 1;
}

// Exclui todos os pets de um dono (para a cascata)
int excluir_pets_por_dono(struct pet** inicio, int codigo_pes) {
    struct pet* atual = *inicio;
    int cont = 0;
    while (atual != NULL) {
        struct pet* prox_temp = atual->prox; // Guarda o próximo antes de deletar
        if (atual->codigo_pes == codigo_pes) {
            excluir_pet(inicio, atual->codigo);
            cont++;
        }
        atual = prox_temp;
    }
    return cont;
}

// Verifica se um tipo está sendo usado (para o bloqueio)
int tipo_tem_pets(struct pet* inicio, int codigo_tipo) {
    struct pet* atual = inicio;
    while (atual != NULL) {
        if (atual->codigo_tipo == codigo_tipo) return 1;
        atual = atual->prox;
    }
    return 0;
}

int alterar_pet(struct pet* inicio, int codigo, char* novo_nome, int novo_dono_id) {
    struct pet* atual = buscar_pet(inicio, codigo);

    if (atual == NULL) {
        printf("[ERRO] Pet %d nao encontrado para alteracao.\n", codigo);
        return 0;
    }

    // Alterar Nome
    if (novo_nome != NULL && strlen(novo_nome) > 0) {
        // Libera o nome antigo para não vazar memória
        free(atual->nome);
        // Aloca e copia o novo (use strdup se disponível, ou malloc+strcpy)
        atual->nome = (char*) malloc(strlen(novo_nome) + 1);
        strcpy(atual->nome, novo_nome);
    }

    // Alterar Dono (Ex: Adoção ou correção)
    if (novo_dono_id != -1) {
        atual->codigo_pes = novo_dono_id;
    }

    return 1;
}

// --- FUNÇÃO DE SALVAR (PERSISTÊNCIA) ---
void salvar_pets_arquivo(struct pet* inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "w"); // "w" apaga o anterior e escreve o novo
    
    if (arq == NULL) {
        printf("[ERRO CRITICO] Nao foi possivel salvar em %s\n", nome_arquivo);
        return;
    }

    struct pet* atual = inicio;
    while (atual != NULL) {
        // Formato: CODIGO;COD_DONO;NOME;COD_TIPO
        fprintf(arq, "%d;%d;%s;%d\n", 
                atual->codigo, 
                atual->codigo_pes, 
                atual->nome, 
                atual->codigo_tipo);
        
        atual = atual->prox;
    }

    fclose(arq);
    // printf("Dados dos pets salvos com sucesso.\n");
}

void listar_todos_pets(struct pet* inicio) {
    if (inicio == NULL) {
        printf("Nenhum pet cadastrado.\n");
        return;
    }
    struct pet* p = inicio;
    while (p != NULL) {
        printf("ID: %-4d | Nome: %-15s | Dono ID: %-4d | Tipo ID: %d\n", 
               p->codigo, p->nome, p->codigo_pes, p->codigo_tipo);
        p = p->prox;
    }
}