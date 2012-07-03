#!/bin/bash
bison --verbose -d parser.y
flex -d lexer.l
gcc -g -DYYDEBUG=1 -DYYERROR_VERBOSE=1 hellLexer.c hellParser.c hellStmt.c -o hell

