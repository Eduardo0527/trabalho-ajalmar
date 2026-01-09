#include "comando_tipo.h" // Header com a struct TipoPet



void executar_fila_tipos(filaComandos *f_tipos, struct tipo_de_pet** lista_tipo) {
    printf("\n--- Executando Comandos de TIPO_PET ---\n");

    while (f_tipos->inicio != NULL) {
        noComando *no = desenfileirar_no(f_tipos);
        char *linha = no->dado.linha_original;
        
        // Variáveis
        int codigo;
        char descricao[100];

        // --- CASO 1: INSERT ---
        if (no->dado.operacao == INSERT) {
            // Exemplo PDF: insert into tipo_pet(codigo, descricao) values(1, 'cachorro');
            
            char *ptr_values = strstr(linha, "values");
            
            if (ptr_values) {
                memset(descricao, 0, 100);
                
                // Formato: values(int, 'string')
                int lidos = sscanf(ptr_values, "values (%d, '%[^']')", &codigo, descricao);

                // Tentativa sem espaços se falhar
                if (lidos < 2) {
                     lidos = sscanf(ptr_values, "values(%d,'%[^']')", &codigo, descricao);
                }

                if (lidos == 2) {
                    printf(" -> Inserindo TipoPet: [%d] %s\n", codigo, descricao);
                    inserir_tipo_pet(lista_tipo, codigo, descricao);
                } else {
                    printf("Erro ao extrair dados do Insert TipoPet: %s\n", linha);
                }
            }
        }

        // --- CASO 2: DELETE ---
        else if (no->dado.operacao == DELETE) {
            // Formato: delete from tipo_pet where codigo = 1;
            char *ptr_where = strstr(linha, "where");
            if (ptr_where) {
                if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1 || 
                    sscanf(ptr_where, "where codigo=%d", &codigo) == 1) {
                    
                    printf(" -> Removendo TipoPet Codigo: %d\n", codigo);
                    excluir_tipo_pet(lista_tipo, codigo);
                } 
            }
        }

        // --- CASO 3: SELECT ---
        else if (no->dado.operacao == SELECT) {
            // PDF pede árvore binária no 'order by' [cite: 36]
            // Aceita 'order by nome' (texto do pdf) ou 'order by descricao' (logica do campo)
            if (strstr(linha, "order by nome") || strstr(linha, "order by descricao")) {
                printf(" -> Listando Tipos (Ordenado por Arvore Binaria)\n");
                exibir_tipos_ordenado_por_nome(*lista_tipo);
            } else {
                char *ptr_where = strstr(linha, "where");
                if (ptr_where) {
                     if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1) {
                         printf(" -> Buscando TipoPet Codigo: %d\n", codigo);
                         listar_tipo_pet(*lista_tipo, codigo);
                     }
                } else {
                    printf(" -> Listando Todos os Tipos (Lista Linear)\n");
                    listar_todos_tipos(*lista_tipo);
                }
            }
        }

        // --- CASO 4: UPDATE ---
        else if (no->dado.operacao == UPDATE) {
            // Exemplo: update tipo_pet set descricao = 'Felino' where codigo = 2;
            
            char *ptr_set = strstr(linha, "set");
            char *ptr_where = strstr(linha, "where");

            if (ptr_set && ptr_where) {
                // 1. Ler ID
                if (sscanf(ptr_where, "where codigo = %d", &codigo) != 1) {
                     if (sscanf(ptr_where, "where codigo=%d", &codigo) != 1) codigo = -1;
                }

                if (codigo != -1) {
                    char *p_desc = NULL;
                    char temp_desc[100] = "";
                    char *loc;

                    // 2. Parser da DESCRICAO (único campo alterável além do ID)
                    // O PDF chama de 'nome' na lista de variáveis mas 'descricao' no script.
                    // Vamos tentar ler ambos para garantir.
                    
                    loc = strstr(linha, "descricao");
                    if (!loc) loc = strstr(linha, "nome"); // Fallback

                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "descricao = '%[^']'", temp_desc) == 1 || 
                            sscanf(loc, "descricao='%[^']'", temp_desc) == 1 ||
                            sscanf(loc, "nome = '%[^']'", temp_desc) == 1) {
                            p_desc = temp_desc;
                        }
                    }

                    printf(" -> Atualizando TipoPet Codigo: %d\n", codigo);
                    // Só chama se realmente houve algo para alterar
                    if (p_desc != NULL) {
                        alterar_tipo_pet(*lista_tipo, codigo, p_desc);
                    } else {
                        printf("Aviso: Nenhum campo valido identificado para alteracao.\n");
                    }
                }
            } else {
                printf("Erro de Sintaxe Update TipoPet.\n");
            }
        }

        free(no);
    }
}