lexer grammar GpuSqlLexer;

DATETIMELIT : SQOUTE DATELIT ([ ] TIMELIT)? SQOUTE;

LF          : '\n' -> skip;
CR          : '\r' -> skip;
CRLF        : '\r\n' -> skip;
WS          : [\t\r\n ]+ -> skip ;
SEMICOL     : ';';
SQOUTE      : '\'';
DQOUTE      : '"';
UNDERSCORE  : '_';
COLON       : ':';
COMMA       : ',';
DOT         : '.';
STRING      : '"' ( ~[\\"\r\n] )* '"';
DELIMID     : '[' ( ~[\\[\]$\r\n\u002e] )+ ']';

DATELIT                 : YEARLIT MINUS MONTHLIT MINUS DAYLIT;

fragment YEARLIT        : [1-9][0-9][0-9][0-9];
fragment MONTHLIT       : [1][0-2]|[0][1-9];
fragment DAYLIT         : [0][1-9]|[1-2][0-9]|[3][0-1];

TIMELIT                 : HOURLIT COLON MINUTELIT COLON SECONDLIT;

fragment HOURLIT        : [0-1][0-9]|[2][0-3];
fragment MINUTELIT      : [0-5][0-9];
fragment SECONDLIT      : [0-5][0-9];

POINT           : 'POINT';
MULTIPOINT      : 'MULTIPOINT';
LINESTRING      : 'LINESTRING';
MULTILINESTRING : 'MULTILINESTRING';
POLYGON         : 'POLYGON';
MULTIPOLYGON    : 'MULTIPOLYGON';

DATATYPE    : (INTTYPE|LONGTYPE|DATETYPE|FLOATTYPE|DOUBLETYPE|STRINGTYPE|BOOLEANTYPE|POINTTYPE|POLYTYPE);

INTTYPE     : I N T;
LONGTYPE    : L O N G;
DATETYPE    : D A T E;
FLOATTYPE   : F L O A T;
DOUBLETYPE  : D O U B L E;
STRINGTYPE  : S T R I N G;
BOOLEANTYPE : B O O L E A N;
POINTTYPE   : G E O '_' P O I N T;
POLYTYPE    : G E O '_' P O L Y G O N;

INSERTINTO  : I N S E R T ' ' I N T O;
CREATEDB    : C R E A T E ' ' D A T A B A S E;
DROPDB      : D R O P ' ' D A T A B A S E;
CREATETABLE : C R E A T E ' ' T A B L E;
DROPTABLE   : D R O P ' ' T A B L E;
ALTERTABLE  : A L T E R ' ' T A B L E;
ADD         : A D D;
DROPCOLUMN  : D R O P ' ' C O L U M N;
ALTERCOLUMN : A L T E R ' ' C O L U M N;
CREATEINDEX : C R E A T E ' ' INDEX;
INDEX       : I N D E X;
PRIMARYKEY  : P R I M A R Y ' ' K E Y;
VALUES      : V A L U E S;
SELECT      : S E L E C T;
FROM        : F R O M;
JOIN        : J O I N;
WHERE       : W H E R E;
GROUPBY     : G R O U P ' ' B Y;
AS          : A S;
IN          : I N;
ISNULL      : I S ' ' N U L L;
ISNOTNULL   : I S ' ' N O T ' ' N U L L;
BETWEEN     : B E T W E E N;
ON          : O N;
ORDERBY     : O R D E R ' ' B Y;
DIR         : (A S C) | (D E S C);
LIMIT       : L I M I T;
OFFSET      : O F F S E T;

INNER       : I N N E R;
FULLOUTER   : F U L L ' ' O U T E R;

SHOWDB      : S H O W ' ' D A T A B A S E S;
SHOWTB      : S H O W ' ' T A B L E S;
SHOWCL      : S H O W ' ' C O L U M N S;

AVG_AGG     : A V G;
SUM_AGG     : S U M;
MIN_AGG     : M I N;
MAX_AGG     : M A X;
COUNT_AGG   : C O U N T;

YEAR        : Y E A R;
MONTH       : M O N T H;
DAY         : D A Y;
HOUR        : H O U R;
MINUTE      : M I N U T E;
SECOND      : S E C O N D;
NOW         : N O W LPAREN RPAREN;

PI          : P I LPAREN RPAREN;
ABS         : A B S;
SIN         : S I N;
COS         : C O S;
TAN         : T A N;
COT         : C O T;
ASIN        : A S I N;
ACOS        : A C O S;
ATAN        : A T A N;
ATAN2       : A T A N '2';

LOG10       : L O G '10';
LOG         : L O G;
EXP         : E X P;
POW         : P O W;
SQRT        : S Q R T;
SQUARE      : S Q U A R E;
SIGN        : S I G N;
ROOT        : R O O T;
ROUND       : R O U N D;
CEIL        : C E I L;
FLOOR       : F L O O R;

LTRIM       : L T R I M;
RTRIM       : R T R I M;
LOWER       : L O W E R;
UPPER       : U P P E R;
REVERSE     : R E V E R S E;
LEN         : L E N;
LEFT        : L E F T;
RIGHT       : R I G H T;
CONCAT      : C O N C A T;

CAST        : C A S T;

GEO_CONTAINS        : G E O UNDERSCORE C O N T A I N S;
GEO_INTERSECT       : G E O UNDERSCORE I N T E R S E C T;
GEO_UNION           : G E O UNDERSCORE U N I O N;

PLUS            : '+' ;
MINUS           : '-' ;
ASTERISK        : '*' ;
DIVISION        : '/' ;
MODULO          : '%' ;
XOR             : '^';
EQUALS          : '=' ;
NOTEQUALS       : '!=' ;
NOTEQUALS_GT_LT : '<>';
LPAREN          : '(' ;
RPAREN          : ')' ;
GREATER         : '>' ;
LESS            : '<' ;
GREATEREQ       : '>=' ;
LESSEQ          : '<=' ;
LOGICAL_NOT     : '!' ;
OR              : O R;
AND             : A N D;
BIT_OR          : '|';
BIT_AND         : '&';
L_SHIFT         : '<<';
R_SHIFT         : '>>';

BOOLEANLIT  : (TRUE|FALSE);
TRUE        : T R U E;
FALSE       : F A L S E;
FLOATLIT    : ('-')?('0.'[0-9]+|[1-9][0-9]* '.'[0-9]+);
INTLIT      : ('-')?('0'|[1-9][0-9]*);
NULLLIT     : N U L L;
ID          : [_]*[A-Za-z0-9_][A-Za-z0-9_]* ;

fragment A : [aA];
fragment B : [bB];
fragment C : [cC];
fragment D : [dD];
fragment E : [eE];
fragment F : [fF];
fragment G : [gG];
fragment H : [hH];
fragment I : [iI];
fragment J : [jJ];
fragment K : [kK];
fragment L : [lL];
fragment M : [mM];
fragment N : [nN];
fragment O : [oO];
fragment P : [pP];
fragment Q : [qQ];
fragment R : [rR];
fragment S : [sS];
fragment T : [tT];
fragment U : [uU];
fragment V : [vV];
fragment W : [wW];
fragment X : [xX];
fragment Y : [yY];
fragment Z : [zZ];

