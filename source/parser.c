#include "parser.h"
#include "symtab.h" //can't remember if its in parser.h, I think so but just in case
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

parse_error_t parse_error = PARSE_NONE;
eval_error_t eval_error = EVAL_NONE;

void cleanup_tree(tree_node_t* node){
	if(node->type == INTERIOR){
		interior_node_t* int_node = (interior_node_t*)node->node;
		cleanup_tree(int_node->left);
		cleanup_tree(int_node->right);
	}
	
	//free(node->token);
	free(node->node);
	free(node);
}	

op_type_t is_op(char* token){
	op_type_t op = NO_OP;
	//I would like to do a swtich statement but I don't think that would work here
	
	if(!strcmp(token, "+")){
		op = ADD_OP;
	}
	else if(!strcmp(token, "-")){
		op = SUB_OP;
	}
	else if(!strcmp(token, "*")){
		op = MUL_OP;
	}
	else if(!strcmp(token, "/")){
		op = DIV_OP;
	}
	else if(!strcmp(token, "%")){
		op = MOD_OP;
	}
	else if(!strcmp(token, "=")){
		op = ASSIGN_OP;
	}
	else if(!strcmp(token, "?")){
		op = Q_OP;
	}
	//: token will never happen in postfix apparently

	return op;
}

tree_node_t* parse(stack_t* stack){
	
	char* token = top(stack);
	pop(stack);
	op_type_t operation = NO_OP;
	if((operation = is_op(token)) != NO_OP){
		tree_node_t* root = make_interior(operation, token, NULL, NULL);
		tree_node_t* right;
		tree_node_t* left;
		if(operation == Q_OP){ //gotta do freaky stuff for the Q_OP
			right = make_interior(ALT_OP, ":", parse(stack), parse(stack)); //first and second on stack
			left = parse(stack); //third on stack
		}
		else{
			right = parse(stack); //annoyingly you have to do it seperately
			left = parse(stack); //else it would be a leftmost derivation
		}

		interior_node_t* root_node = (interior_node_t*)root->node; //gotta cast it cuz its a void*
		root_node->right = right;
		root_node->left = left;
		
		if(root_node->left == NULL || root_node->right == NULL){ //if there's an interior node with a child missing then there weren't enough tokens
			parse_error = TOO_FEW_TOKENS;
		}

		root->node = root_node; //just in case

		return root;
	}
	else if(atoi(token) != 0 || *(token) == '0'){ //if the token was 0, it would evaluate incorrectly
		tree_node_t* root = make_leaf(INTEGER, token);
		return root;
	}
	else{
		tree_node_t* root = make_leaf(SYMBOL, token); 
		return root;
	}

	//error handling
	if(!empty_stack(stack)){
		parse_error = TOO_MANY_TOKENS;
	}
	
}

tree_node_t* make_parse_tree(char* expr){
	stack_t* stack = make_stack();
	
	//char* copy = (char*)calloc(strlen(expr) + 1, sizeof(char));
	//strcpy(copy, expr);

	char* token = strtok(expr, " \n"); //Gotta copy for some reason, something about string literals?
	while(token != NULL){
		push(stack, token);
		token = strtok(NULL, " \n");
	}
	
	tree_node_t* tree = parse(stack);
	
	if(parse_error == TOO_FEW_TOKENS){
		fprintf(stderr, "Invalid expression, not enough tokens\n"); 
		cleanup_tree(tree);
	}
	else if(parse_error == TOO_MANY_TOKENS){
		fprintf(stderr, "Invald expression, too many tokens\n");
		cleanup_tree(tree);
	}

	free_stack(stack);
	//free(copy);
	return tree;
}

void print_infix(tree_node_t* root){
	if(root->type == INTERIOR){
		interior_node_t* root_node = (interior_node_t*)root->node;
		printf("(");
		print_infix(root_node->left);
		printf("%s", root->token);
		print_infix(root_node->right);
		printf(")");
	}
	else{
		printf("%s", root->token);
	}
}

int eval_tree(tree_node_t* node){
	if(node->type == LEAF){
		leaf_node_t* leaf_node = (leaf_node_t*)node->node;
		exp_type_t leaf_type = leaf_node->exp_type;
		if(leaf_type == INTEGER){
			char* token = node->token;
			return atoi(token);
		}
		else{
			symbol_t* symbol = lookup_table(node->token);

			return symbol->val;
		}
	}
	else{
		interior_node_t* int_node = (interior_node_t*)node->node;
		op_type_t op = int_node->op;
		tree_node_t* right = int_node->right;
		tree_node_t* left = int_node->left;
		int rgh; //for division and modulus operators
		int lef; //for ternary operator
		int result;

		switch(op){ //TODO edge case errors
			case ADD_OP:
				result = eval_tree(left) + eval_tree(right);
				break;
			case SUB_OP:
				result = eval_tree(left) - eval_tree(right);
				break;
			case MUL_OP:
				result = eval_tree(left) * eval_tree(right);
				break;
			case DIV_OP:
				rgh = eval_tree(right);
				if(rgh == 0){
					eval_error = DIVISION_BY_ZERO;
					return 0;
				}
				result = eval_tree(left) / rgh;
				break;
			case MOD_OP:
				rgh = eval_tree(right);
				if(rgh == 0){
					eval_error = DIVISION_BY_ZERO;
					return 0;
				}
				result = eval_tree(left) % rgh;
				break;
			case ASSIGN_OP:
				if(left->type != LEAF && ((leaf_node_t*)((*left).node))->exp_type != SYMBOL){
					eval_error = INVALID_LVALUE;
					return 0;
				}
				symbol_t* symbol = lookup_table(left->token);
				if(symbol == NULL){
					symbol = create_symbol(left->token, 0);	
				}
				symbol->val = eval_tree(right);
				result = symbol->val;
				break;
			case Q_OP:
				lef = eval_tree(left);
				interior_node_t* right_int = (interior_node_t*)right->node; //should be an interior node with token ":" due to how ternarys are handled
				if(lef != 0){
					result = eval_tree(right_int->left); //left of the right node
				}
				else{
					result = eval_tree(right_int->right);
				}
				break;
			default:
				result = 0;
				eval_error = UNKNOWN_OPERATION; //idk what else to call it
				break;
		}
		return result;
	}
}

void rep(char *exp){
	char* copy = (char*)calloc(strlen(exp) + 1, sizeof(char)); //copying string incase of read-only
	strcpy(copy, exp);

	tree_node_t* root = make_parse_tree(exp); //make
	if(root == NULL){
		return;	
	}
	if(eval_error == EVAL_NONE){
		print_infix(root); //print (does not include \n
		int result = eval_tree(root); //evaluate
		//print eval error message?
		printf(" = %d\n", result);
	}
	else{ //this is lazy, but the only error you check for is divide by zero, so that's the onlything I'm printing
		//switch statment on eval_error goes here
		fprintf(stderr, "Division by zero\n");
	}
	cleanup_tree(root);
	free(copy);
}
