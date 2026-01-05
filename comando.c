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

noComando *desenfileirar_no(filaComandos *f){
    if (f->inicio == NULL) {
        return NULL;
    }
    noComando *no_removido = f->inicio;

    f->inicio = f->inicio->prox;

    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    no_removido->prox = NULL;

    return no_removido;
}

char* pular_espacos(char *str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

char* verificar_inicio(char *str, const char *prefixo) {
    str = pular_espacos(str);
    int len = strlen(prefixo);
    
    if (_strnicmp(str, prefixo, len) == 0) {
        return str + len;
    }
    return NULL;
}

int verificar_tabela(char **cursor) {
    *cursor = pular_espacos(*cursor);
    
    if (strncasecmp(*cursor, "pessoa", 6) == 0) {
        *cursor += 6;
        return PESSOA;
    }
    if (strncasecmp(*cursor, "tipo_pet", 8) == 0) {
        *cursor += 8;
        return TIPO_PET;
    }
    if (strncasecmp(*cursor, "pet", 3) == 0) {
        *cursor += 3;
        return PET;
    }
    return DESCONHECIDO;
}

int analisar_comando_estrito(comando *cmd) {
    char *cursor = cmd->linha_original;
    
    char *temp = verificar_inicio(cursor, "insert");
    if (temp) {
        cursor = temp;
        
        cursor = verificar_inicio(cursor, "into");
        if (!cursor) return 0;
        
        TabelaAlvo t = verificar_tabela(&cursor);
        cursor = pular_espacos(cursor);
        if (*cursor != '(') return 0;
        if (t == DESCONHECIDO) return 0; // Erro: tabela inválida ou lixo
        
        if (strcasestr(cursor, "values") == NULL) return 0;

        cmd->operacao = INSERT;
        cmd->tabela = t;
        return 1;
    }

    temp = verificar_inicio(cmd->linha_original, "delete"); 
    if (temp) {
        cursor = temp;
        
        cursor = verificar_inicio(cursor, "from");
        if (!cursor) return 0;

        TabelaAlvo t = verificar_tabela(&cursor);
        if (t == DESCONHECIDO) return 0;

        if (strcasestr(cursor, "where") == NULL) return 0;

        cmd->operacao = DELETE;
        cmd->tabela = t;
        return 1;
    }
    temp = verificar_inicio(cmd->linha_original, "select");
    if (temp) {
        cursor = temp;
        
        cursor = pular_espacos(cursor);
        if (*cursor != '*') {
            return 0; 
        }
        cursor++;

        cursor = verificar_inicio(cursor, "from");
        if (!cursor) return 0; // Select sem from

        TabelaAlvo t = verificar_tabela(&cursor);
        if (t == DESCONHECIDO) return 0; // Tabela inválida

        cursor = pular_espacos(cursor);
        if (*cursor != '\0' && *cursor != ';') {
            // Se tem mais texto, deve começar com 'where' ou 'order'
            if (verificar_inicio(cursor, "where") == NULL && 
                verificar_inicio(cursor, "order") == NULL) {
                return 0; // Tem lixo após o nome da tabela
            }
        }

        cmd->operacao = SELECT;
        cmd->tabela = t;
        return 1;
    }

    temp = verificar_inicio(cmd->linha_original, "update");
    if (temp) {
        cursor = temp;
        TabelaAlvo t = verificar_tabela(&cursor);
        if (t == DESCONHECIDO) return 0;

        // 2. Verifica 'set'
        cursor = verificar_inicio(cursor, "set");
        if (!cursor) return 0; 

        cmd->operacao = UPDATE;
        cmd->tabela = t;
        return 1;
    }

    return 0; 
}

/* * Função: processar_validacao_fila
 * Objetivo: Percorre a fila bruta, valida cada comando e mantém apenas os válidos.
 * Lógica: Retira do início -> Valida -> Se bom, põe no fim. Se ruim, free().
 */
void processar_validacao_fila(filaComandos *f) {
    if (f->inicio == NULL) {
        printf("A fila esta vazia. Nada a processar.\n");
        return;
    }

    int total_elementos = 0;
    noComando *aux = f->inicio;
    while (aux != NULL) {
        total_elementos++;
        aux = aux->prox;
    }

    printf("Iniciando validacao de %d comandos...\n", total_elementos);


    int i;
    for (i = 0; i < total_elementos; i++) {
        
        noComando *no_atual = desenfileirar_no(f);
        
        if (no_atual == NULL) break; // Segurança

        int eh_valido = analisar_comando_estrito(&no_atual->dado);

        if (eh_valido) {
            no_atual->dado.valido = 1;
            

            no_atual->prox = NULL;
            
            if (f->fim == NULL) {
                f->inicio = no_atual;
            } else {
                f->fim->prox = no_atual;
            }
            f->fim = no_atual; 

        } else {
            // --- CASO INVÁLIDO: Joga fora ---
            printf("[ERRO] Comando invalido removido: %s\n", no_atual->dado.linha_original);
            
            // Libera a memória do nó, pois ele não serve mais
            free(no_atual);
        }
    }
    
    printf("Validacao concluida.\n");
}