#!/bin/bash
bison --verbose -d parser.y
flex -d lexer.l
gcc -g -DYYERROR_VERBOSE=1 hellLexer.c hellParser.c hellStmt.c -o hell

