#include "symtab.h"
#include <stdio.h>

int main(){
	build_table("./sym.1");
	printf("table built!\n");
	dump_table();
	free_table();

	return 0;
}
