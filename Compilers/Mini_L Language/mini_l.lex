%{
   #include "y.tab.h"
   #include "string.h"
   int currLine = 1, currPos = 1;
%}

%%
"program" 	{currPos += yyleng; return PROGRAM; }
"beginprogram" 	{currPos += yyleng; return BEGIN_PROGRAM;}
"endprogram" 	{ currPos += yyleng; return END_PROGRAM;}
"integer" 	{currPos += yyleng; return INTEGER;}
"array" 	{currPos += yyleng; return ARRAY;}
"of" 		{currPos += yyleng; return OF;}
"if" 		{currPos += yyleng; return IF;}
"then" 		{currPos += yyleng; return THEN;}
"endif" 	{currPos += yyleng; return ENDIF;}
"else" 		{currPos += yyleng; return ELSE;}
"elseif" 	{currPos += yyleng; return ELSEIF;}
"while" 	{currPos += yyleng; return WHILE;}
"do" 		{currPos += yyleng; return DO;}
"beginloop" 	{currPos += yyleng; return BEGINLOOP;}
"endloop" 	{currPos += yyleng; return ENDLOOP;}
"break" 	{currPos += yyleng; return BREAK;}
"continue" 	{currPos += yyleng; return CONTINUE;}
"exit" 		{currPos += yyleng; return EXIT;}
"read" 		{currPos += yyleng; return READ;}
"write" 	{currPos += yyleng; return WRITE;}
"and" 		{currPos += yyleng; return AND;}
"or" 		{currPos += yyleng; return OR;}
"not" 		{currPos += yyleng; return NOT;}
"true" 		{currPos += yyleng; return TRUE;}
"false" 	{currPos += yyleng; return FALSE;}
[0-9]+       	{currPos += yyleng; yylval.ival = atoi(yytext); return NUMBER;}
[a-zA-Z]+[a-zA-Z0-9]*(_[a-zA-Z0-9]+)* 		{currPos += yyleng; strcpy(yylval.s, yytext); return IDENT;}
[0-9]+[_a-zA-Z0-9]+ 				{printf("** Line %d, column %d: error, identifier \"%s\" must begin with a letter\n"
						, currLine, currPos, yytext); exit(0);}
[a-zA-Z]+[a-zA-Z0-9]*(_[a-zA-Z0-9]+)*_		{printf("** Line %d, column %d: error, identifier \"%s\" cannot end with an underscore\n",
						currLine, currPos, yytext); exit(0);}
"-" 		{currPos += yyleng; return SUB;}
"+" 		{currPos += yyleng; return ADD;}
"*" 		{currPos += yyleng; return MULT;}
"/" 		{currPos += yyleng; return DIV;}
"%" 		{currPos += yyleng; return MOD;}
"==" 		{currPos += yyleng; return EQ;}
"<>" 		{currPos += yyleng; return NEQ;}
"<" 		{currPos += yyleng; return LT;}
">" 		{currPos += yyleng; return GT;}
"<=" 		{currPos += yyleng; return LTE;}
">=" 		{currPos += yyleng; return GTE;}
";" 		{currPos += yyleng; return SEMICOLON;}
":" 		{currPos += yyleng; return COLON;}
"," 		{currPos += yyleng; return COMMA;}
"?" 		{currPos += yyleng; return QUESTION;}
"[" 		{currPos += yyleng; return L_BRACKET;}
"]" 		{currPos += yyleng; return R_BRACKET;}
"(" 		{currPos += yyleng; return L_PAREN;}
")" 		{currPos += yyleng; return R_PAREN;}
":=" 		{currPos += yyleng; return ASSIGN;}
"##".*$		{}
"\n"           	{currLine++; currPos = 1;}
[ \t]+         	{currPos += yyleng;}
.              	{printf("** Line %d, column %d: error, unrecognized symbol \"%s\"\n", currLine, currPos, yytext); exit(0);}
%%
