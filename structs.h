#include <stdio.h>

struct pessoa{
    int codigo;
    char* nome;
    char* fone;
    int data;
    char* endereco;
};

struct tipo_de_pet{
    int codigo;
    char* descricao;
};

struct pet{
    int codigo;
    int codigo_pes;
    char* nome;
    int codigo_tipo;
};