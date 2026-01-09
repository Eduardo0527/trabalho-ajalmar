#include "tipo_pet.h"
#ifndef PET_H
#define PET_H


struct pet {
    int codigo;
    int codigo_pes;
    char* nome;
    int codigo_tipo; 
    struct pet* prox;
    struct pet* ant;
};


struct pet* criar_lista_pet();
struct pet* buscar_pet(struct pet* inicio, int codigo);
int inserir_pet(struct pet** inicio, int codigo, int codigo_pes, char* nome, int codigo_tipo, struct tipo_de_pet* lista_tipos);
void carregar_pets_arquivo(struct pet** inicio, char* nome_arquivo, struct tipo_de_pet* tipos);

int excluir_pet(struct pet** inicio, int codigo);
int excluir_pets_por_dono(struct pet** inicio, int codigo_pes);
int tipo_tem_pets(struct pet* inicio, int codigo_tipo);
int alterar_pet(struct pet* inicio, int codigo, char* novo_nome, int* novo_dono_id, int* novo_tipo_id);
void salvar_pets_arquivo(struct pet* inicio, char* nome_arquivo);
void listar_pet(struct pet* inicio, int codigo);
void listar_todos_pets(struct pet* inicio);

#endif