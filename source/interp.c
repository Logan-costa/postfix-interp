#include "interp.h"
#include "symtab.h"
#include "parser.h"
#include <stdio.h> //I have no idea why, but gcc complains that getline() isn't defined, even though stdio is right here
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc > 2){
		fprintf(stderr, "Usage: interp [sym-table]\n");
		return EXIT_FAILURE;
	}
	else if(argc == 2){
		build_table(argv[1]);
		dump_table();
	}
	printf("Enter postfix expressions (CTRL-D to exit):\n");

	char* buffer = NULL;
	size_t size = 0;

	printf("> ");
	while(getline(&buffer, &size, stdin) > 0){
		//printf("%s\n", buffer);
		char* replace = strchr(buffer, '#');
		if(replace != NULL){
			*(buffer + (strlen(buffer) - strlen(replace))) = '\0';
		}
		if(*buffer == '\0'){
			printf("> ");
			continue;
		}
		rep(buffer);
		printf("> ");
	}

	if(buffer != NULL){
		free(buffer);
	}

	printf("\n");
	dump_table();
	free_table(); //this should always be the last thing
	return EXIT_SUCCESS;
}
