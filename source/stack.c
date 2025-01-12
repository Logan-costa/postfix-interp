#include "stack.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

stack_t *make_stack(void){
	stack_t* new = (stack_t*)calloc(1, sizeof(stack_t));
	return new;
}

void push(stack_t *stack, void *data){
	stack_node_t* new = (stack_node_t*)calloc(1, sizeof(stack_node_t));
	new->data = data;
	new->next = stack->top; //can be NULL

	//printf("pushed\n");

	stack->top = new;
}

void *top(stack_t* stack){
	assert(stack->top != NULL);

	return stack->top->data; //can return NULL?
}

void pop(stack_t *stack){
	assert(stack->top != NULL);

	stack_node_t* temp = stack->top; //saving the top node
	stack->top = temp->next; //setting the top node to the next node in the stack

	//free(temp->data);
	free(temp); //freeing the saved node and its data
}

int empty_stack(stack_t* stack){
	return stack->top == NULL ? 1 : 0;
}

void free_stack(stack_t* stack){
	while(stack->top != NULL){
		pop(stack); //should free every node
	}
	free(stack);
}
