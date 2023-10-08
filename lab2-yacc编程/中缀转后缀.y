%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef YYSTYPE
#define YYSTYPE char*
#endif

char numstr[50];
/*用来存放数字数组*/
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
int isdigit(int a);
%}

%token ADD
%token SUB
%token MUL
%token DIV
%token LB
%token RB
%token NUMBER
%left ADD SUB
%left MUL DIV
%right UMINUS

%%
lines   :   lines expr ';'{printf("%s\n",$2);}
        |   lines ';'
        |
        ;

expr    :   expr ADD expr   {   $$=(char*)malloc(50*sizeof(char));
                                /*当前规则结果返回一个指向字符数组的指针*/
                                strcpy($$, $1);
                                strcat($$, $3);
                                strcat($$, "+ ");
                                /*结果为expr1 expr2 + */}
        |   expr SUB expr   {   $$=(char*)malloc(50*sizeof(char));
                                strcpy($$, $1);
                                strcat($$, $3);
                                strcat($$, "- ");
                            }
        |   expr MUL expr   {   $$=(char*)malloc(50*sizeof(char));
                                strcpy($$, $1);
                                strcat($$, $3);
                                strcat($$, "* ");
                            }
        |   expr DIV expr   {   if($3[0]=='0'){
                                yyerror("divided by zero.");
                                }
                                $$=(char*)malloc(50*sizeof(char));
                                strcpy($$, $1);
                                strcat($$, $3);
                                strcat($$, "/ ");
                            }
        |   LB expr RB      {$$=$2;}
        |   SUB expr %prec UMINUS   {   $$=(char*)malloc(50*sizeof(char));
                                        strcpy($$, $2);
                                        strcat($$,"- ");
                                    }
        |   NUMBER                  {   $$=(char*)malloc(50*sizeof(char));
                                        strcpy($$, $1);
                                        strcat($$, " ");
                                    }
        ;
%%

int yylex()
{
    int t;
    while(1){
        t=getchar();
        if(t==' '||t=='\t'||t=='\n'){

        }
        else if(isdigit(t)){
            int ni=0;
            /*用来记录数组下标*/
            while(1){
                if(isdigit(t)){
                    numstr[ni]=t;
                    /*数字数组增长*/
                }
                else if(t==' '||t=='\t'||t=='\n'){
                    /*跳过*/
                }
                else{
                    break;
                }
                ni+=1;
                t=getchar();
            }
            ungetc(t, stdin);
            numstr[ni]='\0';
            yylval=numstr;
            return NUMBER;
        }
        else if(t=='+'){
            return ADD;
        }
        else if(t=='-'){
            return SUB;
        }
        else if(t=='*'){
            return MUL;
        }
        else if(t=='/'){
            return DIV;
        }
        else if(t=='('){
            return LB;
        }
        else if(t==')'){
            return RB;
        }
        else{
            return t;
        }
    }
}

int main(void){
    yyin=stdin;
    /*程序从键盘输入读取数据进行语法分析*/
    do{
        yyparse();
    }while(!feof(yyin));
    /*调用yyparse()进行解析，直至末尾*/
    return 0;
}
void yyerror(const char* s){
    fprintf(stderr, "Parse error: %s\n",s);
    /*输出报错字符串*/
    exit(1);
}
int isdigit(int a){
    if(a>='0'&&a<='9'){
        return 1;
    }
    return 0;
}