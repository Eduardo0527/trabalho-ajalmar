#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

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
    FILE* f = fopen("pessoas.txt", "a"); if(f) fclose(f);
    f = fopen("tipos.txt", "a"); if(f) fclose(f);
    f = fopen("pets.txt", "a"); if(f) fclose(f);

    carregar_pessoas_arquivo(&lista_pessoas, "pessoas.txt");
    carregar_tipos_pet_arquivo(&lista_tipos, "tipos.txt");
    carregar_pets_arquivo(&lista_pets, "pets.txt");

    int opcao = 0;
    while (opcao != 9) {
        printf("\n=== SISTEMA PET ===\n");
        printf("1. Listar Tudo\n");
        printf("2. Inserir Pessoa\n");
        printf("3. Inserir Tipo de Pet\n");
        printf("4. Inserir Pet (Valida Dono e Tipo)\n");
        printf("5. Excluir Pet\n");
        printf("6. Excluir Pessoa (Apaga os pets dela junto!)\n");
        printf("7. Excluir Tipo (So se nao tiver pets!)\n");
        printf("9. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            listar_tudo(lista_pessoas, lista_tipos, lista_pets);
        }
        else if (opcao == 2) {
            int id; char nome[50];
            printf("ID: "); scanf("%d", &id);
            printf("Nome: "); scanf(" %[^\n]", nome);
            if(inserir_pessoa(&lista_pessoas, id, nome, "0000", "01/01/2000", "Rua") == 1)
                printf("Sucesso.\n");
            else printf("Erro (ID duplicado ou vazio).\n");
        }
        else if (opcao == 3) {
            int id; char nome[50];
            printf("ID Tipo: "); scanf("%d", &id);
            printf("Descricao: "); scanf(" %[^\n]", nome);
            if(inserir_tipo_pet(&lista_tipos, id, nome) == 1) printf("Sucesso.\n");
            else printf("Erro.\n");
        }
        else if (opcao == 4) {
            int id, id_dono, id_tipo; char nome[50];
            printf("ID Pet: "); scanf("%d", &id);
            printf("ID Dono: "); scanf("%d", &id_dono);
            printf("ID Tipo: "); scanf("%d", &id_tipo);
            printf("Nome Pet: "); scanf(" %[^\n]", nome);

            if (buscar_pessoa(lista_pessoas, id_dono) == NULL) 
                printf("[ERRO] Dono nao encontrado.\n");
            else if (buscar_tipo_pet(lista_tipos, id_tipo) == NULL) 
                printf("[ERRO] Tipo nao encontrado.\n");
            else {
                if(inserir_pet(&lista_pets, id, id_dono, nome, id_tipo) == 1) printf("Sucesso.\n");
                else printf("Erro (ID duplicado).\n");
            }
        }
        else if (opcao == 5) {
            int id;
            printf("ID Pet: "); scanf("%d", &id);
            if(excluir_pet(&lista_pets, id)) printf("Pet removido.\n");
            else printf("Pet nao encontrado.\n");
        }
        else if (opcao == 6) {
            int id;
            printf("ID Pessoa: "); scanf("%d", &id);
            // 1. Cascata: Remove pets
            int pets_removidos = excluir_pets_por_dono(&lista_pets, id);
            // 2. Remove pessoa
            if(excluir_pessoa(&lista_pessoas, id)) 
                printf("Pessoa e seus %d pets foram removidos.\n", pets_removidos);
            else printf("Pessoa nao encontrada.\n");
        }
        else if (opcao == 7) {
            int id;
            printf("ID Tipo: "); scanf("%d", &id);
            // 1. Validacao: Bloqueia se tiver pets
            if(tipo_tem_pets(lista_pets, id)) {
                printf("[BLOQUEADO] Existem pets desse tipo. Remova-os antes.\n");
            } else {
                if(excluir_tipo_pet(&lista_tipos, id)) printf("Tipo removido.\n");
                else printf("Tipo nao encontrado.\n");
            }
        }
    }
    return 0;
}