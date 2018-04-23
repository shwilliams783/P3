#include <iostream>
#include <list>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <string>
#include "statsem.h"

std::string varStack[101]; // Variable Stack
int tos = 0; // Top of Stack
int varCount = 0;
static const int STACKMAX = 99; // varStack holds strings at indices 0-99

/* push() adds the given string to varStack, and monitors the capacity of 
   varStack, generating a statSemError() if tos >= STACKMAX. (varStack holds
   100 srtings maximum). A successful push() will also increment tos. */
void push(std::string str)
{
	// Check varStack capacity, add if under capacity
	if(tos < STACKMAX)
	{
		varStack[tos] = str;
		tos++;		
	}
	// Stack Overflow Error
	else
	{
		statSemError(1);
	}
}

/* pop() "removes" strings from varStack (by setting them to ""). A successful
   pop() will also decrement tos. */
void pop()
{
	// Remove string from tos
	varStack[tos] = "";
	if(tos > 0)
	{
		// Decrement tos
		tos--;
	}
}

/* find() returns -1 in the event that the given string, str, is not contained
   within varStack. If the string is found within varStack, its location
   relative to tos will be returned. find() works globally, so it's result is
   tested against varCount when testing for local variable allocation.  */
int find(std::string str)
{
	// Default to -1, (not found)
	int result = -1;
	int i;
	for(i = tos; i > 0; i--)
	{
		// If the provided string matches the ith value of varStack
		if(str.compare(varStack[i]) == 0)
		{
			// Set result to its location relative to tos, and break the loop
			result = tos - i;
			break;
		}
	}
	return result;
}

/* statSemError() takes a given error code, displays an error message to cerr
   describing the error, and exits the program. */
void statSemError(int errCode)
{
	switch(errCode)
	{
		case 1:
			// Stack overflow
			std::cerr<<"Error: Static semantics: varStack overflow."<<std::endl;
			exit(errCode);
		case 2:
			// Duplicate variable allocation in same scope
			std::cerr<<"Error: Static semantics: Duplicate variable found in scope."<<std::endl;
			exit(errCode);
		case 3:
			// Undleclares variable
			std::cerr<<"Error: Static semantics: Undeclared variable."<<std::endl;
			exit(errCode);
		default:
			// Unknown other error
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
			statSemError(3);
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