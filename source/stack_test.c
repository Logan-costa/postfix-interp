#include "stack.h"
#include <stdio.h>

int main(){
	stack_t* stack = make_stack();

	char* s1 = "1";
	char* s2 = "2";
	char* s3 = "3";

	push(stack, (void*)s1);
	push(stack, (void*)s2);
	push(stack, (void*)s3);

	printf("%c\n", *(char*)top(stack));
	pop(stack);		
	printf("%c\n", *(char*)top(stack));
	pop(stack);
	printf("%c\n", *(char*)top(stack));
	
	free_stack(stack);

	return 0;
}
