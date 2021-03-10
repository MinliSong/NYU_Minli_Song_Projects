#include<stdio.h>

int main(int argc, char** argv){
	char * input;

	if(argc != 2){
		printf("usage: countas <arg>\n");
	}

	input = argv[1];
	int i = 0;
	int count = 0;
	while(input[i] != '\0'){
		if(input[i] == 'a'){
			count ++;
		}
		i ++;
	}
	printf("%d\n", count);

}

