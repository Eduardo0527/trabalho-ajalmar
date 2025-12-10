#include "pessoa.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Pessoa* criar_lista_pessoa() {
    return NULL;
}

void inserir_pessoa(Pessoa** inicio, int codigo, char* nome, char* fone, char* data, char* endereco) {
    Pessoa* novo = (Pessoa*) malloc(sizeof(Pessoa));
    
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
}

void carregar_pessoas_arquivo(Pessoa** inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "r");
    
    if (arq == NULL) return;

    int cod;
    char n[100], f[20], d[15], e[200];

    while (fscanf(arq, "%d;%[^;];%[^;];%[^;];%[^\n]\n", &cod, n, f, d, e) != EOF) {
        
        inserir_pessoa(inicio, cod, n, f, d, e);
    }

    fclose(arq);
}

void buscar_pessoa(Pessoa* inicio, int codigo) {
    Pessoa* atual = inicio;

    while (atual != NULL) {
        if (atual->codigo == codigo) {
            printf("Pessoa encontrada:\n");
            printf("Codigo: %d\n", atual->codigo);
            printf("Nome: %s\n", atual->nome);
            printf("Fone: %s\n", atual->fone);
            printf("Nasc: %s\n", atual->data_nascimento);
            printf("Endereco: %s\n", atual->endereco);
            return;
        }
        atual = atual->prox;
    }
    printf("Pessoa com codigo %d nao encontrada.\n", codigo);
}

void alterar_pessoa(Pessoa* inicio, int codigo, char* novo_nome, char* novo_fone, char* nova_data, char* novo_endereco) {
    Pessoa* atual = inicio;

    while (atual != NULL) {
        if (atual->codigo == codigo) {
            free(atual->nome);
            atual->nome = (char*) malloc(strlen(novo_nome) + 1);
            strcpy(atual->nome, novo_nome);

            free(atual->fone);
            atual->fone = (char*) malloc(strlen(novo_fone) + 1);
            strcpy(atual->fone, novo_fone);

            free(atual->data_nascimento);
            atual->data_nascimento = (char*) malloc(strlen(nova_data) + 1);
            strcpy(atual->data_nascimento, nova_data);

            free(atual->endereco);
            atual->endereco = (char*) malloc(strlen(novo_endereco) + 1);
            strcpy(atual->endereco, novo_endereco);

            printf("Pessoa com codigo %d alterada com sucesso.\n", codigo);
            return;
        }
        atual = atual->prox;
    }
    printf("Pessoa com codigo %d nao encontrada para alteracao.\n", codigo);
}

void excluir_pessoa(Pessoa** inicio, int codigo) {
    Pessoa* atual = *inicio;

    while (atual != NULL) {
        if (atual->codigo == codigo) {
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
            return;
        }
        atual = atual->prox;
    }
}