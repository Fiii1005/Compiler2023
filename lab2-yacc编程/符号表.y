%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NSYMS 20
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror (const char* s);
struct symsign {
  char *name;
  double value;
} symsign[NSYMS];
//定义了一个可以存储20个符号的符号表结构
struct symsign* symlook(char *s);
int nsym=0;
char idname[20];
//存放变量名的数组
%}

%union{
    double dval;
    struct symsign *symp;
}
/*创建了一个联合体*/
/*可以根据需要选择存储双精度浮点数或指向符号表条目的指针*/

%token <symp> NAME
/*指定NAME的类型是指向符号表条目的指针*/
%token <dval> NUMBER
/*指定NUMBER的类型是双精度浮点数*/
/*声明词法符号*/
%token ADD
%token SUB
%token MUL
%token DIV
%token LB
%token RB
%left ADD SUB
%left MUL DIV
%right UMINUS
%type <dval> expr
/*指定非终结符expr的类型是双精度*/

%%
stmtlist    :   stmt ';'
            |   stmtlist stmt ';'
            ;

stmt        :   NAME '=' expr   {$1->value=$3;}
                /*标识符赋值*/
            |   expr            {printf("= %g\n", $1);}
                /*表达式*/
            ;

expr    :   expr ADD expr   {$$=$1+$3;}
            /*$$表示当前规则的结果值*/
        |   expr SUB expr   {$$=$1-$3;}
        |   expr MUL expr   {$$=$1*$3;}
        |   expr DIV expr   {if($3==0.0){
                                    yyerror("divided by zero.");
                                }
                            $$=$1/$3;}
        |   LB expr RB      {$$=$2;}
        |   SUB expr %prec UMINUS   {$$=-$2;}
            /*指定负号的优先级为UMINUS*/
        |   NUMBER          {$$=$1;}
        |   NAME            {$$=$1->value;}  
        ;

%%

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
            yylval.dval=0;
            /*yylval是yacc中隐式定义的变量，存储传递给语法分析器的值*/
            while(1){
                if(isdigit(t)){
                    yylval.dval=yylval.dval*10+t-'0';
                }
                else if(t==' '||t=='\t'||t=='\n'){
                    /*如果是空格、制表符、换行，跳过*/
                }
                else if((t>='a'&&t<='z')||(t>='A'&&t<='Z')||t=='_'){
                    yyerror("invalid id");
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
        else if((t>='a'&&t<='z')||(t>='A'&&t<='Z')||t=='_'){
            int ni=0;
            //变量长度
            while(1){
                if((t>='a'&&t<='z')||(t>='A'&&t<='Z')||t=='_'||isdigit(t)){
                    idname[ni]=t;
                }
                else{
                    break;
                }
                ni++;
                t=getchar();
            }
            ungetc(t,stdin);
            idname[ni]='\0';
            //返回标识符在符号表中的位置
            yylval.symp=symlook(idname);
            return NAME;
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
/*在符号表中操作的函数*/
struct symsign* symlook(char *s)
{
    int i;
    for(i=0;i<nsym;i++){
        if(!strcmp(symsign[i].name, s)){
            //如果标识符相等
            //返回该标识符地址
            return &symsign[i];
        }
    }
    //如果没有匹配到
    if(nsym<NSYMS){
        //如果符号数没有超出最大个数
        symsign[nsym].name=strdup(s);
        //将新标识符字符串复制到符号表空间
        symsign[nsym].value=0;
        nsym++;//符号个数++
        return &symsign[nsym-1];
    }
    //否则，符号表满了，不能装新的符号进去。
    yyerror("Too many symbols.");
    exit(1);
}