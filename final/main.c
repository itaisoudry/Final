/*
 * main.c
 *
 *  Created on: Mar 18, 2017
 *      Author: soudry
 */

#include <stdlib.h>
#include <stdio.h>

int main3() {
	printf("main");
	FILE* file = fopen("myconfig.config", "r");
	if (file == NULL)
		printf("NULL");
	char* line = malloc(1024 * sizeof(char));
	while (fgets(line,1024,file) != NULL) {
		printf("%s\n", line);
	}
	return 0;
}
