%code requires {
  #include <memory>
  #include <string>
  #include "ast.h"
  #include <utility>
}

%{
#include "ast.h"
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <cstring>
// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<BaseAST> &ast }
%define parse.error verbose
// yylval 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>?
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况
%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
  std::vector<std::unique_ptr<BaseAST>> *vec_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN LOR LAND GEQ EQ NEQ LEQ CONST IF ELSE WHIHE BREAK CONTINUE VOID
%token <str_val> IDENT
%token <int_val> INT_CONST

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt Number 
%type <ast_val> Exp UnaryExp PrimaryExp MulExp AddExp LOrExp LAndExp EqExp RelExp
%type <ast_val> Decl ConstDecl BType ConstDef ConstInitVal BlockItem LVal ConstExp 
%type <ast_val> VarDecl VarDef InitVal 
%type <ast_val> FuncFParam CompDef 
%type <vec_val> BlockItems MulConstDef MulVarDef FuncFParams FuncRParams CompDefs
%type <str_val> UnaryOp AddOp MulOp EqOp RelOp
//Number已被修改

//定义优先级
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%

// 开始符, CompUnit ::= FuncDef, 大括号后声明了解析完成后 parser 要做的事情
// 之前我们定义了 FuncDef 会返回一个 str_val, 也就是字符串指针
// 而 parser 一旦解析完 CompUnit, 就说明所有的 token 都被解析了, 即解析结束了
// 此时我们应该把 FuncDef 返回的结果收集起来, 作为 AST 传给调用 parser 的函数
// $1 指代规则里第一个符号的返回值, 也就是 FuncDef 的返回值
// 由于compunit需要重复，所以进行增加，防止出错
CompUnit
  : CompDefs {
    auto comp_unit = make_unique<CompUnitAST>();
    comp_unit->comp_vec = unique_ptr<vector<unique_ptr<BaseAST>>>($1);
    ast = move(comp_unit);
  }
  ;
// FuncDef ::= FuncType IDENT '(' ')' Block;
// 我们这里可以直接写 '(' 和 ')', 因为之前在 lexer 里已经处理了单个字符的情况
// 解析完成后, 把这些符号的结果收集起来, 然后拼成一个新的字符串, 作为结果返回
// $$ 表示非终结符的返回值, 我们可以通过给这个符号赋值的方法来返回结果
// 你可能会问, FuncType, IDENT 之类的结果已经是字符串指针了
// 为什么还要用 unique_ptr 接住它们, 然后再解引用, 把它们拼成另一个字符串指针呢
// 因为所有的字符串指针都是我们 new 出来的, new 出来的内存一定要 delete
// 否则会发生内存泄漏, 而 unique_ptr 这种智能指针可以自动帮我们 delete
// 虽然此处你看不出用 unique_ptr 和手动 delete 的区别, 但当我们定义了 AST 之后
// 这种写法会省下很多内存管理的负担
CompDefs
  :FuncDef {
    auto vec = new vector<unique_ptr<BaseAST>>();
    auto compdef = unique_ptr<BaseAST>($1);
    vec->push_back(move(compdef));
    $$ = vec;
  }| CompDefs FuncDef{
    auto vec = (vector<unique_ptr<BaseAST>>*)($1);
    auto compdef = unique_ptr<BaseAST>($2);
    vec->push_back(move(compdef));
    $$ = vec;
  }
  ;

CompDef : FuncDef | Decl 

FuncDef
  : FuncType IDENT '(' ')' Block {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseAST>($1);
    ast->ident = *unique_ptr<string>($2);
    ast->block = unique_ptr<BaseAST>($5);
    ast->type = 0;
    $$ = ast;
  }| FuncType IDENT '(' FuncFParams ')' Block {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseAST>($1);
    ast->ident = *unique_ptr<string>($2);
    ast->funcdef_vec = unique_ptr<vector<unique_ptr<BaseAST>>>($4);
    ast->block = unique_ptr<BaseAST>($6);
    ast->type = 1;
    $$ = ast;
  }
  ;

FuncFParams
  :FuncFParam {
    auto vec = new vector<unique_ptr<BaseAST>>();
    auto funcdef = unique_ptr<BaseAST>($1);
    vec->push_back(move(funcdef));
    $$ = vec;
  }| FuncFParam ',' FuncFParams {
    auto vec = (vector<unique_ptr<BaseAST>>*)($3);
    auto funcdef = unique_ptr<BaseAST>($1);
    vec->push_back(move(funcdef));
    $$ = vec;
  }
  ;

FuncFParam
  : BType IDENT {
    auto ast = new FuncFAST();
    ast->btype = unique_ptr<BaseAST>($1);
    ast->ident = *unique_ptr<string>($2);
    $$ = ast;
  }
  ;

// 同上, 不再解释
FuncType
  : INT {
    auto ast =new FuncTypeAST();
    ast->temp = *unique_ptr<string>(new string("int"));
    //token是终结符，无法读取扩展
    $$ = ast;
  }| VOID {
    auto ast =new FuncTypeAST();
    ast->temp = *unique_ptr<string>(new string("void"));
    $$ = ast;
  }
  ;

//block -> {} 文档没写但需要实现
Block
  : '{' BlockItems '}' {
    auto ast = new BlockAST();
    ast->type = 0;
    ast->blockitem_vec = unique_ptr<vector<unique_ptr<BaseAST>>>($2);
    $$ = ast;
  }| '{' '}' {
    auto ast = new BlockAST();  
    ast->type = 1;
    $$ = ast;
  }
  ;

BlockItems
  : BlockItem {
    auto vec = new vector<unique_ptr<BaseAST>> ();
    auto blockitem = unique_ptr<BaseAST>($1);
    vec->push_back(move(blockitem));
    $$ = vec;
  }| BlockItem BlockItems {
    auto vec = (vector<unique_ptr<BaseAST>>*)($2);
    auto blockitem = unique_ptr<BaseAST>($1);
    vec->push_back(move(blockitem));
    $$ = vec;
  }
  ;

BlockItem : Stmt | Decl;

Decl : ConstDecl | VarDecl;

//已经修改
VarDecl 
  : FuncType MulVarDef ';' {
    auto ast = new VarDeclAST();
    ast->btype = unique_ptr<BaseAST>($1);
    ast->vardef_vec = unique_ptr<vector<unique_ptr<BaseAST>>>($2);
    $$ = ast;
  }
  ;

MulVarDef 
  : VarDef {
    auto vec = new vector<unique_ptr<BaseAST>>();
    auto vardef = unique_ptr<BaseAST>($1);
    vec->push_back(move(vardef));
    $$ = vec;
  }| VarDef ',' MulVarDef{
    auto vec = (vector<unique_ptr<BaseAST>>*)($3);
    auto vardef = unique_ptr<BaseAST>($1);
    vec->push_back(move(vardef));
    $$ = vec;
  }
  ;

VarDef
  : IDENT '=' InitVal {
    auto ast = new VarDefAST();
    ast->ident = *unique_ptr<string>($1);
    ast->type = 1;
    ast->ival = unique_ptr<BaseAST>($3);
    $$ = ast;
  }| IDENT {
    auto ast = new VarDefAST();
    ast->type = 0;
    ast->ident = *unique_ptr<string>($1);
    $$ = ast;
  }
  ;

InitVal
  : Exp {
    auto ast = new InitValAST();
    ast->exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

ConstDecl 
  : CONST BType MulConstDef ';' {
    auto ast = new ConstDeclAST();
    ast->btype = unique_ptr<BaseAST>($2);
    ast->constdef_vec = unique_ptr<vector<unique_ptr<BaseAST>>>($3);
    $$ = ast;
  }
  ;


BType
  : INT {
    auto ast =new FuncTypeAST();
    ast->temp = *unique_ptr<string>(new string("int"));
    $$ = ast;
  }
  ;

MulConstDef 
  : ConstDef {
    auto vec = new vector<unique_ptr<BaseAST>>();
    auto constdef = unique_ptr<BaseAST>($1);
    vec->push_back(move(constdef));
    $$ = vec;
  }| ConstDef ',' MulConstDef{
    auto vec = (vector<unique_ptr<BaseAST>>*)($3);
    auto constdef = unique_ptr<BaseAST>($1);
    vec->push_back(move(constdef));
    $$ = vec;
  }
  ;

ConstDef
  : IDENT '=' ConstInitVal {
    auto ast = new ConstDefAST();
    ast->ident = *unique_ptr<string>($1);
    ast->cival = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

ConstInitVal
  : ConstExp {
    auto ast = new ConstInitValAST();
    ast->conexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

ConstExp
  : Exp {
    auto ast = new ConstExpAST();
    ast->exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

Stmt
  : RETURN Exp ';' {
    auto ast = new StmtAST();
    ast->type = 0;
    ast->exp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }| LVal '=' Exp ';' {
    auto ast = new StmtAST();
    ast->type = 1;
    ast->lval = unique_ptr<BaseAST>($1);
    ast->exp = unique_ptr<BaseAST>($3);
    $$ = ast;   
  }| Block {
    auto ast = new StmtAST();
    ast->type = 2;
    ast->exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }| Exp ';' {
    auto ast = new StmtAST();
    ast->type = 3;
    ast->exp = unique_ptr<BaseAST>($1);
    $$ = ast;    
  }| ';' {
    auto ast = new StmtAST();
    ast->type = 4;
    $$ = ast;
  }| RETURN ';' {
    auto ast = new StmtAST();
    ast->type = 5;
    $$ = ast;
  }| IF '(' Exp ')' Stmt %prec LOWER_THAN_ELSE {
    auto ast = new StmtAST();
    ast->type = 7;
    ast->lval = unique_ptr<BaseAST>($3);
    ast->exp = unique_ptr<BaseAST>($5);
    $$ = ast;
  }| IF '(' Exp ')' Stmt ELSE Stmt {
    auto ast = new StmtAST();
    ast->type = 6;
    ast->lval = unique_ptr<BaseAST>($3);
    ast->exp = unique_ptr<BaseAST>($5);
    ast->stmt = unique_ptr<BaseAST>($7);
    $$ = ast;
  }| WHIHE '(' Exp ')' Stmt {
    auto ast = new StmtAST();
    ast->type = 8;
    ast->exp = unique_ptr<BaseAST>($3);
    ast->stmt = unique_ptr<BaseAST>($5);
    $$ = ast;
  }| BREAK ';' {
    auto ast = new StmtAST();
    ast->type = 9;
    $$ = ast;
  }| CONTINUE ';' {
    auto ast = new StmtAST();
    ast->type = 10;
    $$ = ast;
  }
  ;

//else空悬问题-设定优先级来解决-思考自行实现-lookahead


Exp
  : LOrExp {
    auto ast = new ExpAST();
    ast->lor = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

LOrExp  
  :LAndExp {
    auto ast = new LOrExpAST();
    ast->type = 0;
    ast->land = unique_ptr<BaseAST>($1);
    $$ = ast;
  }| LOrExp LOR LAndExp {
    auto ast = new LOrExpAST();
    ast->type = 1;
    ast->lor = unique_ptr<BaseAST>($1);
    ast->op = *unique_ptr<string>(new string("||"));
    ast->land = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

LAndExp  
  :EqExp {
    auto ast = new LAndExpAST();
    ast->type = 0;
    ast->eq = unique_ptr<BaseAST>($1);
    $$ = ast;
  }| LAndExp LAND EqExp {
    auto ast = new LAndExpAST();
    ast->type = 1;
    ast->land = unique_ptr<BaseAST>($1);
    ast->op = *unique_ptr<string>(new string("&&"));
    ast->eq = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

EqExp  
  :RelExp {
    auto ast = new EqExpAST();
    ast->type = 0;
    ast->rel = unique_ptr<BaseAST>($1);
    $$ = ast;
  }| EqExp EqOp RelExp {
    auto ast = new EqExpAST();
    ast->type = 1;
    ast->eq = unique_ptr<BaseAST>($1);
    ast->op = *unique_ptr<string>($2);
    ast->rel = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

EqOp 
  : EQ {
    $$ = new string("==");
  }| NEQ {
    $$ = new string("!=");
  }
  ;

RelExp  
  :AddExp {
    auto ast = new RelExpAST();
    ast->type = 0;
    ast->add = unique_ptr<BaseAST>($1);
    $$ = ast;
  }| RelExp RelOp AddExp {
    auto ast = new RelExpAST();
    ast->type = 1;
    ast->rel = unique_ptr<BaseAST>($1);
    ast->op = *unique_ptr<string>($2);
    ast->add = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

RelOp 
  : '<' {
    $$ = new string("<");
  }| '>' {
    $$ = new string(">");
  }| GEQ {
    $$ = new string(">=");
  }| LEQ {
    $$ = new string("<=");
  }
  ;

AddExp 
  : MulExp {
    auto ast = new AddExpAST();
    ast->type = 0;
    ast->mulexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }| AddExp AddOp MulExp {
    auto ast = new AddExpAST();
    ast->type = 1;
    ast->addexp = unique_ptr<BaseAST>($1);
    ast->op = *unique_ptr<string>($2);
    ast->mulexp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

AddOp 
  : '+' {
    $$ = new string("+");
  }| '-' {
    $$ = new string("-");
  }
  ;

MulExp 
  : UnaryExp {
    auto ast = new MulExpAST();
    ast->type = 0;
    ast->unaryexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }| MulExp MulOp UnaryExp {
    auto ast = new MulExpAST();
    ast->type = 1;
    ast->mulexp = unique_ptr<BaseAST>($1);
    ast->op = *unique_ptr<string>($2);
    ast->unaryexp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

MulOp 
  : '*' {
    $$ = new string("*");
  }| '/' {
    $$ = new string("/");
  }| '%' {
    $$ = new string("%");
  }
  ;

UnaryExp
  : PrimaryExp {
    auto ast = new UnaryExpAST();
    ast->pexp = unique_ptr<BaseAST>($1);
    ast->type = 1; //139 c3
    $$ = ast;
  }| UnaryOp UnaryExp {
    auto ast = new UnaryExpAST();
    ast->opexp = *unique_ptr<string>($1);
    ast->type = 0;
    ast->pexp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }| IDENT '(' ')' {
    auto ast = new UnaryExpAST();
    ast->opexp = *unique_ptr<string>($1);
    ast->type = 2;
    $$ = ast;
  }| IDENT '(' FuncRParams ')' {
    auto ast = new UnaryExpAST();
    ast->opexp = *unique_ptr<string>($1);
    ast->unary_vec = unique_ptr<vector<unique_ptr<BaseAST>>>($3);
    ast->type = 3;
    $$ = ast;
  }
  ;

FuncRParams
  :Exp {
    auto vec = new vector<unique_ptr<BaseAST>>();
    auto expdef = unique_ptr<BaseAST>($1);
    vec->push_back(move(expdef));
    $$ = vec;
  }| Exp ',' FuncRParams {
    auto vec = (vector<unique_ptr<BaseAST>>*)($3);
    auto expdef = unique_ptr<BaseAST>($1);
    vec->push_back(move(expdef));
    $$ = vec;
  }
  ;


PrimaryExp
  : '(' Exp ')' {
    auto ast = new PrimaryExpAST();
    ast->num = unique_ptr<BaseAST>($2);
    ast->type = 0;
    $$ = ast;
  }| Number {
    auto ast = new PrimaryExpAST();
    ast->num = unique_ptr<BaseAST>($1);
    ast->type = 2;
    $$ = ast;
  }| LVal {
    auto ast = new PrimaryExpAST();
    ast->num = unique_ptr<BaseAST>($1);
    ast->type = 1;
    $$ = ast;
  }
  ;

LVal
  : IDENT{
    auto ast = new LValAST();
    ast->ident = *unique_ptr<string>($1);
    $$ = ast;
  }
  ;

UnaryOp 
  : '+' {
    $$ = new string("+");
  }| '-' {
    $$ = new string("-");
  }| '!' {
    $$ = new string("!");
  }
  ;

Number
  : INT_CONST {
    auto ast = new NumberAST();
    ast->number = $1;
    $$ = ast;
  }
  ;

%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
    extern int yylineno;    // defined and maintained in lex
    extern char *yytext;    // defined and maintained in lex
    int len=strlen(yytext);
    int i;
    char buf[512]={0};
    for (i=0;i<len;++i)
    {
        sprintf(buf,"%s%d ",buf,yytext[i]);
    }
    fprintf(stderr, "ERROR: %s at symbol '%s' on line %d\n", s, buf, yylineno);
}
