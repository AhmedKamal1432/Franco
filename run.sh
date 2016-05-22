lex lex.l
yacc -d yac.y
gcc lex.yy.c y.tab.c -lfl -o comp

python test/rm_cm.py input.txt > test/input_rm_cm.txt
# cat test/input_rm_cm.txt
./comp < test/input_rm_cm.txt
