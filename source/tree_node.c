#include "tree_node.h"
#include <stdlib.h>

tree_node_t* make_interior(op_type_t op, char* token, tree_node_t* left, tree_node_t* right){
	tree_node_t* root = (tree_node_t*)calloc(1, sizeof(tree_node_t)); //node ADT base
	root->token = token;
	root->type = INTERIOR;

	interior_node_t* interior = (interior_node_t*)calloc(1, sizeof(interior_node_t)); //interior node data
	interior->op = op;
	interior->left = left;
	interior->right = right;

	root->node = (void*)interior;

	return root;
}

tree_node_t* make_leaf(exp_type_t exp_type, char* token){
	tree_node_t* root = (tree_node_t*)calloc(1, sizeof(tree_node_t)); //node ADT base
	root->token = token;
	root->type = LEAF;

	leaf_node_t* leaf = (leaf_node_t*)calloc(1, sizeof(leaf_node_t)); //leaf node data
	leaf->exp_type = exp_type;

	root->node = (void*)leaf;

	return root;
}
