#ifndef TIPO_PET_H
#define TIPO_PET_H

struct tipo_de_pet {
    int codigo;
    char* nome;
    struct tipo_de_pet* prox;
    struct tipo_de_pet* ant;
};

struct tipo_de_pet* criar_lista_tipo_pet();
struct tipo_de_pet* buscar_tipo_pet(struct tipo_de_pet* inicio, int codigo);
int inserir_tipo_pet(struct tipo_de_pet** inicio, int codigo, char* nome);

void carregar_tipos_pet_arquivo(struct tipo_de_pet** inicio, char* nome_arquivo);
void salvar_tipos_pet_arquivo(struct tipo_de_pet* inicio, char* nome_arquivo);
int excluir_tipo_pet(struct tipo_de_pet** inicio, int codigo);
int alterar_tipo_pet(struct tipo_de_pet* inicio, int codigo, char* novo_nome);
void listar_tipo_pet(struct tipo_de_pet *inicio, int codigo);
void listar_todos_tipos(struct tipo_de_pet* inicio);

#endif