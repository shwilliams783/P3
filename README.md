Local Variables - statSem

statSem reads a file from the command line to be scanned into tokens by
parser(), which will produce a parse tree for the back end. After parser()
makes the parse tree, statSem() will validate static semantics, particularly
variable allocation and scoping.

Usage: 
	<code>./statSem P3_test1</code>
or:
	<code>./statSem < P3_test1.sp18</code>

Future projects will utilize the parse tree and static semantics created by
statSem to create the intermediate code (pseudo-Assembly) on the back end.

statSem.c and statSem.h

These source and header files utilize traversePostOrder(), push(), pop(),
find(), and statSemError() to traverse the parse tree created by parser(),
and verify the static semantics of .sp18 files.

traversePostOrder() uses a post order traversal of each of the nodes,
searching for <block> and <vars>/<mvars> to validate variable allocation using
a local scope, and also searches for <R>, <in>, and <assign> nodes to validate
variable usage after allocation.

push(), pop(), and find() are used to add, remove, and search for variables in
the varStack.

statSemError() is used for error reporting. 


*******************************Previous Versions*******************************

parser.c, parser.h and node.h

These source and header files utilize scanner() from previous projects to scan
tokens from the input file. By parsing through the tokens supplied by
scanner(), parser() and its accessory functions: program(), block(), vars(),
mvars(), expr(), M(), R(), stats(), mstats(), stat(), in(), out(), ifFunc(),
loop(), assign(), and RO() work together to represent a high-level language
BNF.

These accessory functions create a parse tree that stores both node types and
tokens, such as identifiers and integers in a parse tree. The nodes are created
using genNode(). The syntax of the language is verified by the BNF functions,
and any syntax errors are displayed by parseError(), along with the line the 
error occurred in, and what token was expected.

testTree.c and testTree.h

These source and header files contain a function to print the parse tree
created by parser(), including the types of nodes and tokens stored in the
tree. Each node is indented by 2 " " spaces times the depth that node falls
within the tree.
