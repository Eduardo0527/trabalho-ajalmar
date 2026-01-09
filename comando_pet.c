#include "comando_pet.h" // Ou o header onde estão as structs de Pet

void executar_fila_pets(filaComandos *f_pets, struct pet** lista_pet, struct tipo_de_pet* lista_tipos) {
    printf("\n--- Executando Comandos de PET ---\n");

    while (f_pets->inicio != NULL) {
        noComando *no = desenfileirar_no(f_pets);
        char *linha = no->dado.linha_original;
        
        // Variáveis para armazenar os dados extraídos
        // Campos do Pet: codigo, codigo_pes (dono), nome, codigo_tipo
        int codigo;
        int codigo_pes, codigo_tipo;
        char nome[100];

        // --- CASO 1: INSERT ---
        if (no->dado.operacao == INSERT) {
            // Exemplo PDF: insert into pet(codigo, codigo_cli, nome, codigo_tipo) values(1, 1, 'bilu', 2);
            // Nota: O PDF usa "codigo_cli" no insert, mas a struct chama "codigo_pes".
            // Vamos focar nos valores dentro do values().
            
            char *ptr_values = strstr(linha, "values");
            
            if (ptr_values) {
                // Limpar string
                memset(nome, 0, 100); 
                
                // Formato esperado: values(int, int, 'string', int)
                // values (%d, %d, '%[^']', %d)
                
                int lidos = sscanf(ptr_values, "values (%d, %d, '%[^']', %d)", 
                                   &codigo, &codigo_pes, nome, &codigo_tipo);

                // O sscanf as vezes é sensível a espaços. 
                // Se falhar, tentamos formato sem espaços após a virgula: values(%d,%d,'%[^']',%d)
                if (lidos < 4) {
                     lidos = sscanf(ptr_values, "values(%d,%d,'%[^']',%d)", 
                                   &codigo, &codigo_pes, nome, &codigo_tipo);
                }

                if (lidos == 4) {
                    printf(" -> Inserindo Pet: [%d] %s (Dono: %d, Tipo: %d)\n", codigo, nome, codigo_pes, codigo_tipo);
                    
                    // CHAMA A FUNÇÃO DO PARCEIRO
                    inserir_pet(lista_pet, codigo, codigo_pes, nome, codigo_tipo, lista_tipos);
                } else {
                    printf("Erro ao extrair dados do Insert Pet: %s\n", linha);
                }
            }
        }

        // --- CASO 2: DELETE ---
        else if (no->dado.operacao == DELETE) {
            // Formato: delete from pet where codigo = 3;
            char *ptr_where = strstr(linha, "where");
            if (ptr_where) {
                if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1 || 
                    sscanf(ptr_where, "where codigo=%d", &codigo) == 1) {
                    
                    printf(" -> Removendo Pet Codigo: %d\n", codigo);
                    excluir_pet(lista_pet, codigo);
                } 
            }
        }

        // --- CASO 3: SELECT ---
        else if (no->dado.operacao == SELECT) {
            // Verifica Order By (Árvore Binária)
            if (strstr(linha, "order by nome")) {
                printf(" -> Listando Pets (Ordenado por Arvore Binaria)\n");
                exibir_pets_ordenado_por_nome(*lista_pet);
            } else {
                // Select Simples ou Busca
                char *ptr_where = strstr(linha, "where");
                if (ptr_where) {
                     if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1) {
                         printf(" -> Buscando Pet Codigo: %d\n", codigo);
                         listar_pet(*lista_pet, codigo);
                     }
                } else {
                    printf(" -> Listando Todos os Pets (Lista Linear)\n");
                    listar_todos_pets(*lista_pet);
                }
            }
        }

        // --- CASO 4: UPDATE ---
        else if (no->dado.operacao == UPDATE) {
            // Exemplo: update pet set nome = 'Rex', codigo_tipo = 2 where codigo = 1;
            
            char *ptr_set = strstr(linha, "set");
            char *ptr_where = strstr(linha, "where");

            if (ptr_set && ptr_where) {
                // 1. Extrair ID
                if (sscanf(ptr_where, "where codigo = %d", &codigo) != 1) {
                    if (sscanf(ptr_where, "where codigo=%d", &codigo) != 1) {
                         codigo = -1; 
                    }
                }

                if (codigo != -1) {
                    // 2. Preparar variáveis. 
                    // Para inteiros opcionais, usamos ponteiros para saber se foram setados ou não.
                    char *p_nome = NULL;
                    int *p_cod_pes = NULL;
                    int *p_cod_tipo = NULL;

                    char temp_nome[100] = "";
                    int temp_cod_pes;
                    int temp_cod_tipo;

                    char *loc;

                    // --- Parser do NOME ---
                    loc = strstr(linha, "nome");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "nome = '%[^']'", temp_nome) == 1 || 
                            sscanf(loc, "nome='%[^']'", temp_nome) == 1) {
                            p_nome = temp_nome;
                        }
                    }

                    // --- Parser do CODIGO_PES (Dono) ---
                    // O PDF as vezes usa codigo_cli ou codigo_pes, verifique o padrão do script
                    loc = strstr(linha, "codigo_cli"); // Tentativa 1
                    if (!loc) loc = strstr(linha, "codigo_pes"); // Tentativa 2

                    if (loc && loc > ptr_set && loc < ptr_where) {
                        // Tenta ler "codigo_cli = 10"
                        // sscanf ignora o texto "codigo_cli =" se usarmos o formato correto ou pularmos
                        // Mas o jeito mais seguro é ler o numero logo apos o sinal de igual
                        char *igual = strchr(loc, '=');
                        if (igual && igual < ptr_where) {
                            if (sscanf(igual + 1, "%d", &temp_cod_pes) == 1) {
                                p_cod_pes = &temp_cod_pes;
                            }
                        }
                    }

                    // --- Parser do CODIGO_TIPO ---
                    loc = strstr(linha, "codigo_tipo");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        char *igual = strchr(loc, '=');
                        if (igual && igual < ptr_where) {
                            if (sscanf(igual + 1, "%d", &temp_cod_tipo) == 1) {
                                p_cod_tipo = &temp_cod_tipo;
                            }
                        }
                    }

                    printf(" -> Atualizando Pet Codigo: %d\n", codigo);
                    
                    // 4. Chamar a função da dupla
                    // A assinatura de alterar_pet deve aceitar ponteiros para int para saber o que mudar
                    alterar_pet(*lista_pet, codigo, p_nome, p_cod_pes, p_cod_tipo);
                }
            } else {
                printf("Erro de Sintaxe Update Pet: Faltou 'set' ou 'where'.\n");
            }
        }

        // Limpeza final do nó processado
        free(no);
    }
}