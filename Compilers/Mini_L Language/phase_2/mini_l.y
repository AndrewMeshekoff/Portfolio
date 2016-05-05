%{
 #include <stdio.h>
 #include <stdlib.h>


 void yyerror(const char *msg);
 extern int currLine;
 extern int currPos;

%}

%union{
  int ival;
  char s[20];
}

%error-verbose
%start prog
%token 	PROGRAM BEGIN_PROGRAM END_PROGRAM INTEGER
	ARRAY OF IF THEN ENDIF ELSE ELSEIF WHILE DO
	BEGINLOOP ENDLOOP BREAK CONTINUE EXIT READ
	WRITE AND OR NOT TRUE FALSE SUB ADD MULT DIV
	MOD EQ NEQ LT GT LTE GTE PLUS MINUS
	SEMICOLON COLON COMMA QUESTION 
	L_BRACKET R_BRACKET L_PAREN R_PAREN ASSIGN 



%token <ival> NUMBER
%token <s> IDENT
%type <ival> exp
%left PLUS MINUS
%left MULT DIV

%% 
prog:			PROGRAM ident SEMICOLON block END_PROGRAM   	{printf("prog -> PROGRAM ident SEMICOLON block END_PROGRAM\n");} 
			;

block:			dec SEMICOLON dec_loop BEGIN_PROGRAM stat_loop	{printf("block -> dec SEMICOLON dec_loop BEGIN_PROGRAM stat_loop\n");}
			;

dec_loop:		dec SEMICOLON dec_loop				{printf("dec_loop -> dec SEMICOLON dec_loop\n");}
			|						{printf("dec_loop -> \n");}
			;

dec:			ident ident_loop COLON dec2			{printf("dec -> ident ident_loop COLON dec2\n");}
			;

ident_loop:		COMMA ident ident_loop				{printf("ident_loop -> COMMA ident ident_loop\n");}
			|						{printf("ident_loop -> \n");}
			;

ident:			IDENT						{printf("ident -> IDENT (%s)\n", $1);}
			;

dec2:			INTEGER						{printf("dec2 -> INTEGER\n");}
			| ARRAY L_BRACKET NUMBER R_BRACKET OF INTEGER	{printf("dec2 -> ARRAY L_BRACKET NUMBER (%d) R_BRACKET OF INTEGER\n", $3);}
			;

stat:			EXIT						{printf("stat -> EXIT\n");}
			| CONTINUE					{printf("stat -> CONTINUE\n");}
			| BREAK						{printf("stat -> BREAK\n");}
			| WRITE var_loop				{printf("stat -> WRITE var_loop\n");}
			| READ var_loop					{printf("stat -> READ var_loop\n");}
			| if_stat					{printf("stat -> if_stat\n");}
			| DO BEGINLOOP stat_loop ENDLOOP WHILE bool_exp		{printf("stat -> DO BEGINLOOP stat_loop ENDLOOP WHILE bool_exp\n");}
			| WHILE bool_exp BEGINLOOP stat_loop ENDLOOP	{printf("stat -> WHILE bool_exp BEGINLOOP stat_loop ENDLOOP\n");}
			| var ASSIGN stat2				{printf("stat -> var ASSIGN stat2\n");}
			;

if_stat:		IF bool_exp THEN stat_loop else_loop ENDIF	{printf("if_stat -> IF bool_exp THEN stat_loop else_loop ENDIF\n");}
			;

else_loop:		ELSEIF bool_exp stat_loop else_loop		{printf("else_loop -> ELSEIF bool_exp stat_loop else_loop\n");}
			| ELSE stat_loop				{printf("else_loop -> ELSE stat_loop\n");}
			|						{printf("else_loop -> \n");}
			;

stat2:			exp						{printf("stat2 -> exp\n");}
			| bool_exp QUESTION exp COLON exp		{printf("stat2 -> bool_exp QUESTION exp COLON exp\n");}
			;

bool_exp:		rel_and_exp bool_exp2				{printf("bool_exp -> rel_and_exp bool_exp2\n");}
			;

bool_exp2:		OR rel_and_exp bool_exp2			{printf("bool_exp2 -> OR rel_and_exp\n");}
			|						{printf("bool_exp2 -> \n");}
			;

rel_and_exp:		rel_exp rel_and_exp2				{printf("rel_and_exp -> rel_exp rel_and_exp2\n");}
			;

rel_and_exp2:		AND rel_exp rel_and_exp2			{printf("rel_and_exp2 -> AND rel_exp rel_and_exp2\n");}
			|						{printf("rel_and_exp2 -> \n");}
			;

rel_exp:		NOT rel_exp2					{printf("rel_exp -> NOT rel_exp2\n");}
			| rel_exp2					{printf("rel_exp -> rel_exp2\n");}
			;

rel_exp2:		exp comp exp					{printf("rel_exp2 -> exp comp exp\n");}
			| TRUE						{printf("rel_exp2 -> TRUE\n");}
			| FALSE						{printf("rel_exp2 -> FALSE\n");}
			| L_PAREN bool_exp R_PAREN			{printf("rel_exp2 -> L_PAREN bool_exp R_PAREN\n");}
			;

comp:			EQ						{printf("comp -> EQ\n");}
			| NEQ						{printf("comp -> NEQ\n");}
			| LT						{printf("comp -> LT\n");}
			| GT						{printf("comp -> GT\n");}
			| LTE						{printf("comp -> LTE\n");}
			| GTE						{printf("comp -> GTE\n");}
			;

exp:			m_exp exp2					{printf("exp -> m_exp exp2\n");}
			;

exp2:			exp_op m_exp exp2				{printf("exp2 -> exp_op m_exp exp2\n");}
			|						{printf("exp2 -> \n");}
			;

exp_op:			ADD						{printf("exp_op -> ADD\n");}
			| SUB						{printf("exp_op -> SUB\n");}
			;

m_exp:			term m_exp2					{printf("m_exp -> term m_exp2\n");}
			;

m_exp2:			mult_op term m_exp2				{printf("m_exp2 -> mult_op term m_exp2\n");}
			|						{printf("m_exp2 -> \n");}
			;

mult_op:		MULT						{printf("mult_op -> MULT\n");}
			| DIV						{printf("mult_op -> DIV\n");}
			| MOD						{printf("mult_op -> MOD\n");}
			;

term:			SUB term2					{printf("term -> SUB term2\n");}
			| term2						{printf("term -> term2\n");}
			;

term2:			var						{printf("term2 -> var\n");}
			| NUMBER					{printf("term2 -> NUMBER (%d)\n", $1);}
			| L_PAREN exp R_PAREN				{printf("term2 -> L_PAREN exp R_PAREN\n");}
			;

var:			ident						{printf("var -> ident\n");}
			| ident L_BRACKET exp R_BRACKET			{printf("var -> ident L_PAREN exp R_BRACKET\n");}
			;

stat_loop:		stat SEMICOLON stat_loop2			{printf("stat_loop -> stat SEMICOLON stat_loop2\n");}
			;

stat_loop2:		stat SEMICOLON stat_loop2			{printf("stat_loop2 -> stat SEMICOLON stat_loop2\n");}
			|						{printf("stat_loop2 -> \n");}
			;

var_loop:		var var_loop2					{printf("var_loop -> var var_loop2\n");}
			;

var_loop2:		COMMA var var_loop2				{printf("var_loop2 -> COMMA var var_loop2\n");}
			|						{printf("var_loop2 -> \n");}
			;
%%

int main() {
   yyparse();
   return 0;
}

void yyerror(const char *msg) {
   printf("** Line %d, position %d: %s\n", currLine, currPos, msg);
}
