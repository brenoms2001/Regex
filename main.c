/*
ALUNO: Breno Martins Silva
DRE: 122031418
*/

//Cabecalho
#include <stdio.h>
#include <stdlib.h>
#define MAX 100

//Structs
enum RegExpTag {
    TAG_EMPTY,
    TAG_CHAR,
    TAG_STAR,
    TAG_CONCAT,
    TAG_UNION,
};

struct RegExp {
    char tag;
    union {
        struct {
            char c;
        } ch;
        struct {
            struct RegExp *filho;
        } un;
        struct {
            struct RegExp *filho1;
            struct RegExp *filho2;
        } bin;
    } u;
};
typedef struct RegExp RegExp;

//Funcoes de criacao de nos
RegExp *new_empty();
RegExp *new_char(char c);
RegExp *new_star(RegExp *filho);
RegExp *new_concat(RegExp *filho1, RegExp *filho2);
RegExp *new_union(RegExp *filho1, RegExp *filho2);

//funcoes de parse
static RegExp *parse_regexp();
static RegExp *parse_uniao();
static RegExp *parse_concat();
static RegExp *parse_estrela();
static RegExp *parse_basico();

//Funcoes de criacao e conversao de regex para arvores
RegExp *regexToTree();
void printTree(RegExp);

int c;

int main(void){
    char expressao[MAX];
    scanf("%s", expressao);

    RegExp *arvore = malloc(sizeof(RegExp)); 
    arvore->tag = TAG_UNION;

    RegExp *conc = malloc(sizeof(RegExp));;
    conc->tag = TAG_CONCAT;

    RegExp *char1 = malloc(sizeof(RegExp));;
    char1->tag = TAG_CHAR;
    char1->u.ch.c = 'a';
    RegExp *char2 = malloc(sizeof(RegExp));;
    char2->tag = TAG_CHAR;
    char2->u.ch.c = 'b';

    arvore->u.bin.filho1 = conc;
    conc->u.bin.filho1 = char1;
    conc->u.bin.filho2 = char2;

    RegExp *char3 = malloc(sizeof(RegExp));;
    char3->tag = TAG_CHAR;
    char3->u.ch.c = 'c';
    arvore->u.bin.filho2 = char3;

    return 0;
}

RegExp *new_empty(){
    RegExp *exp = malloc(sizeof(RegExp));
    exp->tag = TAG_EMPTY;
    return exp;
};
RegExp *new_char(char c){
    RegExp *exp = malloc(sizeof(RegExp));
    exp->tag=TAG_CHAR;
    exp->u.ch.c = c;
    return exp;
};
RegExp *new_star(RegExp *filho){
    RegExp *exp = malloc(sizeof(RegExp));
    exp->tag = TAG_STAR;
    exp->u.un.filho = filho;
    return exp;
};
RegExp *new_concat(RegExp *filho1, RegExp *filho2){
    RegExp *exp = malloc(sizeof(RegExp));
    exp->tag = TAG_CONCAT;
    exp->u.bin.filho1 = filho1;
    exp->u.bin.filho2 = filho2;
    return exp;
};
RegExp *new_union(RegExp *filho1, RegExp *filho2){
    RegExp *exp = malloc(sizeof(RegExp));
    exp->tag = TAG_UNION;
    exp->u.bin.filho1 = filho1;
    exp->u.bin.filho2 = filho2;
    return exp;
};
static RegExp *parse_regexp();
static RegExp *parse_uniao();
static RegExp *parse_concat();
static RegExp *parse_estrela();
static RegExp *parse_basico();
RegExp *regexToTree();
void printTree(RegExp);
