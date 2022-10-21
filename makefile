compile: tree.h pword.c tword.c
	gcc -w pword.c -lrt -o pword
	gcc -w tword.c -lpthread -o tword
clean: pword tword
	rm pword tword