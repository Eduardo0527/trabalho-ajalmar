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

int inserir_pet(struct pet** inicio, int codigo, int codigo_pes, char* nome, int codigo_tipo, struct tipo_de_pet* lista_tipos) {
    if (nome == NULL || strlen(nome) == 0) return 0;
    
    if (buscar_pet(*inicio, codigo) != NULL) {
        printf("[ERRO] Pet com codigo %d ja existe.\n", codigo);
        return -1;
    }

    if (buscar_tipo_pet(lista_tipos, codigo_tipo) == NULL) {
        printf("[ERRO] Tipo de Pet %d nao existe. Cadastro negado.\n", codigo_tipo);
        return 0;
    }

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

void carregar_pets_arquivo(struct pet** inicio, char* nome_arquivo, struct tipo_de_pet* lista_tipos) {
    FILE* arq = fopen(nome_arquivo, "r");
    
    if (arq == NULL) {
        printf("Arquivo de pets %s nao encontrado. Iniciando lista vazia.\n", nome_arquivo);
        return;
    }
    
    int cod, cod_pes, cod_tipo; 
    char n[100];
    int total_carregados = 0;
    int total_ignorados = 0;

    while (fscanf(arq, "%d;%d;%[^;];%d\n", &cod, &cod_pes, n, &cod_tipo) != EOF) {

        int status = inserir_pet(inicio, cod, cod_pes, n, cod_tipo, lista_tipos);
        
        if (status == 1) {
            total_carregados++;
        } else {
            printf("[AVISO CARGA] Pet '%s' (ID %d) ignorado. Motivo: Tipo %d invalido ou erro de dados.\n", n, cod, cod_tipo);
            total_ignorados++;
        }
    }

    fclose(arq);
    printf("Carga de Pets finalizada: %d carregados, %d ignorados.\n", total_carregados, total_ignorados);
}

// Exclui um pet
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

int excluir_pets_por_dono(struct pet** inicio, int codigo_pes) {
    struct pet* atual = *inicio;
    int cont = 0;
    while (atual != NULL) {
        struct pet* prox_temp = atual->prox; 
        if (atual->codigo_pes == codigo_pes) {
            excluir_pet(inicio, atual->codigo);
            cont++;
        }
        atual = prox_temp;
    }
    return cont;
}

// Verifica se um tipo está sendo usado
int tipo_tem_pets(struct pet* inicio, int codigo_tipo) {
    struct pet* atual = inicio;
    while (atual != NULL) {
        if (atual->codigo_tipo == codigo_tipo) return 1;
        atual = atual->prox;
    }
    return 0;
}

int alterar_pet(struct pet* inicio, int codigo, char* novo_nome, int* novo_dono_id, int* novo_tipo_id) {
    
    // 1. Busca o pet na lista
    struct pet* atual = buscar_pet(inicio, codigo);

    if (atual == NULL) {
        printf("[ERRO] Pet %d nao encontrado para alteracao.\n", codigo);
        return 0;
    }

    // 2. Alterar nome 
    if (novo_nome != NULL && strlen(novo_nome) > 0) {
        if (atual->nome != NULL) {
            free(atual->nome);
        }
        
        atual->nome = (char*) malloc(strlen(novo_nome) + 1);
        if (atual->nome != NULL) {
            strcpy(atual->nome, novo_nome);
        }
    }

    // 3. Alterar Dono
    if (novo_dono_id != NULL) {
        atual->codigo_pes = *novo_dono_id;
    }

    if (novo_tipo_id != NULL) {
        atual->codigo_tipo = *novo_tipo_id;
    }

    return 1; 
}

// Função de salvar
void salvar_pets_arquivo(struct pet* inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "w"); // "w" apaga o anterior e escreve o novo
    
    if (arq == NULL) {
        printf("[ERRO CRITICO] Nao foi possivel salvar em %s\n", nome_arquivo);
        return;
    }

    struct pet* atual = inicio;
    while (atual != NULL) {
        fprintf(arq, "%d;%d;%s;%d\n", 
                atual->codigo, 
                atual->codigo_pes, 
                atual->nome, 
                atual->codigo_tipo);
        
        atual = atual->prox;
    }

    fclose(arq);
}

void listar_pet(struct pet* inicio, int codigo){
   struct pet* atual = inicio;
    while (atual != NULL && atual->codigo != codigo) {
        atual = atual->prox;
    }
    if(atual == NULL){
        printf("Não existe pessoa com o codigo: %d para ser exibida", codigo);
    }else{
        printf("ID: %-4d | Nome: %-15s | Dono ID: %-4d | Tipo ID: %d\n", atual->codigo, atual->nome, atual->codigo_pes, atual->codigo_tipo);
    }
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