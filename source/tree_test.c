#include "tree_node.h"
#include <stdio.h>

int main(){
	tree_node_t* interior_test1 = make_interior(ALT_OP, "RAHH", NULL, NULL);
	tree_node_t* interior_test2 = make_interior(ALT_OP, "GAHH", interior_test1, NULL);

	tree_node_t* leaf_test = make_leaf(INTEGER, "BAHH");

	interior_node_t* node = (interior_node_t*)interior_test2->node; //I hope I find a better way to do this, but you have to cast the 'node' member of the node before you can access it
	printf("%s, %s\n", interior_test2->token, node->left->token);
	printf("%s\n", leaf_test->token);
	//GAHH, RAHH\n BAHH expected
	return 0;
}
