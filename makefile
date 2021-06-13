# Created: 13-Jun-2021
# Geoffrey Jarman
# makefile for mysql-c-library project
# Ref: http://www6.uniovi.es/cscene/CS2/CS2-10.html -- Gnu Make and Multifile projects
#      https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html -- Make tutorial
#      https://www.youtube.com/watch?v=G5dNorAoeCM&feature=youtu.be -- Jacob Sorber, Automatic variables in make
# -ggdb option includs gdb friendly debug info in the executable
# $@ Target file
# $^ Dependency files
# $(CC) Compiler executable
# $(CFLAGS) Compiler options
# $@ Souce file
# $^ Dependent files (dependencies)

# Variables

CC=gcc
CFLAGS=-g -o

all: mysql-c-library -I/usr/include/mysql

# General pattern make
%: %.c %.h
	$(CC) $(CFLAGS) $@ $^

# Specific pattern makes

mysql-c-library: mysql-c-library.c
	gcc -g -o mysql-c-library $(mysql_config --cflags) mysql-c-library.c $(mysql_config --libs) cs50.c

# clean or delete all generated intermediate files
clean:
	
