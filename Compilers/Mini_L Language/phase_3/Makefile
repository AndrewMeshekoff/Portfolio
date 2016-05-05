all: my_compiler

my_compiler: y.tab.c y.tab.h lex.yy.c
	g++ -o my_compiler y.tab.c lex.yy.c -lfl
        
y.tab.c y.tab.h: mini_l.y
	bison -v -d --file-prefix=y mini_l.y
        
#y.tab.h: y.tab.c
#        noop

lex.yy.c: mini_l.lex
	flex mini_l.lex
