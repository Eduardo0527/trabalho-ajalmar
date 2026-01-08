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

Pessoa* buscar_pessoa(Pessoa* inicio, int codigo);

int inserir_pessoa(Pessoa** inicio, int codigo, char* nome, char* fone, char* data, char* endereco);

int alterar_pessoa(Pessoa* inicio, int codigo, char* novo_nome, char* novo_fone, char* nova_data, char* novo_endereco);
int excluir_pessoa(Pessoa** inicio, int codigo);

void carregar_pessoas_arquivo(Pessoa** inicio, char* nome_arquivo);

void salvar_pessoas_arquivo(Pessoa* inicio, char* nome_arquivo);

void listar_todas_pessoas(Pessoa* inicio);