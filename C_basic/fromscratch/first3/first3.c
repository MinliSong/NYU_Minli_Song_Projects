#include<stdio.h>
#include<string.h>

int main(int argc, char** argv){
	char * first_char;
	char * second_char;

	if(argc != 3){
		printf("error: I can only handle two variables!\n");
		return 0;
	}

	first_char = argv[1];
	second_char = argv[2];
	
	if(strlen(first_char)<3 && strlen(second_char)<3){
		printf("error: one or more arguments have fewer than 3 characters\n");
		return 0;
	}
	printf("%c%c%c%c%c%c\n",first_char[0],first_char[1],first_char[2],second_char[0],second_char[1],second_char[2]);

	return 1;

}
