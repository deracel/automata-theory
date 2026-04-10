%{
#include <stdio.h>
#include <stdlib.h>
#include <string,h>

int line_num = 1;
int error_count = 0;
%}

DIGIT [0-9]
LETTER [a-zA-Z]
WHITESPACE [ \t\r]+
NEWLINE \n

%%

<PROGRAM>                   {return TOKEN_PROGRAM;}
</PROGRAM>                  {return TOKEN_PROGRAM_END;}

<VARDECLARATION>            {return TOKEN_VARDECL;}
</VARDECLARATION>           {return TOKEN_VARDECL_END;}

<VAR                        {return TOK_VAR_OPEN;}
</VAR>                      {return TOK_VAR_CLOSE;}

<FUNC                       {return TOK_FUNC_OPEN;}
</FUNC>                     {return TOK_FUNC_CLOSE;}

<CALL>                      {return TOK_CALL_OPEN;}
</CALL>                     {return TOK_CALL_CLOSE;}



name=                       {return TOK_NAME_ATTR;}
CONST=                      {return TOK_CONST;}

