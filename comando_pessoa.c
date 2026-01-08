#include "comando_pessoa.h"


void executar_fila_pessoas(filaComandos *f_pessoas, Pessoa** lista_pessoa) {
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
                    // remover_pessoa(codigo);
                } 
                // Tratamento para "where codigo=3" (sem espaços)
                else if (sscanf(ptr_where, "where codigo=%d", &codigo) == 1) {
                     printf(" -> Removendo Pessoa Codigo: %d\n", codigo);
                     // remover_pessoa(codigo);
                }
            }
        }

        // --- CASO 3: SELECT ---
        else if (no->dado.operacao == SELECT) {
            // O PDF [cite: 36] exige tratamento especial para ORDER BY
            if (strstr(linha, "order by nome")) {
                printf(" -> Listando Pessoas (Ordenado por Arvore Binaria)\n");
                // listar_pessoas_arvore();
            } else {
                // Pode ser um select * simples ou com where
                char *ptr_where = strstr(linha, "where");
                if (ptr_where) {
                     if (sscanf(ptr_where, "where codigo = %d", &codigo) == 1) {
                         printf(" -> Buscando Pessoa Codigo: %d\n", codigo);
                         // buscar_pessoa(codigo);
                     }
                } else {
                    printf(" -> Listando Todas as Pessoas (Lista Linear)\n");
                    // listar_pessoas_simples();
                }
            }
        }

        // Limpeza final do nó processado
        free(no);
    }
}