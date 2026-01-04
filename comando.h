typedef enum {INSERT, UPDATE, DELETE, SELECT, INVALIDO } TipoOperacao;
typedef enum {PESSOA, PET, TIPO_PET, DESCONHECIDO} FilaAlvo;

typedef struct{
    char linha_original[200];
    TipoOperacao operacao;
    FilaAlvo filaAlvo;
    int valido;
} comando;

typedef struct noComando{
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