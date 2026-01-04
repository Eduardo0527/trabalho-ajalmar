#include "comando.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void inicializar_fila(filaComandos *f){
    f->inicio = NULL;
    f->fim = NULL;
}

void enfileirar(filaComandos *f, char* linha_lida){
    noComando *novo = (noComando*) malloc(sizeof(noComando));
    if(novo == NULL){
        printf("Erro ao alocar memoria!\n");
        return;
    }
    strcpy(novo->dado.linha_original, linha_lida);
    novo->prox = NULL;
    if(f->fim == NULL){
        f->inicio = novo;
    }else{
        f->fim->prox = novo;
    }
    f->fim = novo;
}

int carregar_script(char *nome_arquivo, filaComandos *fila_geral){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }
    char buffer[200];
    while(fgets(buffer, 200, arquivo) != NULL){
        buffer[strcspn(buffer, "\n")];
        if(strlen(buffer) == 0) continue;
        enfileirar(fila_geral,buffer);
    }
    fclose(arquivo);
    return 1;
}

void str_to_lower(char *dest, const char *src){
    int i = 0;
    while(src[i]){
        dest[i] = towlower(src[i]);
        i++;
    }
    dest[i] = '\0';
}

int analisar_comando(comando *cmd){
    char temp[200];
    str_to_lower(temp, cmd->linha_original);
    
}