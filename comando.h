typedef enum { INSERT, UPDATE, DELETE, SELECT, INVALIDO } TipoOperacao;
typedef enum { PESSOA, PET, TIPO_PET, DESCONHECIDO } TabelaAlvo;


typedef struct {
    char linha_original[200]; 
    TipoOperacao operacao;    
    TabelaAlvo tabela;        
    int valido;               
} comando;

// Nó da fila de comandos
typedef struct NoComando {
    comando dado;
    struct noComando *prox;
} noComando;

// A estrutura da Fila em si
typedef struct {
    noComando *inicio;
    noComando *fim;
} filaComandos;