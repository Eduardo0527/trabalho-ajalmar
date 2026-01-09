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
     strncpy(novo->dado.linha_original, linha_lida, 199);
    novo->dado.linha_original[199] = '\0';

    novo->dado.valido = 0;
    novo->dado.operacao = INVALIDO;
    novo->dado.tabela = DESCONHECIDO;
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
        buffer[strcspn(buffer, "\n")] = '\0';
        if(strlen(buffer) == 0) continue;
        enfileirar(fila_geral,buffer);
    }
    fclose(arquivo);
    return 1;
}

void str_to_lower(char *dest, const char *src){
    int i = 0;
    while(src[i]){
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
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

int minha_strncasecmp(const char *s1, const char *s2, size_t n) {
    while (n > 0) {
        unsigned char c1 = (unsigned char)tolower((unsigned char)*s1);
        unsigned char c2 = (unsigned char)tolower((unsigned char)*s2);

        if (c1 != c2) return c1 - c2;
        if (c1 == '\0') return 0;

        s1++;
        s2++;
        n--;
    }
    return 0;
}

// Verifica se a string começa com o prefixo e avança o ponteiro
char* verificar_inicio(char *str, const char *prefixo) {
    str = pular_espacos(str);
    int len = strlen(prefixo);

    if (minha_strncasecmp(str, prefixo, len) == 0) {
        char prox = str[len];
        if (prox == '\0' || isspace((unsigned char)prox) || 
            prox == '(' || prox == ')' || prox == ',' || prox == ';' || prox == '*') {
            return str + len;
        }
    }
    return NULL;
}

char* pular_espacos(char *str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
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

// Tenta casar o cursor com qualquer campo válido para a tabela especificada.
// Retorna o ponteiro avançado se achar um campo válido.
// Retorna NULL se o que estiver lá não for um campo da tabela.
char* verificar_qualquer_campo(char *cursor, TabelaAlvo t) {
    char *p;

    if ((p = verificar_inicio(cursor, "codigo"))) return p; // Todas tem codigo

    if (t == PESSOA) {
        if ((p = verificar_inicio(cursor, "nome"))) return p;
        if ((p = verificar_inicio(cursor, "fone"))) return p;
        if ((p = verificar_inicio(cursor, "endereco"))) return p;
        if ((p = verificar_inicio(cursor, "data_nascimento"))) return p;
    }
    else if (t == PET) {
        if ((p = verificar_inicio(cursor, "nome"))) return p;
        if ((p = verificar_inicio(cursor, "codigo_pes"))) return p; // Campo do dono
        if ((p = verificar_inicio(cursor, "codigo_tipo"))) return p;
    }
    else if (t == TIPO_PET) {
        if ((p = verificar_inicio(cursor, "descricao"))) return p;
    }

    return NULL;
}

int analisar_comando_estrito(comando *cmd) {
    char *cursor = cmd->linha_original;
    char *temp;

    // INSERT
    temp = verificar_inicio(cursor, "insert");
    if (temp) {
        cursor = temp;
        
        cursor = verificar_inicio(cursor, "into");
        if (!cursor) return 0;
        
        TabelaAlvo t = verificar_tabela(&cursor);
        if (t == DESCONHECIDO) return 0;
        
        cursor = pular_espacos(cursor);
        if (*cursor != '(') return 0;
        
        while (*cursor != '\0' && *cursor != ')') {
            cursor++;
        }
        if (*cursor != ')') return 0; 
        cursor++; 
        
        cursor = verificar_inicio(cursor, "values");
        if (!cursor) return 0;

        cmd->operacao = INSERT;
        cmd->tabela = t;
        return 1;
    }

    // DELETE
    temp = verificar_inicio(cmd->linha_original, "delete"); 
    if (temp) {
        cursor = temp;
        
        cursor = verificar_inicio(cursor, "from");
        if (!cursor) return 0;

        TabelaAlvo t = verificar_tabela(&cursor);
        if (t == DESCONHECIDO) return 0;

        cursor = verificar_inicio(cursor, "where");
        if (!cursor) return 0;
        if (verificar_qualquer_campo(cursor, t) == NULL) return 0;

        cmd->operacao = DELETE;
        cmd->tabela = t;
        return 1;
    }

    // SELECT
    temp = verificar_inicio(cmd->linha_original, "select");
    if (temp) {
        cursor = temp;
        
        cursor = pular_espacos(cursor);
        if (*cursor != '*') return 0; 
        cursor++; 

        cursor = verificar_inicio(cursor, "from");
        if (!cursor) return 0; 

        TabelaAlvo t = verificar_tabela(&cursor);
        if (t == DESCONHECIDO) return 0; 

        cursor = pular_espacos(cursor);
        
        if (*cursor != '\0' && *cursor != ';') {
            
            // Caso 1: Verifica se é WHERE
            char *temp_where = verificar_inicio(cursor, "where");
            if (temp_where) {
                // Se achou WHERE, o próximo tem que ser um campo válido
                if (verificar_qualquer_campo(temp_where, t) == NULL) return 0;
            }
            // Caso 2: Verifica se é ORDER
            else {
                char *temp_order = verificar_inicio(cursor, "order");
                if (temp_order) {
                    // Se achou ORDER, o próximo TEM que ser "by"
                    char *temp_by = verificar_inicio(temp_order, "by");
                    if (!temp_by) return 0;
                    
                    if (verificar_inicio(temp_by, "nome") == NULL) return 0;
                } 
                else {
                    return 0; 
                }
            }
        }

        cmd->operacao = SELECT;
        cmd->tabela = t;
        return 1;
    }

    // UPDATE
    temp = verificar_inicio(cmd->linha_original, "update");
    if (temp) {
        cursor = temp;
        
        TabelaAlvo t = verificar_tabela(&cursor);
        if (t == DESCONHECIDO) return 0;

        cursor = verificar_inicio(cursor, "set");
        if (!cursor) return 0; 

        cursor = pular_espacos(cursor);
        if (*cursor == '\0' || *cursor == ';') return 0; // "update ... set;" é inválido
        
        cmd->operacao = UPDATE;
        cmd->tabela = t;
        return 1;
    }

    return 0; 
}

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
        
        if (no_atual == NULL) break;

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
            printf("[ERRO] Comando invalido removido: %s\n", no_atual->dado.linha_original);
            free(no_atual);
        }
    }
    
    printf("Validacao concluida.\n");
}

void enfileirar_no_existente(filaComandos *f, noComando *no) {
    no->prox = NULL;

    if (f->inicio == NULL) {
        f->inicio = no;
    } else {
        f->fim->prox = no;
    }
    f->fim = no;
}

void distribuir_comandos(filaComandos *f_geral, filaComandos *f_pessoa, filaComandos *f_pet, filaComandos *f_tipo) {
    printf("Distribuindo comandos para filas especificas...\n");

    while (f_geral->inicio != NULL) {
        
        noComando *no_atual = desenfileirar_no(f_geral);
        
        switch (no_atual->dado.tabela) {
            case PESSOA:
                enfileirar_no_existente(f_pessoa, no_atual);
                break;
                
            case PET:
                enfileirar_no_existente(f_pet, no_atual);
                break;
                
            case TIPO_PET:
                enfileirar_no_existente(f_tipo, no_atual);
                break;
                
            default:
                printf("Erro: Comando orfao encontrado (Tabela %d). Descartando.\n", no_atual->dado.tabela);
                free(no_atual);
                break;
        }
    }
    
    printf("Distribuicao concluida.\n");
}