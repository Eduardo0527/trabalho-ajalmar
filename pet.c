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
    
    // 1. Verifica duplicidade do ID do Pet
    if (buscar_pet(*inicio, codigo) != NULL) {
        printf("[ERRO] Pet com codigo %d ja existe.\n", codigo);
        return -1;
    }

    // 2. [NOVO] Verifica se o Tipo de Pet existe
    if (buscar_tipo_pet(lista_tipos, codigo_tipo) == NULL) {
        printf("[ERRO] Tipo de Pet %d nao existe. Cadastro negado.\n", codigo_tipo);
        return 0; // Falha de validação
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

// [IMPORTANTE] A assinatura mudou: agora recebe lista_tipos
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

    // Formato: CODIGO;COD_PESSOA;NOME;COD_TIPO
    while (fscanf(arq, "%d;%d;%[^;];%d\n", &cod, &cod_pes, n, &cod_tipo) != EOF) {
        
        // Passamos a lista_tipos para que o inserir_pet valide se o tipo existe
        int status = inserir_pet(inicio, cod, cod_pes, n, cod_tipo, lista_tipos);
        
        if (status == 1) {
            total_carregados++;
        } else {
            // Se status for 0, provavelmente o tipo não existe ou nome é nulo
            printf("[AVISO CARGA] Pet '%s' (ID %d) ignorado. Motivo: Tipo %d invalido ou erro de dados.\n", n, cod, cod_tipo);
            total_ignorados++;
        }
    }

    fclose(arq);
    printf("Carga de Pets finalizada: %d carregados, %d ignorados.\n", total_carregados, total_ignorados);
}

// Exclui um único pet
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

// Exclui todos os pets de um dono (para a cascata)
int excluir_pets_por_dono(struct pet** inicio, int codigo_pes) {
    struct pet* atual = *inicio;
    int cont = 0;
    while (atual != NULL) {
        struct pet* prox_temp = atual->prox; // Guarda o próximo antes de deletar
        if (atual->codigo_pes == codigo_pes) {
            excluir_pet(inicio, atual->codigo);
            cont++;
        }
        atual = prox_temp;
    }
    return cont;
}

// Verifica se um tipo está sendo usado (para o bloqueio)
int tipo_tem_pets(struct pet* inicio, int codigo_tipo) {
    struct pet* atual = inicio;
    while (atual != NULL) {
        if (atual->codigo_tipo == codigo_tipo) return 1;
        atual = atual->prox;
    }
    return 0;
}

// Assinatura atualizada para aceitar ponteiros nos campos opcionais e o novo campo tipo
int alterar_pet(struct pet* inicio, int codigo, char* novo_nome, int* novo_dono_id, int* novo_tipo_id) {
    
    // 1. Busca o Pet na lista
    struct pet* atual = buscar_pet(inicio, codigo);

    if (atual == NULL) {
        printf("[ERRO] Pet %d nao encontrado para alteracao.\n", codigo);
        return 0;
    }

    // 2. Alterar Nome (Verifica se não é NULL e se não está vazio)
    if (novo_nome != NULL && strlen(novo_nome) > 0) {
        // Libera memória antiga se existir (boa prática)
        if (atual->nome != NULL) {
            free(atual->nome);
        }
        
        // Aloca nova memória e copia
        atual->nome = (char*) malloc(strlen(novo_nome) + 1);
        if (atual->nome != NULL) {
            strcpy(atual->nome, novo_nome);
        }
    }

    // 3. Alterar Dono
    // Recebemos um ponteiro (int*). Se for NULL, significa que o comando SQL 
    // não tinha "set codigo_pes = ...", então ignoramos.
    if (novo_dono_id != NULL) {
        // Desreferenciamos o ponteiro (*) para pegar o valor inteiro
        // OBS: O PDF  pede validação se esse ID existe. 
        // Idealmente, você verificaria isso antes de atribuir.
        atual->codigo_pes = *novo_dono_id;
    }

    // 4. Alterar Tipo (Novo campo necessário)
    if (novo_tipo_id != NULL) {
        // Mesma lógica: só altera se o ponteiro for válido
        atual->codigo_tipo = *novo_tipo_id;
    }

    return 1; // Sucesso
}

// --- FUNÇÃO DE SALVAR (PERSISTÊNCIA) ---
void salvar_pets_arquivo(struct pet* inicio, char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "w"); // "w" apaga o anterior e escreve o novo
    
    if (arq == NULL) {
        printf("[ERRO CRITICO] Nao foi possivel salvar em %s\n", nome_arquivo);
        return;
    }

    struct pet* atual = inicio;
    while (atual != NULL) {
        // Formato: CODIGO;COD_DONO;NOME;COD_TIPO
        fprintf(arq, "%d;%d;%s;%d\n", 
                atual->codigo, 
                atual->codigo_pes, 
                atual->nome, 
                atual->codigo_tipo);
        
        atual = atual->prox;
    }

    fclose(arq);
    // printf("Dados dos pets salvos com sucesso.\n");
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