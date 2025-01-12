#include "parser.h"
#include "symtab.h"

int main(){
	build_table("./sym.1");
	dump_table();
	rep("3 2 +");
	rep("b a 2 + -"); //b - a + 2 = 8
	rep("b 100 ="); //b = 100
	rep("b a 2 + -"); //b - a + 2 = 88
	rep("b 2 /"); //b/2 = 50
	rep("0 2 3 ?"); //0?2:3 = 3
	rep("1 2 3 ?"); //1?2:3 = 2
	free_table();

}
