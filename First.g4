grammar First;

line: (decl | assignment | expr | funCall | ifStat) NEWLINE ;

decl: ID ;
assignment: ID '=' expr ;
expr: pri |
    expr ('*'|'/') expr |
    expr ('+'|'-') expr
    ;

pri: NUM | ID | '(' expr ')' ;

funCall: 'ID' '(' expr (',' expr)* ')' ;

ifStat: 'if' expr 'then' line ;


PLUS: '+' ;
MINUS: '-' ;
MUL: '*' ;
DIV: '/' ;
GT: '>' ;
GE: '>=' ;
LT: '<' ;
LE: '<=' ;
EQ: '==' ;
NE: '~=' ;
ASSIGN: '=' ;
LPAREN: '(' ;
RPAREN: ')' ;
COMMA: ',' ;
IF: 'if' ;
THEN: 'then' ;

NUM: Digit+ ;
ID: Char (Digit|Char)* ;

NEWLINE: '\r'? '\n' ;
WS: [ \t]+ -> skip ;

fragment Digit: [0-9] ;
fragment Char: [A-Za-z] ;