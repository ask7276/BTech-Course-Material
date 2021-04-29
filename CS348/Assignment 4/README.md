# Roll number: 180101037
# Name: Anket Sanjay Kotkar

Instructions to execute the code:

    $ yacc -d 180101037.y
    $ lex 180101037.l
    $ gcc lex.yy.c y.tab.c -o parser
    $ ./parser < input_file_path

where input_file_path is the path of input file. In case of semantic error, parser will try to print all errors. In case of syntax error, it will stop execution after printing the 1st syntax error.
