#include <stddef.h>

#ifdef _WIN32
    #define strncasecmp _strnicmp
#endif

typedef enum {INVALIDO = 0, INSERT, UPDATE, DELETE, SELECT} TipoOperacao;
typedef enum {DESCONHECIDO = 0, PESSOA, PET, TIPO_PET} TabelaAlvo;


typedef struct {
    char linha_original[200]; 
    TipoOperacao operacao;    
    TabelaAlvo tabela;        
    int valido;               
} comando;


typedef struct noComando {
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
char* pular_espacos(char *str);
int minha_strncasecmp(const char *s1, const char *s2, size_t n);
char* verificar_inicio(char *str, const char *prefixo);
int verificar_tabela(char **cursor);
char* verificar_qualquer_campo(char *cursor, TabelaAlvo t);
int analisar_comando_estrito(comando *cmd);
noComando *desenfileirar_no(filaComandos *f);
void processar_validacao_fila(filaComandos *f);
void enfileirar_no_existente(filaComandos *f, noComando *no);
void distribuir_comandos(filaComandos *f_geral, filaComandos *f_pessoa, filaComandos *f_pet, filaComandos *f_tipo);
