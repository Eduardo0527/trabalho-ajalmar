#include <stdio.h>
#include <stdlib.h>

typedef struct pessoa {
    int codigo;
    char* nome;
    char* fone;
    char* data_nascimento;
    char* endereco;
    
    struct pessoa* prox;
    struct pessoa* ant;
} Pessoa;

Pessoa* criar_lista_pessoa();

void inserir_pessoa(Pessoa** inicio, int codigo, char* nome, char* fone, char* data, char* endereco);

void carregar_pessoas_arquivo(Pessoa** inicio, char* nome_arquivo);