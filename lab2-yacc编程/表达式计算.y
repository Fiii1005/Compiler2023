%{
    /*定义部分*/
    #include <stdio.h>
    #include <stdlib.h>
    /*定义语法分析树中的数据类型*/
    #ifndef YYSTYPE
    #define YYSTYPE double
    #endif
    int yylex();
    extern int yyparse();
    FILE* yyin;
    void yyerror (const char* s);
    int isdigit(int a);
%}

/*声明词法符号*/
%token ADD
%token SUB
%token MUL
%token DIV
%token LB
%token RB
%token NUMBER

/*先定义的优先级较低*/
/*left表示左结合，right表示右结合*/
%left ADD SUB
%left MUL DIV
%right UMINUS

/*规则部分*/
%%

lines   :   lines expr ';' {printf("%f\n", $2);}
            /*$用于将不同节点的值传递给后续的计算或操作*/
            /*打印expr表达式的值和一个回车*/
            /*识别文字改为;结尾*/
        |   lines ';'
        |
        ;

expr    :   expr ADD expr   {$$=$1+$3;}
            /*$$表示当前规则的结果值*/
        |   expr SUB expr   {$$=$1-$3;}
        |   expr MUL expr   {$$=$1*$3;}
        |   expr DIV expr   {   if($3==0.0){
                                    yyerror("divided by zero.");
                            }
                                $$=$1/$3;}
        |   LB expr RB      {$$=$2;}
        |   SUB expr %prec UMINUS   {$$=-$2;}
            /*指定负号的优先级为UMINUS*/
        |   NUMBER          {$$=$1;}
        ;
/*
NUMBER  :   '0'     {$$=0.0;}
        |   '1'     {$$=1.0;}
        |   '2'     {$$=2.0;}
        |   '3'     {$$=3.0;}
        |   '4'     {$$=4.0;}
        |   '5'     {$$=5.0;}
        |   '6'     {$$=6.0;}
        |   '7'     {$$=7.0;}
        |   '8'     {$$=8.0;}
        |   '9'     {$$=9.0;}
        ;*/
%%

/*用户子程序*/

int yylex()
{
    int t;
    while(1){
        t=getchar();
        /*不停读取输入,返回int类型的值*/
        if(t==' '||t=='\t'||t=='\n'){
            /*忽略空格、制表符、换行*/
        }
        else if(isdigit(t)){
            /*isdigit判断ascii码，如果读取到的是数字*/
            yylval=0;
            /*yylval是yacc中隐式定义的变量，存储传递给语法分析器的值*/
            while(1){
                if(isdigit(t)){
                    yylval=yylval*10+t-'0';
                }
                else if(t==' '||t=='\t'||t=='\n'){
                    /*如果是空格、制表符、换行，跳过*/
                }
                else{
                    break;
                }
                t=getchar();
                /*t读取下一个字符*/
            }
            ungetc(t,stdin);
            /*上面循环的时候把t取出了，把t放回去防止影响后续读取*/
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

int main(void)
{
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