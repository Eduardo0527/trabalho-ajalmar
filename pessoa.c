#include "pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pessoa* criar_lista_pessoa() {
    return NULL;
}

Pessoa* buscar_pessoa(Pessoa* inicio, int codigo) {
    Pessoa* atual = inicio;
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL; 
}

// Retorna: 1 (Sucesso), 0 (Erro de validação), -1 (Erro de duplicidade)
int inserir_pessoa(Pessoa** inicio, int codigo, char* nome, char* fone, char* data, char* endereco) {
    
    if (nome == NULL || strlen(nome) == 0) {
        printf("[ERRO] Nao e permitido cadastrar pessoa sem nome.\n");
        return 0; 
    }

    if (buscar_pessoa(*inicio, codigo) != NULL) {
        printf("[ERRO] Ja existe uma pessoa cadastrada com o codigo %d.\n", codigo);
        return -1;
    }

    Pessoa* novo = (Pessoa*) malloc(sizeof(Pessoa));
    if (novo == NULL) return 0; 

    novo->codigo = codigo;
    
    novo->nome = (char*) malloc(strlen(nome) + 1);
    strcpy(novo->nome, nome);
    
    novo->fone = (char*) malloc(strlen(fone) + 1);
    strcpy(novo->fone, fone);

    novo->data_nascimento = (char*) malloc(strlen(data) + 1);
    strcpy(novo->data_nascimento, data);

    novo->endereco = (char*) malloc(strlen(endereco) + 1);
    strcpy(novo->endereco, endereco);

    novo->prox = *inicio; 
    novo->ant = NULL;

    if (*inicio != NULL) {
        (*inicio)->ant = novo;
    }
    *inicio = novo; 
    printf("%s Inserido", nome);

    return 1; 
}

void carregar_pessoas_arquivo(Pessoa** inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "r");
    
    if (arq == NULL) {
        printf("Arquivo %s nao encontrado ou falha na abertura.\n", nome_arquivo);
        return;
    }

    int cod;
    char n[100], f[20], d[15], e[200]; 

    while (fscanf(arq, "%d;%[^;];%[^;];%[^;];%[^\n]\n", &cod, n, f, d, e) != EOF) {
        
        int status = inserir_pessoa(inicio, cod, n, f, d, e);
        
        if (status == -1) {
            printf("Aviso: Registro duplicado (Codigo %d) no arquivo ignorado.\n", cod);
        }
    }

    fclose(arq);
    printf("Carga de pessoas finalizada.\n");
}

// Retorna: 1 (Sucesso), 0 (Pessoa não encontrada)
int alterar_pessoa(Pessoa* inicio, int codigo, char* novo_nome, char* novo_fone, char* nova_data, char* novo_endereco) {
    
    Pessoa* atual = buscar_pessoa(inicio, codigo);

    if (atual == NULL) {
        printf("[ERRO] Pessoa %d nao encontrada para alteracao.\n", codigo);
        return 0;
    }
    
    if (novo_nome != NULL) {
        free(atual->nome);
        atual->nome = (char*) malloc(strlen(novo_nome) + 1);
        strcpy(atual->nome, novo_nome);
    }

    if (novo_fone != NULL) {
        free(atual->fone);
        atual->fone = (char*) malloc(strlen(novo_fone) + 1);
        strcpy(atual->fone, novo_fone);
    }

    if (nova_data != NULL) {
        free(atual->data_nascimento);
        atual->data_nascimento = (char*) malloc(strlen(nova_data) + 1);
        strcpy(atual->data_nascimento, nova_data);
    }

    if (novo_endereco != NULL) {
        free(atual->endereco);
        atual->endereco = (char*) malloc(strlen(novo_endereco) + 1);
        strcpy(atual->endereco, novo_endereco);
    }

    return 1;
}

// Retorna: 1 (Sucesso), 0 (Pessoa não encontrada)
int excluir_pessoa(Pessoa** inicio, int codigo) {
    
    Pessoa* atual = buscar_pessoa(*inicio, codigo);

    if (atual == NULL) {
        printf("[ERRO] Pessoa %d nao encontrada para exclusao.\n", codigo);
        return 0;
    }

    if (atual->ant != NULL) {
        atual->ant->prox = atual->prox;
    } else {
        *inicio = atual->prox;
    }

    if (atual->prox != NULL) {
        atual->prox->ant = atual->ant;
    }

    free(atual->nome);
    free(atual->fone);
    free(atual->data_nascimento);
    free(atual->endereco);

    free(atual);

    return 1; 
}