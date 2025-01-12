#define STR_LEN  100
#define _DEFAULT_SOURCE //I think this is neede for getline()

#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

symbol_t* symbol_table = NULL; //assuming they want a global variable, cuz idk how else these functions gonna work

symbol_t *create_symbol(char* name, int val){
		if(symbol_table == NULL){
			symbol_table = (symbol_t*)calloc(1, sizeof(symbol_t));
		}

		symbol_t* current = symbol_table;
		//printf("%p, %p\n", (void*)current, (void*)symbol_table);
		while(current->var_name != NULL){ //getting to the last entry in the table
			//printf("AHH\n");
			current = current->next;		
		}
		
		current->var_name = calloc(STR_LEN, sizeof(char));
		strcpy(current->var_name, name);
		current->val = val;

		current->next = NULL;
		current->next = (symbol_t*)calloc(1, sizeof(symbol_t)); //initilizing the next value so we can loop to it in the future

		return current;
}

void build_table(char *filename){ //should always be called first
	if(symbol_table == NULL){
		symbol_table = (symbol_t*)calloc(1, sizeof(symbol_t));
	}

	FILE* fp;
	char* line = NULL;
	size_t len;
	
	fp = fopen(filename, "r");
	assert(fp != NULL);

	while(getline(&line, &len, fp) >= 0){
		char* name = calloc(STR_LEN, sizeof(char));
		int val;
		int name_flag = 0; //flags for if name and val were assigned properly
		int val_flag = 0; //might not be the most efficient way, but it works I think
		int name_index = 0;

		for(size_t i = 0; i < len; i++){
			char ch = *(line + i);

			if(ch == '#' || (name_flag != 0 && val_flag != 0)){ //characters that indicate we should be moving on
				//printf("%s\n", name);
				assert((name_flag != 0 && val_flag != 0) || (name_flag == 0 && val_flag == 0)); //making sure that we've assigned both name and value
				if(name_flag != 0 && val_flag != 0){
					create_symbol(name, val);
					//printf("symbol created\n");
				}
				break;
			}

			if(((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) && name_flag == 0){ //excluding whitespace essentially
				*(name + name_index) = ch;
				name_index++;
				//printf("%d\n", name_index);
				//printf("%c", ch);
			}
			else if(name_flag == 0 && name_index != 0){ //if name_flag isn't flagged and there's something in the string
				name_flag = 1;
				*(name + name_index) = '\0'; //to be safe
			}
			if ((ch >= '0' && ch <= '9') && val_flag == 0){ //if not whitespace and name_flag has been flagged and val_flag has not
				val = atoi(line + i);
				//printf("val: %d\n", val);
				val_flag = 1;
			}
		}
		free(name);
	}

	free(line);
	if(fclose(fp) != 0){
		perror("failed to close symtab file");
	}
}

symbol_t* lookup_table(char *variable){
	symbol_t* symbol = symbol_table;
	if(symbol == NULL){
		return NULL;
	}
	while(strcmp(symbol->var_name, variable) != 0){
		if(symbol->next->var_name != NULL){
			symbol = symbol->next;
		}
		else{
			return NULL; //if the next symbol is null and we haven't found, we won't find
		}
	}

	return symbol;
}

void dump_table(void){
	if(symbol_table != NULL){
		printf("SYMBOL TABLE:\n");
		symbol_t* symbol = symbol_table;

		while(symbol->var_name != NULL){ //looping over the names since we always have the unpopulated node
			printf("\tName: %s, Value: %d\n", symbol->var_name, symbol->val);
			symbol = symbol->next;
		}
	}
}

void free_table(void){
	symbol_t* symbol = symbol_table;

	while(symbol != NULL){
		symbol_t* temp = symbol->next;
		free(symbol->var_name);
		free(symbol);
		symbol = temp;
	}
	symbol_table = NULL;
}
