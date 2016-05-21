lex lex.l
yacc -d yac.y
gcc lex.yy.c y.tab.c -lfl -o comp
./comp < input.txt
