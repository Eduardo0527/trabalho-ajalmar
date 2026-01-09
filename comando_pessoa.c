#include "comando_pessoa.h"


void executar_fila_pessoas(filaComandos *f_pessoas, Pessoa** lista_pessoa, struct pet** lista_pets) {
    printf("\n--- Executando Comandos de PESSOA ---\n");

    while (f_pessoas->inicio != NULL) {
        noComando *no = desenfileirar_no(f_pessoas);
        char *linha = no->dado.linha_original;
        
        // Variáveis para armazenar os dados extraídos
        int codigo;
        char nome[100], fone[50], data[20], endereco[100];

        // --- CASO 1: INSERT ---
        if (no->dado.operacao == INSERT) {
            // Estratégia: Achar onde começa o "values"
            char *ptr_values = strstr(linha, "values");
            
            if (ptr_values) {
                // Formato esperado: values(1, 'nome', 'fone', 'data', 'endereco')
                // Dica de sscanf: 
                // %d = inteiro
                // ' = caractere aspa literal
                // %[^'] = leia TUDO até encontrar uma aspa (lê espaços inclusive)
                
                // Limpar lixo das strings antes
                memset(nome, 0, 100); memset(fone, 0, 50); 
                memset(data, 0, 20); memset(endereco, 0, 100);

                // Tenta ler com 4 campos (endereço é opcional em alguns inserts?)
                // O PDF mostra exemplos variados. Vamos assumir o completo para o exemplo:
                int lidos = sscanf(ptr_values, "values (%d, '%[^']', '%[^']', '%[^']', '%[^']')", 
                                   &codigo, nome, fone, data, endereco);

                if (lidos >= 3) { // Se leu pelo menos código, nome e fone
                    printf(" -> Inserindo Pessoa: [%d] %s\n", codigo, nome);
                    // CHAMA A FUNÇÃO DO PARCEIRO
                    inserir_pessoa(lista_pessoa, codigo, nome, fone, data, endereco);
                    // inserir_pessoa(codigo, nome, fone, data, endereco);
                } else {
                    printf("Erro ao extrair dados do Insert: %s\n", linha);
                }
            }
        }

        // --- CASO 2: DELETE ---
        else if (no->dado.operacao == DELETE) {
            // Formato: delete from pessoa where codigo = 3;
            // Estratégia: Achar o "where"
            char *ptr_where = strstr(linha, "where");
            if (ptr_where) {
                // Tenta ler: where codigo = %d
                // O espaço no formato do sscanf ignora espaços reais múltiplos
                if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1) {
                    printf(" -> Removendo Pessoa Codigo: %d\n", codigo);
                    excluir_pessoa(lista_pessoa, codigo, lista_pets);
                    // remover_pessoa(codigo);
                } 
                // Tratamento para "where codigo=3" (sem espaços)
                else if (sscanf(ptr_where, "where codigo=%d", &codigo) == 1) {
                     printf(" -> Removendo Pessoa Codigo: %d\n", codigo);
                     excluir_pessoa(lista_pessoa, codigo, lista_pets);
                     // remover_pessoa(codigo);
                }
            }
        }

        // --- CASO 3: SELECT ---
        else if (no->dado.operacao == SELECT) {
            // O PDF [cite: 36] exige tratamento especial para ORDER BY
            if (strstr(linha, "order by nome")) {
                printf(" -> Listando Pessoas (Ordenado por Arvore Binaria)\n");
                //Função da árvore
                exibir_pessoas_ordenado_por_nome(*lista_pessoa);
            } else {
                // Pode ser um select * simples ou com where
                char *ptr_where = strstr(linha, "where");
                if (ptr_where) {
                     if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1) {
                         printf(" -> Buscando Pessoa Codigo: %d\n", codigo);
                         listar_pessoa(*lista_pessoa, codigo);
                         // buscar_pessoa(codigo);
                     }
                } else {
                    printf(" -> Listando Todas as Pessoas (Lista Linear)\n");
                    listar_todas_pessoas(*lista_pessoa);
                }
            }
        }
        // --- CASO 4: UPDATE ---
        else if (no->dado.operacao == UPDATE) {
            // Exemplo: update pessoa set nome = 'Novo Nome', fone = '123' where codigo = 1;
            
            char *ptr_set = strstr(linha, "set");
            char *ptr_where = strstr(linha, "where");

            if (ptr_set && ptr_where) {
                // 1. Extrair ID do WHERE (Fundamental)
                // Tenta com espaços " = " ou colado "="
                if (sscanf(ptr_where, "where codigo = %d", &codigo) != 1) {
                    if (sscanf(ptr_where, "where codigo=%d", &codigo) != 1) {
                         printf("Erro: Nao foi possivel ler o codigo no Update.\n");
                         codigo = -1; // Flag de erro
                    }
                }

                if (codigo != -1) {
                    // 2. Preparar variáveis (NULL indica que não houve alteração)
                    char *p_nome = NULL;
                    char *p_fone = NULL;
                    char *p_data = NULL;
                    char *p_endereco = NULL;

                    // Buffers temporários para armazenar o texto extraído
                    char temp_nome[100] = "";
                    char temp_fone[50] = "";
                    char temp_data[20] = "";
                    char temp_endereco[100] = "";

                    // 3. Varrer a string procurando cada campo
                    // Usamos 'loc' para localizar a posição do campo na string
                    char *loc;

                    // --- Parser do NOME ---
                    loc = strstr(linha, "nome");
                    // Verifica se achou E se está entre o 'set' e o 'where'
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "nome = '%[^']'", temp_nome) == 1 || 
                            sscanf(loc, "nome='%[^']'", temp_nome) == 1) {
                            p_nome = temp_nome; // Aponta para o buffer preenchido
                        }
                    }

                    // --- Parser do FONE ---
                    loc = strstr(linha, "fone");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "fone = '%[^']'", temp_fone) == 1 || 
                            sscanf(loc, "fone='%[^']'", temp_fone) == 1) {
                            p_fone = temp_fone;
                        }
                    }

                    // --- Parser da DATA ---
                    loc = strstr(linha, "data_nascimento");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "data_nascimento = '%[^']'", temp_data) == 1 || 
                            sscanf(loc, "data_nascimento='%[^']'", temp_data) == 1) {
                            p_data = temp_data;
                        }
                    }

                    // --- Parser do ENDERECO ---
                    loc = strstr(linha, "endereco");
                    if (loc && loc > ptr_set && loc < ptr_where) {
                        if (sscanf(loc, "endereco = '%[^']'", temp_endereco) == 1 || 
                            sscanf(loc, "endereco='%[^']'", temp_endereco) == 1) {
                            p_endereco = temp_endereco;
                        }
                    }

                    printf(" -> Atualizando Pessoa Codigo: %d\n", codigo);
                    
                    // 4. Chamar a função da dupla
                    // Passamos *lista_pessoa porque a função alterar_pessoa precisa do início da lista
                    // Passamos os ponteiros (p_nome, etc). Se for NULL, a função não altera aquele campo.
                    alterar_pessoa(*lista_pessoa, codigo, p_nome, p_fone, p_data, p_endereco);
                }
            } else {
                printf("Erro de Sintaxe Update: Faltou 'set' ou 'where'.\n");
            }
        }

        // Limpeza final do nó processado
        free(no);
    }
}
