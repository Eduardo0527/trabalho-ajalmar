typedef enum { INSERT, UPDATE, DELETE, SELECT, INVALIDO } TipoOperacao;
typedef enum { PESSOA, PET, TIPO_PET, DESCONHECIDO } TabelaAlvo;


typedef struct {
    char linha_original[200]; 
    TipoOperacao operacao;    
    TabelaAlvo tabela;        
    int valido;               
} comando;


typedef struct NoComando {
    comando dado;
    struct noComando *prox;
} noComando;

typedef struct {
    noComando *inicio;
    noComando *fim;
} filaComandos;

void inicializar_fila(filaComandos *f);
void enfileirar(filaComandos *f, char* linha_lida);
int carregar_script(char *nome_arquivo, filaComandos *fila_geral);
void str_to_lower(char *dest, const char *src);
int analisar_comando(comando *cmd);
void processar_validacao_fila(filaComandos *f);