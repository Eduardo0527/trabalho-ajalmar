#include <stdio.h>
#include <stdlib.h>
#include "pessoa.h"

int main() {
    Pessoa* lista = criar_lista_pessoa();
    Pessoa* atual = lista;
    if (atual == NULL) {
        printf("A lista esta vazia! Verifique se o arquivo pessoas.txt esta na pasta correta.\n");
    }

    while (atual != NULL) {
        printf("Codigo: %d\n", atual->codigo);
        printf("Nome: %s\n", atual->nome);
        printf("Fone: %s\n", atual->fone);
        printf("Nasc: %s\n", atual->data_nascimento);
        printf("Endereco: %s\n", atual->endereco);
        printf("-------------------------\n");
        
        atual = atual->prox;
    }

    return 0;
}