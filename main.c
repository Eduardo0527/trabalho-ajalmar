#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipo_pet.h"
#include "pet.h"
#include "comando_pessoa.h"

void listar_tudo(Pessoa* l_pes, struct tipo_de_pet* l_tipos, struct pet* l_pets) {
    printf("\n--- PESSOAS ---\n");
    Pessoa* p = l_pes;
    while(p){ printf("ID: %d | Nome: %s\n", p->codigo, p->nome); p=p->prox;}
    
    printf("\n--- TIPOS DE PET ---\n");
    struct tipo_de_pet* t = l_tipos;
    while(t){ printf("ID: %d | Tipo: %s\n", t->codigo, t->nome); t=t->prox;}

    printf("\n--- PETS ---\n");
    struct pet* pt = l_pets;
    while(pt){ printf("ID: %d | Nome: %s | Dono ID: %d | Tipo ID: %d\n", pt->codigo, pt->nome, pt->codigo_pes, pt->codigo_tipo); pt=pt->prox;}
    printf("-----------------\n");
}

int main() {
    Pessoa* lista_pessoas = criar_lista_pessoa();
    struct tipo_de_pet* lista_tipos = criar_lista_tipo_pet();
    struct pet* lista_pets = criar_lista_pet();
    
    // Criação de arquivos de teste automáticos (se não existirem)
    FILE* file = fopen("pessoas.txt", "a"); if(file) fclose(file);
    file = fopen("tipos.txt", "a"); if(file) fclose(file);
    file = fopen("pets.txt", "a"); if(file) fclose(file);

    carregar_pessoas_arquivo(&lista_pessoas, "pessoas.txt");
    carregar_tipos_pet_arquivo(&lista_tipos, "tipos.txt");
    carregar_pets_arquivo(&lista_pets, "pets.txt");
    filaComandos f;
    filaComandos f_pessoa;
    filaComandos f_pet;
    filaComandos f_tipo;
    inicializar_fila(&f);
    inicializar_fila(&f_pessoa);
    inicializar_fila(&f_pet);
    inicializar_fila(&f_tipo);
    carregar_script("arquivo.txt", &f);
    processar_validacao_fila(&f);
    distribuir_comandos(&f,&f_pessoa,&f_pet,&f_tipo);
    executar_fila_pessoas(&f_pessoa, &lista_pessoas);
    
    return 0;
}