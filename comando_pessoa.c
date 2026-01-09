#include "comando_pessoa.h"


void executar_fila_pessoas(filaComandos *f_pessoas, Pessoa** lista_pessoa, struct pet** lista_pets) {
    printf("\n--- Executando Comandos de PESSOA ---\n");

    while (f_pessoas->inicio != NULL) {
        noComando *no = desenfileirar_no(f_pessoas);
        char *linha = no->dado.linha_original;
        
        int codigo;
        char nome[100], fone[50], data[20], endereco[100];

        // CASO 1: INSERT
        if (no->dado.operacao == INSERT) {
            char *ptr_values = strstr(linha, "values");
            
            if (ptr_values) {
                // Formato esperado: values(1, 'nome', 'fone', 'data', 'endereco')
                memset(nome, 0, 100); memset(fone, 0, 50); 
                memset(data, 0, 20); memset(endereco, 0, 100);

                int lidos = sscanf(ptr_values, "values (%d, '%[^']', '%[^']', '%[^']', '%[^']')", 
                                   &codigo, nome, fone, data, endereco);

                if (lidos >= 3) {
                    printf(" -> Inserindo Pessoa: [%d] %s\n", codigo, nome);
                    inserir_pessoa(lista_pessoa, codigo, nome, fone, data, endereco);
                } else {
                    printf("Erro ao extrair dados do Insert: %s\n", linha);
                }
            }
        }

        // CASO 2: DELETE
        else if (no->dado.operacao == DELETE) {
            // Formato: delete from pessoa where codigo = 3;
            char *ptr_where = strstr(linha, "where");
            if (ptr_where) {
                if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1) {
                    printf(" -> Removendo Pessoa Codigo: %d\n", codigo);
                    excluir_pessoa(lista_pessoa, codigo, lista_pets);
                } 
                else if (sscanf(ptr_where, "where codigo=%d", &codigo) == 1) {
                     printf(" -> Removendo Pessoa Codigo: %d\n", codigo);
                     excluir_pessoa(lista_pessoa, codigo, lista_pets);
                }
            }
        }

        // CASO 3: SELECT
        else if (no->dado.operacao == SELECT) {
            if (strstr(linha, "order by nome")) {
                printf(" -> Listando Pessoas (Ordenado por Arvore Binaria)\n");
                exibir_pessoas_ordenado_por_nome(*lista_pessoa);
            } else {
                char *ptr_where = strstr(linha, "where");
                if (ptr_where) {
                     if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1) {
                         printf(" -> Buscando Pessoa Codigo: %d\n", codigo);
                         listar_pessoa(*lista_pessoa, codigo);
                     }
                } else {
                    printf(" -> Listando Todas as Pessoas (Lista Linear)\n");
                    listar_todas_pessoas(*lista_pessoa);
                }
            }
        }
        // CASO 4: UPDATE
        else if (no->dado.operacao == UPDATE) {      
            char *ptr_set = strstr(linha, "set");
            char *ptr_where = strstr(linha, "where");

            if (ptr_set && ptr_where) {
                if (sscanf(ptr_where, "where codigo = %d", &codigo) != 1) {
                    if (sscanf(ptr_where, "where codigo=%d", &codigo) != 1) {
                         printf("Erro: Nao foi possivel ler o codigo no Update.\n");
                         codigo = -1;
                    }
                }

                if (codigo != -1) {
                    char *p_nome = NULL;
                    char *p_fone = NULL;
                    char *p_data = NULL;
                    char *p_endereco = NULL;

                    char temp_nome[100] = "";
                    char temp_fone[50] = "";
                    char temp_data[20] = "";
                    char temp_endereco[100] = "";
                    char *loc;

                    loc = strstr(linha, "nome");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "nome = '%[^']'", temp_nome) == 1 || 
                            sscanf(loc, "nome='%[^']'", temp_nome) == 1) {
                            p_nome = temp_nome;
                        }
                    }

                    loc = strstr(linha, "fone");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "fone = '%[^']'", temp_fone) == 1 || 
                            sscanf(loc, "fone='%[^']'", temp_fone) == 1) {
                            p_fone = temp_fone;
                        }
                    }

                    loc = strstr(linha, "data_nascimento");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "data_nascimento = '%[^']'", temp_data) == 1 || 
                            sscanf(loc, "data_nascimento='%[^']'", temp_data) == 1) {
                            p_data = temp_data;
                        }
                    }

                    loc = strstr(linha, "endereco");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "endereco = '%[^']'", temp_endereco) == 1 || 
                            sscanf(loc, "endereco='%[^']'", temp_endereco) == 1) {
                            p_endereco = temp_endereco;
                        }
                    }

                    printf(" -> Atualizando Pessoa Codigo: %d\n", codigo);
                    alterar_pessoa(*lista_pessoa, codigo, p_nome, p_fone, p_data, p_endereco);
                }
            } else {
                printf("Erro de Sintaxe Update: Faltou 'set' ou 'where'.\n");
            }
        }
        free(no);
    }
}
