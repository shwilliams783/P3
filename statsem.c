#include <iostream>
#include <list>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <string>
#include "statsem.h"

std::string varStack[101];
int tos = 0;
int varCount = 0;
static const int STACKMAX = 99;

void push(std::string str)
{
	if(tos < STACKMAX)
	{
		varStack[tos] = str;
		tos++;		
	}
	else
	{
		statSemError(1);
	}
}

void pop()
{
	varStack[tos] = "";
	if(tos > 0)
	{
		tos--;
	}
}

int find(std::string str)
{
	int result = -1;
	int i;
	for(i = tos; i > 0; i--)
	{
		if(str.compare(varStack[i]) == 0)
		{
			result = tos - i;
			break;
		}
	}
	return result;
}

void statSemError(int errCode)
{
	switch(errCode)
	{
		case 1:
			std::cerr<<"Error: Static semantics: varStack overflow."<<std::endl;
			exit(errCode);
		case 2:
			std::cerr<<"Error: Static semantics: Duplicate variable found in scope."<<std::endl;
			exit(errCode);
		case 3:
			std::cerr<<"Error: Static semantics: Undeclared variable."<<std::endl;
			exit(errCode);
		default:
			std::cerr<<"Error: Static semantics"<<std::endl;
			exit(errCode);
	}
}

/*	traversePostorder() outputs the node types and tokens contained in each node of the tree
	in the order: root (self), child0, child1, child2, child3.*/
void traversePostorder(node_t* root)
{
	int i;
	
	// Preserve previous varCount 
	int previousVarCount = varCount;
	
	// Resolve children, left to right
	
	// Variable Scope: <block> open
	if(root->type == blocND)
	{
		std::cout<<"<block> open, resetting varCount from "<<previousVarCount<<" to "<<varCount<<std::endl; // DELETE AFTER DEBUGGING
		varCount = 0;
	}
	
	// Variable Allocation: <vars>/<mvars> open
	if(root->type == varsND || root->type == mvarND)
	{
		std::cout<<"<vars>/<mvars> open"<<std::endl; // DELETE AFTER DEBUGGING
		// Check if node contains an identifier
		if(root->tokens[0].str.compare("NULL") != 0)
		{	
			std::cout<<"Identifier: "<<root->tokens[0].str<<" found"<<std::endl; // DELETE AFTER DEBUGGING
			// Check to see if the Identifier is already defined in this scope
			if(find(root->tokens[0].str) <= varCount && find(root->tokens[0].str) >= 0)
			{
				statSemError(2);
			}
			else
			{
				std::cout<<"added "<<root->tokens[0].str<<" to varStack"<<std::endl; // DELETE AFTER DEBUGGING
				push(root->tokens[0].str);
				varCount++;
			}
		}
		// Terminal <mvars> node
		else
		{
			std::cout<<"Terminal <mvars>"<<std::endl; // DELETE AFTER DEBUGGING
		}
	}
	
	// Variable Usage: <R>, <in>, or <assign> open
	if(root->type == RND || root->type == inND || root->type == asgnND)
	{
		std::cout<<"<R>/<in>/<assign> open"<<std::endl; // DELETE AFTER DEBUGGING
		// IDTK found locally
		if(find(root->tokens[0].str) < varCount && find(root->tokens[0].str) >= 0)
		{
			std::cout<<root->tokens[0].str<<" found at "<<find(root->tokens[0].str)<<std::endl; // DELETE AFTER DEBUGGING
		}
		// IDTK found globally
		else if(find(root->tokens[0].str) >= 0)
		{
			std::cout<<root->tokens[0].str<<" found globally at "<<find(root->tokens[0].str)<<std::endl; // DELETE AFTER DEBUGGING// DEBUG COUT
		}
		// <R> contains int or nothing -> (<expr>)
		else if(root->tokens[0].id == IntTK || root->tokens[0].str.compare("NULL") == 0)
		{
			std::cout<<"<R> contains Int or NULL."<<std::endl; // DELETE AFTER DEBUGGING
		}
		else
		{
			statSemError(3); // ADD ME TO statSemError()!!!
		}
	}
	
	
	/* Resolve child0 */
	if(root->child0 != NULL)
	{
		traversePostorder(root->child0);
	}
	/* Resolve child1 */
	if(root->child1 != NULL)
	{
		traversePostorder(root->child1);
	}
	/* Resolve child2 */
	if(root->child2 != NULL)
	{
		traversePostorder(root->child2);
	}
	/* Resolve child3 */
	if(root->child3 != NULL)
	{
		traversePostorder(root->child3);
	}
	
	/* Resolve the root */
	
	// Scope Closure: <block> closure
	if(root->type == blocND)
	{
		std::cout<<"<block> close, resetting varCount from "<<varCount<<" to "<<previousVarCount<<std::endl; // DELETE AFTER DEBUGGING
		std::cout<<"tos = "<<tos<<std::endl; // DELETE AFTER DEBUGGING
		// Pop() varCount variables off of the varStack
		for(i = 0; i < varCount; i++)
		{
			pop();
			std::cout<<"pop() called, tos = "<<tos<<std::endl; // DELETE AFTER DEBUGGING
		}
		std::cout<<"tos = "<<tos<<std::endl; // DELETE AFTER DEBUGGING
		// Reset varCount to previousVarCount before closing <block>
		varCount = previousVarCount;
	}
	
}