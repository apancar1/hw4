#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
// this is for max 
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int count (Node* root)
// MODIFY THIS FUNCTION TO APPLY TO THIS GOAL NOW
{
	if (root == NULL){
		return 0;
		// tree is empty / base case 
	}
	else{
		//eturn 1 + count(root->left) + count(root->right);
		// this gives all nodes in tree (THIS WAS FROM LECTURE)
		// to modify to apply to this case: look for the longest from the left / right and use that 
		return 1 + max(count(root->left), count(root->right));
	}
	// this function should be correct (slight modification from lecture)
}


bool equalPaths(Node * root)
{
    // Add your code below
		// general plan 
		// terminate when one is not the same as the rest - return false 
		// base case: end when there is no child 
		//

		// given guiding principle: recursive code should hangle only 1 element
		// recursion to handle rest of elements 

		// recurisve tree traversal starting with root:
		// if t == NULL return 
		// process(t) print val
		// preorder(t->left)
		// preorder(t->right)

		// use count helper function! (from lecture slides)
		// The function from slides simply counts all the nodes in a tree 
		// should use the count to count how long a path is
		if (root == NULL){
			return true; 
			// if it has no children then it is true 
		}

		if ((root->left == NULL) && (root->right == NULL)){
			return true;
			// if it is a leaf node then it is also true (recursion based)
		}

		if ((root->left != NULL) && (root->right != NULL)){
			// put all code into this if statement because it will execute only if there are both existing 
			// this is so it doesnt return false when it should not 
			// this is bc you should only compare lengths when there is both children 
			// otherwise if there is only one child then it will not be correct anyways
			int leftlen;
			int rightlen; 
			leftlen = count(root->left);
			rightlen = count(root->right);

			if (leftlen != rightlen){
				return false; 
				// this is when recursion breaks (first case of non equal)
			}
		}
		
		// recursive call 
		// recursion should check for both left & right 
		// this means it will be and statement 
		return equalPaths(root->left) && equalPaths(root->right);
}

