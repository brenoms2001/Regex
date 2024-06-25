 /*
ALUNO: Breno Martins Silva
DRE: 122031418
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

enum RegExpTag{
    TAG_EMPTY,
    TAG_CHAR,
    TAG_STAR,
    TAG_CONCAT,
    TAG_UNION,
};

struct RegExp{
    char tag;
    union{
        struct{
            char c;
        } ch;
        struct{
            struct RegExp *filho;
        } un;
        struct{
            struct RegExp *filho1;
            struct RegExp *filho2;
        } bin;
    } u;
};
typedef struct RegExp RegExp;

// Funções de criação de nós
RegExp *new_empty();
RegExp *new_char(char c);
RegExp *new_star(RegExp *filho);
RegExp *new_concat(RegExp *filho1, RegExp *filho2);
RegExp *new_union(RegExp *filho1, RegExp *filho2);

// Funções de parse
static RegExp *parse_regexp();
static RegExp *parse_uniao();
static RegExp *parse_concat();
static RegExp *parse_estrela();
static RegExp *parse_basico();

// Funções para visualização da árvore
void print_tree(RegExp *tree, int nivel);

// Variável com o próximo dígito e a sua posicao
int c;
int pos;
int nParentesesAbertos = 0;

// Funcao para avancar o digito e a posicao simultaneamente
void proxDigito();

int main(void){
    proxDigito();

    while (c != EOF){
        RegExp *tree = parse_regexp();
        if (nParentesesAbertos > 0){
            fprintf(stderr, "Erro de sintaxe na posição %d: esperava ')', encontrei EOF\n", pos);
            exit(1);
        }
        // Nivel 0 porque precisa começar no ponto mais baixo, no caso a raiz
        print_tree(tree, 0);

        // Finalizar
        proxDigito();
    }
    return 0;
}

RegExp *new_empty(){
    RegExp *exp = (RegExp *)malloc(sizeof(RegExp));
    exp->tag = TAG_EMPTY;
    return exp;
}
RegExp *new_char(char c){
    RegExp *exp = (RegExp *)malloc(sizeof(RegExp));
    exp->tag = TAG_CHAR;
    exp->u.ch.c = c;
    return exp;
}
RegExp *new_star(RegExp *filho){
    RegExp *exp = (RegExp *)malloc(sizeof(RegExp));
    exp->tag = TAG_STAR;
    exp->u.un.filho = filho;
    return exp;
}
RegExp *new_concat(RegExp *filho1, RegExp *filho2){
    RegExp *exp = (RegExp *)malloc(sizeof(RegExp));
    exp->tag = TAG_CONCAT;
    exp->u.bin.filho1 = filho1;
    exp->u.bin.filho2 = filho2;
    return exp;
}
RegExp *new_union(RegExp *filho1, RegExp *filho2){
    RegExp *exp = (RegExp *)malloc(sizeof(RegExp));
    exp->tag = TAG_UNION;
    exp->u.bin.filho1 = filho1;
    exp->u.bin.filho2 = filho2;
    return exp;
}

static RegExp *parse_regexp(){
    return parse_uniao();
}
static RegExp *parse_uniao(){
    RegExp *e1 = parse_concat();
    if (c == '|'){
        proxDigito();
        RegExp *e2 = parse_uniao();
        e1 = new_union(e1, e2);
    }
    if ( c== ')' && nParentesesAbertos == 0){
        fprintf(stderr, "Erro de sintaxe na posição %d: esperava '\\n', encontrei ')'\n", pos);
        exit(1);        
    }    
    return e1;
}
static RegExp *parse_concat(){
    RegExp *e1 = parse_estrela();
    //Se c for um caracter normal
    if (c != '|' && c != '(' && c != ')' && c != '\n' && c != '\0' && c != EOF){
        RegExp *e2 = parse_concat();
        e1 = new_concat(e1, e2);
    }
    return e1;
}
static RegExp *parse_estrela(){
    RegExp *e = parse_basico();
    // Se os digitos seguintes forem estrelas
    while (c == '*'){
        proxDigito();
        e = new_star(e);
    }
    return e;
}
static RegExp *parse_basico(){
    RegExp *e = (RegExp *)malloc(sizeof(RegExp));
    //Se c for um caracter normal
    if (c != '|' && c != '(' && c != ')' && c != '\n' && c != '\0' && c != EOF){
        e = new_char(c);
        proxDigito();
    }
    //Se houver parenteses abrindo, chama uma nova expressao 
    else if (c == '('){
        proxDigito();
        nParentesesAbertos++;
        e = parse_regexp();
        // Fechando parênteses
        if (c == ')') {
            proxDigito();
            nParentesesAbertos--;
        }
        else if (c == '\n'){
            fprintf(stderr, "(a\nErro de sintaxe na posição %d: esperava ')', encontrei '\\n'\n", pos);
            exit(1);
        }
    }
    return e;
}
void proxDigito(){
    c = getchar();
    pos++;
}
void print_tree(RegExp *tree, int nivel){
    // Se a arvore estiver vazia, nao continue executando
    if (tree == NULL){
        return;
    }
    //Printa as identacoes
    for (int i = 0; i < nivel; i++){
        printf("  ");
    }

    switch (tree->tag){
        case TAG_EMPTY:
            printf("EMPTY\n");
            break;
        case TAG_CHAR:
            printf("CHAR %c\n", tree->u.ch.c);
            break;
        case TAG_STAR:
            printf("STAR\n");
            print_tree(tree->u.un.filho, nivel + 1);
            break;
        case TAG_CONCAT:
            printf("CONCAT\n");
            print_tree(tree->u.bin.filho1, nivel + 1);
            print_tree(tree->u.bin.filho2, nivel + 1);
            break;
        case TAG_UNION:
            printf("UNION\n");
            print_tree(tree->u.bin.filho1, nivel + 1);
            print_tree(tree->u.bin.filho2, nivel + 1);
            break;
    }
}