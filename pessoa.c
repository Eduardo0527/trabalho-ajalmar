#include "pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* alocar_string_segura(char* origem) {
    if (origem == NULL) {
        char* vazio = (char*) malloc(1);
        if (vazio) vazio[0] = '\0';
        return vazio;
    }
    char* destino = (char*) malloc(strlen(origem) + 1);
    if (destino) {
        strcpy(destino, origem);
    }
    return destino;
}

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
    
    // 1. Validação de campo obrigatório
    if (nome == NULL || strlen(nome) == 0) {
        printf("[ERRO] Nao e permitido cadastrar pessoa sem nome.\n");
        return 0; 
    }

    // 2. Verificação de duplicidade
    if (buscar_pessoa(*inicio, codigo) != NULL) {
        printf("[ERRO] Ja existe uma pessoa cadastrada com o codigo %d.\n", codigo);
        return -1;
    }

    // 3. Alocação do nó da lista
    Pessoa* novo = (Pessoa*) malloc(sizeof(Pessoa));
    if (novo == NULL) {
        printf("[ERRO] Falha de memoria ao alocar Pessoa.\n");
        return 0;
    }

    // 4. Atribuição de valores com tratamento de NULL
    novo->codigo = codigo;
    
    // Se for NULL, salva como ""
    novo->nome = alocar_string_segura(nome);
    novo->fone = alocar_string_segura(fone);
    novo->data_nascimento = alocar_string_segura(data);
    novo->endereco = alocar_string_segura(endereco);

    novo->prox = *inicio; 
    novo->ant = NULL;

    if (*inicio != NULL) {
        (*inicio)->ant = novo;
    }
    *inicio = novo; 

    printf("[SUCESSO] %s inserido(a) com codigo %d.\n", novo->nome, codigo);
    return 1; 
}

void carregar_pessoas_arquivo(Pessoa** inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "r");
    if (arq == NULL) {
        printf("Arquivo %s nao encontrado.\n", nome_arquivo);
        return;
    }

    char linha[500];
    while (fgets(linha, sizeof(linha), arq)) {
        linha[strcspn(linha, "\n")] = '\0';

        if (strlen(linha) < 2) continue;

        int cod;
        char *n, *f, *d, *e;
        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        cod = atoi(token);

        token = strtok(NULL, ";");
        n = token ? token : "";

        token = strtok(NULL, ";");
        f = token ? token : "";

        token = strtok(NULL, ";");
        d = token ? token : "";

        token = strtok(NULL, ";");
        e = token ? token : "";

        inserir_pessoa(inicio, cod, n, f, d, e);
    }

    fclose(arq);
    printf("Carga de pessoas finalizada com seguranca.\n");
}

// Retorna: 1 (Sucesso), 0 (Pessoa não encontrada)
int alterar_pessoa(Pessoa* inicio, int codigo, char* novo_nome, char* novo_fone, char* nova_data, char* novo_endereco) {
    
    // 1. Busca a pessoa na lista
    Pessoa* atual = buscar_pessoa(inicio, codigo);

    if (atual == NULL) {
        printf("[ERRO] Pessoa %d nao encontrada para alteracao.\n", codigo);
        return 0;
    }
    
    // 2. Atualiza NOME (Se foi passado no comando UPDATE)
    if (novo_nome != NULL && strlen(novo_nome) > 0) {
        if (atual->nome != NULL) free(atual->nome);
        
        atual->nome = (char*) malloc(strlen(novo_nome) + 1);
        if (atual->nome != NULL) {
            strcpy(atual->nome, novo_nome);
        }
    }

    // 3. Atualiza FONE
    if (novo_fone != NULL && strlen(novo_fone) > 0) {
        if (atual->fone != NULL) free(atual->fone);
        
        atual->fone = (char*) malloc(strlen(novo_fone) + 1);
        if (atual->fone != NULL) {
            strcpy(atual->fone, novo_fone);
        }
    }

    // 4. Atualiza DATA DE NASCIMENTO
    if (nova_data != NULL && strlen(nova_data) > 0) {
        if (atual->data_nascimento != NULL) free(atual->data_nascimento);
        
        atual->data_nascimento = (char*) malloc(strlen(nova_data) + 1);
        if (atual->data_nascimento != NULL) {
            strcpy(atual->data_nascimento, nova_data);
        }
    }

    // 5. Atualiza ENDEREÇO
    if (novo_endereco != NULL && strlen(novo_endereco) > 0) {
        if (atual->endereco != NULL) free(atual->endereco);
        
        atual->endereco = (char*) malloc(strlen(novo_endereco) + 1);
        if (atual->endereco != NULL) {
            strcpy(atual->endereco, novo_endereco);
        }
    }

    return 1;
}

// Retorna: 1 (Sucesso), 0 (Pessoa não encontrada)
int excluir_pessoa(Pessoa** inicio, int codigo, struct pet** lista_pets) {
    
    Pessoa* atual = buscar_pessoa(*inicio, codigo);

    if (atual == NULL) {
        printf("[ERRO] Pessoa %d nao encontrada para exclusao.\n", codigo);
        return 0;
    }

    // Removemos todos os pets vinculados a este código de pessoa
    int pets_removidos = excluir_pets_por_dono(lista_pets, codigo);
    if (pets_removidos > 0) {
        printf("[AVISO] %d pet(s) do dono %d foram removidos em cascata.\n", pets_removidos, codigo);
    }

    // Exclusão da pessoa da lista
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

void salvar_pessoas_arquivo(Pessoa* inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "w");
    if (arq == NULL) return;

    Pessoa* atual = inicio;
    while (atual != NULL) {

        fprintf(arq, "%d", atual->codigo);
        fprintf(arq, ";%s", atual->nome ? atual->nome : "");
        fprintf(arq, ";%s", atual->fone ? atual->fone : "");
        fprintf(arq, ";%s", atual->data_nascimento ? atual->data_nascimento : "");
        fprintf(arq, ";%s", atual->endereco ? atual->endereco : "");
        fprintf(arq, "\n");
        
        atual = atual->prox;
    }

    fclose(arq);
}

void listar_todas_pessoas(Pessoa* inicio) {
    if (inicio == NULL) {
        printf("Nenhuma pessoa cadastrada.\n");
        return;
    }

    Pessoa* p = inicio;
    printf("\n--- RELATORIO DE PESSOAS ---\n");
    while (p != NULL) {
        printf("ID: %-4d | Nome: %-20s | Fone: %s\n", p->codigo, p->nome, p->fone);
        p = p->prox;
    }
    printf("----------------------------\n");
}

void listar_pessoa(Pessoa* inicio, int codigo){
    Pessoa* atual = inicio;
    while (atual != NULL && atual->codigo != codigo) {
        atual = atual->prox;
    }
    if(atual == NULL){
        printf("Não existe pessoa com o codigo: %d para ser exibida", codigo);
    }else{
        printf("ID: %-4d | Nome: %-20s | Endereco: %-20s | Fone: %s\n", atual->codigo, atual->nome, atual->endereco, atual->fone);
    }
}