%{
 #include <stdio.h>
 #include <stdlib.h>
 #include <sstream>
 #include <fstream>
 #include <string>
 #include <map>
 #include <stack>
 using namespace std;

 void yyerror(const char *msg);
 int yylex(void);
 extern int currLine;
 extern int currPos;

 stringstream declare;
 stringstream body;
 //ofstream declare("declare.txt");
 //ofstream body("body.txt");

 ofstream output;

 map<string, int> identifiers;
 stack<string> id_stack;
 stack<string> index_stack;
 stack<string> logic_stack;
 stack<string> pred_stack;
 stack<string> label_stack;
 stack<string> cont_stack;
 stack<string> brk_stack;
 stack<string> io_stack;

 string program_name;

 int temp_num = 0;
 int pred_num = 0;
 int label_num = 0;
 
 bool declare_mode = 1;
 bool error_flag = 0;
 string from_array (string save) {
	if (index_stack.top()!="-1") {
		stringstream conv;
		conv << temp_num;
		declare << ". t" << temp_num << endl;
		body << "=[] t" << temp_num << ", " << save
		<< ", " << index_stack.top() << endl;
		temp_num++;
		return "t"+conv.str();
	}
	return save;
 }

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
%type <ival> exp_op
%type <ival> mult_op
%left PLUS MINUS
%left MULT DIV

%% 
prog:			PROGRAM p_ident SEMICOLON block END_PROGRAM {
				body << ": EndLabel" << endl;
				if (!error_flag) {
					output.open (program_name.c_str(), std::ofstream::out);
					output << declare.rdbuf();
					output << body.rdbuf();
				}
			} 
			;

p_ident:		IDENT {
				string aug_name = "_"+string($1);
				pair<string, int> id(string(aug_name),-1);
				identifiers.insert(id);
				program_name = string($1)+".mil";
			}
			;

block:			dec SEMICOLON dec_loop begin_program stat_loop
			;

begin_program:		BEGIN_PROGRAM {
				body << ": START" << endl;
				declare_mode = 0;
			}

dec_loop:		dec SEMICOLON dec_loop
			|
			;

dec:			ident ident_loop COLON dec2
			;

ident_loop:		COMMA ident ident_loop		
			|				
			;

ident:			IDENT {
				string aug_name = "_"+string($1);
				if(declare_mode) {
					pair<string, int> id(string(aug_name),-1);
					if (!identifiers.insert(id).second) {
						//declaration error already defined
						string error = "error, declaration name '"+string($1)
						+"' already defined.";
						yyerror(error.c_str());
					}			
				}
				else {
					if (
					string($1) == "integer" ||
					string($1) == "array" ||
					string($1) == "loop" ||
					string($1) == "endloop" ||
					string($1) == "break" ||
					string($1) == "continue" ||
					string($1) == "read" ||
					string($1) == "write" ||
					string($1) == "of" ||
					string($1) == "if" ||
					string($1) == "while" ||
					string($1) == "and" ||
					string($1) == "or" ||
					string($1) == "not" ||
					string($1) == "then" ||
					string($1) == "else" ||
					string($1) == "endif" ||
					string($1) == "true" ||
					string($1) == "false" ||
					string($1) == "program" ||
					string($1) == "endprogram"
					) {
						string error = "error, "+string($1)
						+" is not declared.";
						yyerror(error.c_str());
					}
					else if(identifiers.find(aug_name)==identifiers.end()) {
						//variable error
						string error = "error, "+string($1)
						+" is not declared.";
						yyerror(error.c_str());
					}
					
				}
				id_stack.push(string(aug_name));
			}
			;

dec2:			INTEGER	{
				for(string dec; !id_stack.empty(); id_stack.pop()) {
					dec = id_stack.top();
					declare << ". " << dec << endl;
				}
									
			}
			| ARRAY L_BRACKET NUMBER R_BRACKET OF INTEGER {
				if ($3 <= 0) {
					string error = "error, array size must be a positive value.";
					yyerror(error.c_str());
				}
				for (string dec; !id_stack.empty(); id_stack.pop()) {
					dec = id_stack.top();
					identifiers[dec] = $3;
					declare << ".[] " << dec << ", " << identifiers[dec] << endl;
				}
			}
			| ARRAY L_BRACKET SUB NUMBER R_BRACKET OF INTEGER {
				string error = "error, array size must be a positive value.";
				yyerror(error.c_str());
			}
			;

stat:			EXIT {body << "\t:= EndLabel" << endl;}
			| CONTINUE {
				if (cont_stack.size())
					body << ":= " << cont_stack.top() << endl;
				else {
					string error = "error, 'continue' occurs outside of loop scope.";
					yyerror(error.c_str());
				}
			}
			| BREAK {
				if (brk_stack.size())
					body << ":= " << brk_stack.top() << endl;
				else {
					string error = "error, 'break' occurs outside of loop scope.";
					yyerror(error.c_str());
				}
			}
			| WRITE var_loop {
				stringstream conv;
				while (id_stack.size()) {
					if(index_stack.top() == "-1") {
						conv.str("");
						conv << "\t.> " << id_stack.top();
						io_stack.push(conv.str());
					}
					else {
						conv.str("");
						conv << "\t.[]> " << id_stack.top() << ", " 
						<< index_stack.top();
						io_stack.push(conv.str());
					}
					id_stack.pop();
					index_stack.pop();
				}
				while(io_stack.size()) {
					body << io_stack.top() << endl;
					io_stack.pop();
				}
			}
			| READ var_loop {
				stringstream conv;
				while (id_stack.size()) {
					if(index_stack.top() == "-1") {
						conv.str("");
						conv << "\t.< " << id_stack.top();
						io_stack.push(conv.str());
					}
					else {
						conv.str("");
						conv << "\t.[]< " << id_stack.top() << ", " 
						<< index_stack.top();
						io_stack.push(conv.str());
					}
					id_stack.pop();
					index_stack.pop();
				}
				while(io_stack.size()) {
					body << io_stack.top() << endl;
					io_stack.pop();
				}
			}

			| if_stat
			| DO d_beginloop stat_loop ENDLOOP WHILE bool_exp {
				body << ": " << cont_stack.top() << endl;
				cont_stack.pop();
				body << "\t?:= " << label_stack.top() << ", " << pred_stack.top() << endl;
				label_stack.pop();
				pred_stack.pop();
				body << ": " << brk_stack.top() << endl;
				brk_stack.pop();
			}
			| w_while bool_exp w_beginloop stat_loop w_endloop
			| var ASSIGN stat2 {
			        string saver = from_array(id_stack.top());
			        id_stack.pop();
			        index_stack.pop();
			        if(index_stack.top()!="-1") {
			                body << "\t[]= " << id_stack.top() << ", " 
			                << index_stack.top() << ", " << saver << endl;
			        }
			        else {
			                body << "\t= " << id_stack.top() << ", " << saver << endl;
			        }
			        id_stack.pop();
			        index_stack.pop();
			}
			;

if_stat:		IF bool_exp then stat_loop else_loop ENDIF {
				if (label_stack.top() != "dummy")
					body << ": " << label_stack.top() << endl;
				label_stack.pop();
				body << ": " << label_stack.top() << endl;
				label_stack.pop();
			}
			;

then:			THEN {
				//end label
				stringstream conv;
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;

                                body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				conv.str("");
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
			}
			;

else_loop:		elseif bool_exp e_cond stat_loop else_loop {

				
			}
			| else stat_loop {
				
			}
			|
			;

else:			ELSE {
				string iter = label_stack.top();
				label_stack.pop();
				body << "\t:= " << label_stack.top() << endl;
				body << ": " << iter << endl;
				label_stack.push("dummy");
			}
			;

elseif:			ELSEIF {
				string iter = label_stack.top();
				label_stack.pop();
				body << "\t:= " << label_stack.top() << endl;
				body << ": " << iter << endl;
			}
			;

e_cond:			/*epsilon*/ {
                                body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << "\t. p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
			}
			;

d_beginloop:		BEGINLOOP {
				stringstream conv;
				conv << "L" << label_num;
				brk_stack.push(conv.str());
				label_num++;
                                body << ": L" << label_num << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
				conv.str("");
				conv << "L" << label_num;
				cont_stack.push(conv.str());
				label_num++;
			}
			;
			
w_while:                 WHILE {
                                body << ": L" << label_num << endl;
				stringstream conv;
				conv << "L" << label_num;
				label_stack.push(conv.str());
				cont_stack.push(conv.str());
				label_num++;
                        }
                        ;
                        
w_beginloop:            BEGINLOOP {
                                body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				brk_stack.push(conv.str());
				label_num++;
                        }
                        ;
                        
w_endloop:              ENDLOOP {
				cont_stack.pop();
				brk_stack.pop();
                                string end_label = label_stack.top();
                                label_stack.pop();
                                body << "\t:= " << label_stack.top() << endl;
                                label_stack.pop();
                                body << ": " << end_label << endl;
                        }
                        ;

stat2:			exp
			| bool_exp question exp colon exp {
			        string result = id_stack.top();
			        id_stack.pop();
			        if (index_stack.top()!="-1")
                        		body << "\t=[] " << id_stack.top() << ", " << result
                        		<< ", " << index_stack.top() << endl;
	                        else
	                                body << "\t= " << id_stack.top() << ", " << result << endl;
	                        index_stack.pop();

			        body << ": " << label_stack.top() << endl;
                                label_stack.pop();
			}
			;
			
colon:                  COLON {
                        
			        if (index_stack.top()!="-1")
                        		body << "\t=[] t" << temp_num << ", " << id_stack.top()
                        		<< ", " << index_stack.top() << endl;
	                        else
	                                body << "\t= t" << temp_num << ", " << id_stack.top() << endl;
	                        index_stack.pop();
	                        id_stack.pop();
	                        declare << ". t" << temp_num << endl;
	                        stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
	                        

                                body << "\t:= L" << label_num << endl;
                                body << ": " << label_stack.top() << endl;
                                label_stack.pop();
                                conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
                        }
                        ;
			
question:               QUESTION {
				body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
                        }
			;

bool_exp:		rel_and_exp bool_exp2
			;

bool_exp2:		OR rel_and_exp bool_exp2 {
                                string saver = pred_stack.top();
                                pred_stack.pop();
                                string savel = pred_stack.top();
                                pred_stack.pop();
                                declare << ". p" << pred_num << endl;
                                body << "\t|| p" << pred_num << ", " << savel << ", " << saver << endl;
                                stringstream conv;
                                conv << "p" << pred_num;
                                pred_stack.push(conv.str());
                                pred_num++;
                        }
			|
			;

rel_and_exp:		rel_exp rel_and_exp2
			;

rel_and_exp2:		AND rel_exp rel_and_exp2 {
                                string saver = pred_stack.top();
                                pred_stack.pop();
                                string savel = pred_stack.top();
                                pred_stack.pop();
                                declare << ". p" << pred_num << endl;
                                body << "\t&& p" << pred_num << ", " << savel << ", " << saver << endl;
                                stringstream conv;
                                conv << "p" << pred_num;
                                pred_stack.push(conv.str());
                                pred_num++;
                        }
			|
			;

rel_exp:		NOT rel_exp2 {
				body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				pred_stack.push(conv.str());
				pred_num++;
			}
			| rel_exp2
			;

rel_exp2:		exp comp exp {
				string saver = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				string savel = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				body << "\t" <<logic_stack.top() << " p" << pred_num
				<< ", " << savel << ", " << saver << endl;
				logic_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				pred_stack.push(conv.str());
				pred_num++;
			}
			| TRUE {pred_stack.push("1");}
			| FALSE {pred_stack.push("0");}
			| L_PAREN bool_exp R_PAREN
			;

comp:			EQ {logic_stack.push("==");}
			| NEQ {logic_stack.push("!=");}
			| LT {logic_stack.push("<");}
			| GT {logic_stack.push(">");}
			| LTE {logic_stack.push("<=");}
			| GTE {logic_stack.push(">=");}
			;

exp:			m_exp exp2
			;

exp2:			exp_op m_exp exp2 {
				string saver = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				string savel = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				body << "\t";
				switch ($1) {
					case 0: //addition
						body << "+";
					break;
					case 1: //subtraction
						body << "-";
					break;
				}
				declare << ". t" << temp_num << endl;
 				body << " t" << temp_num << ", " << savel << ", " << saver << endl;
				stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
			}
			|
			;

exp_op:			ADD {$$ = 0;}
			| SUB {$$ = 1;}
			;

m_exp:			term m_exp2
			;

m_exp2:			mult_op term m_exp2 {
				string saver = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				string savel = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				body << "\t";
				switch ($1) {
					case 0: //multiplication
						body << "*";
					break;
					case 1: //division
						body << "/";
					break;
					case 2: //modulus
						body << "%";
					break;
				}
				declare << ". t" << temp_num << endl;
 				body << " t" << temp_num << ", " << savel << ", " << saver << endl;
				stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
			}
			|
			;

mult_op:		MULT {$$ = 0;}
			| DIV {$$ = 1;}
			| MOD {$$ = 2;}
			;

term:			SUB term2 {
				string save = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				declare << ". t" << temp_num << endl;
				body << "\t- t" << temp_num << ", 0, " << save << endl;
				stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
			}
			| term2
			;

term2:			var
			| NUMBER {
				stringstream conv;
				conv << $1;
				id_stack.push(string(conv.str()));
				index_stack.push("-1");
			}
			| L_PAREN exp R_PAREN
			;

var:			ident {
				if (identifiers.find(id_stack.top())!=identifiers.end() &&
				identifiers[id_stack.top()]!=-1) {
					//error: this is an array with index
					string error = "error, "+id_stack.top().substr(1)
					+" is an array and requires an index.";
					yyerror(error.c_str());
				}
				index_stack.push("-1");
			}
			| ident l_bracket exp R_BRACKET {
				index_stack.pop();
				index_stack.push(id_stack.top());
				id_stack.pop();
			
			}
			;
l_bracket:		L_BRACKET {
				if (identifiers.find(id_stack.top())!=identifiers.end() &&
				identifiers[id_stack.top()]==-1) {
					string error = "error, "+id_stack.top().substr(1)
					+" is not an array.";
					yyerror(error.c_str());
				}
			}
			;

stat_loop:		stat SEMICOLON stat_loop2
			;

stat_loop2:		stat SEMICOLON stat_loop2
			|
			;

var_loop:		var var_loop2
			;

var_loop2:		COMMA var var_loop2
			|
			;
%%

int main() {
   yyparse();
   return 0;
}

void yyerror(const char *msg) {
   error_flag = 1;
   printf("** Line %d, column %d: %s\n", currLine, currPos, msg);
}
