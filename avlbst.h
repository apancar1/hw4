#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node); 
    void removeFix(AVLNode<Key,Value>* inputNode, int diff); 
    void rotateRight(AVLNode<Key,Value>* node); 
    void rotateLeft(AVLNode<Key,Value>* node); 


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // AVL tree is BST where height diff btwn left & right is at least 1 
    // balance = right-left where it can be -1,0,1
    // psuedocode: 
    // if empty tree, set n as root, b(n) = 0
    if (this->root_ == nullptr){
        this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
        //static_cast<AVLNode<Key,Value>*>(this->root_)->setBalance(0);
        return; 
    }
    // else insert n (by walking the tree to a leaf & inserting new node as its child)
    //BinarySearchTree<Key,Value>::insert(new_item);

    AVLNode<Key,Value>* avlnode = static_cast<AVLNode<Key,Value>*>(this->root_);
		AVLNode<Key,Value>* parent = avlnode;
		if (avlnode == nullptr){
			return; 
		}
    // BST INSERT FUNCTION DOES UP UNTIL HERE: 
    // set balance to 0 then look at its parent, p 
    //avlnode->setBalance(0); 
    // to do this, must change the insertedNode type 
    // see if it was root first 
    if (parent == nullptr){
        return; 
    }
		// copy the implementation of insert from BST as follows: 
		while (avlnode != nullptr){
			parent = avlnode; 
			if (new_item.first > avlnode->getKey()){
				avlnode = avlnode->getRight(); 
			}
			else if (new_item.first < avlnode->getKey()){
				avlnode = avlnode->getLeft(); 
			}
			else{
				avlnode->setValue(new_item.second);
				return; 
			}
		}

		AVLNode<Key,Value>* tempnode = new AVLNode<Key,Value>(new_item.first, new_item.second, parent);
		if (new_item.first < parent->getKey()){
			parent->setLeft(tempnode);
			parent->updateBalance(-1);
		}
		else{
			parent->setRight(tempnode);
			parent->updateBalance(1);
		}
		if (parent->getBalance() != 0){
			insertFix(parent, tempnode);
		}
    // now check if left or right 
		//insertFix(parent, avlnode);
    // if (parent->getLeft() == avlnode){
    //     // if b(p) was -1, then b(p) = 0
    //     if (parent->getBalance() == -1){
    //         parent->setBalance(0);
    //         return; 
    //     }
    //     // if b(p) was +1, then b(p) = 0
    //     else if (parent->getBalance() == 1){
    //         parent->setBalance(0);
    //         return; 
    //     }
    //     // if b(p) was 0, then update b(p) and call insert-fix(p,n)?
    //     else if (parent->getBalance() == 0){
    //         parent->setBalance(-1);
    //         insertFix(parent, avlnode); 
    //     }
    // }
    // else{
    //     // only change to 1 instead of -1 here:  
    //     if (parent->getBalance() == -1){
    //         parent->setBalance(0);
    //         return; 
    //     }
    //     // if b(p) was +1, then b(p) = 0
    //     else if (parent->getBalance() == 1){
    //         parent->setBalance(0);
    //         return; 
    //     }
    //     // if b(p) was 0, then update b(p) and call insert-fix(p,n)?
    //     else if (parent->getBalance() == 0){
    //         parent->setBalance(1);
    //         insertFix(parent, avlnode); 
    //     }
    // }
}


// insertFix helper: 
template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node)
{
    // insertFix part: 
    // get parent
    // insert fix part 
    // precondition: p and n are balanced: {-1,0,1}
    // postcondition: g,p, and n are balanced: {-1,0,1}
    // if p is null or parent(p) is null, return 
    if (parent == nullptr){
        return; 
    }
		if (parent->getParent() == nullptr){
			return; 
		}
    AVLNode<Key,Value>* grand = parent->getParent();
    if (grand == nullptr){
        return; 
    }
    // let g = parent(p)
    // assume p is left child of g (for right child swap left/right and +/-)
    // update g's balance: b(g) += -1 
    if (grand->getLeft() == parent){
        grand->updateBalance(-1); 
        // case 1: b(g) == 0 return 
        if (grand->getBalance() == 0){
            return; 
        }
        // case 2: b(g) == 1, insertFix(g,p) --> recursion! 
        else if (grand->getBalance() == -1){
            insertFix(grand, parent);
        }
        // case 3: b(g) == 2
        else if (grand->getBalance() == -2){
            // if zig-zig then rotateRight(g), b(p) = b(g) = 0
            if (parent->getLeft() == node){
                rotateRight(grand);
                parent->setBalance(0);
                grand->setBalance(0); 
            }
            // if zig-zag then rotateLeft(p), rotateRight(g)
            else{
                rotateLeft(parent);
                rotateRight(grand);
                // case 3a: b(n) == -1 then b(p) = 0, b(g) = +1, b(n) = 0
                if (node->getBalance() == -1){
                    parent->setBalance(0);
                    grand->setBalance(1);
                    node->setBalance(0);
                }
                // case 3b: b(n) == 0 then b(p) = 0, b(g) = 0; b(n) = 0 
                else if(node->getBalance() == 0){
                    parent->setBalance(0);
                    grand->setBalance(0);
                    node->setBalance(0);
                }
                // case 3c: b(n) == +1, then b(p) = -1, b(g) = 0, b(n) = 0 
                else if(node->getBalance() == 1){
                    parent->setBalance(-1);
                    grand->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }
    else{
        //MIRROR WHAT IS ABOVE 
        grand->updateBalance(1); 
        // case 1: b(g) == 0 return 
        if (grand->getBalance() == 0){
            return; 
        }
        // case 2: b(g) == 1, insertFix(g,p) --> recursion! 
        else if (grand->getBalance() == 1){
            insertFix(grand, parent);
        }
        // case 3: b(g) == 2
        else if (grand->getBalance() == 2){
            // if zig-zig then rotateRight(g), b(p) = b(g) = 0
            if (parent->getRight() == node){
                rotateLeft(grand);
                parent->setBalance(0);
                grand->setBalance(0); 
            }
            // if zig-zag then rotateLeft(p), rotateRight(g)
            else{
                rotateRight(parent);
                rotateLeft(grand);
                // case 3a: b(n) == -1 then b(p) = 0, b(g) = +1, b(n) = 0
                if (node->getBalance() == 1){
                    parent->setBalance(0);
                    grand->setBalance(-1);
                    node->setBalance(0);
                }
                // case 3b: b(n) == 0 then b(p) = 0, b(g) = 0; b(n) = 0 
                else if(node->getBalance() == 0){
                    parent->setBalance(0);
                    grand->setBalance(0);
                    node->setBalance(0);
                }
                // case 3c: b(n) == +1, then b(p) = -1, b(g) = 0, b(n) = 0 
                else if(node->getBalance() == -1){
                    parent->setBalance(1);
                    grand->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }
    // general idea: work up ancestor chain updating balances of the ancestor chain or fix a node that is out of balance 
    // if you perform a rotation to fix a node that is out of balance, you will not need to recurse. you are done 
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    // find node to remove by walking the tree 
    // if n has 2 children swap pos with in-order successor and perform the next step
		if (this->internalFind(key) == nullptr){
			return; 
		} 
    // BinarySearchTree<Key,Value>::remove(key);
    // let p = parent(n)
    AVLNode<Key,Value>* AVLNODE = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
		if (AVLNODE == nullptr){
			return; 
		}
    // if p is not NULL:
    if (AVLNODE != nullptr && AVLNODE->getLeft() != nullptr && AVLNODE->getRight() != nullptr){
        AVLNode<Key,Value>* pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(AVLNODE));
				if (pred != nullptr){
					//AVLNode<Key,Value>* converted = static_cast<AVLNode<Key,Value>*>(pred);
					nodeSwap(pred, AVLNODE);
				}
    }
    // if n is a left child, let diff = +1 
    // if n is a left child to be removed, the right subtree now has greater height, add diff = +1 to balance of its parent 
    // if n is a right child, let diff = -1
    // if n is a right child to be removed, the left subtree now has greater height, so add diff = -1 to balance of its parent 
    AVLNode<Key,Value>* parent = AVLNODE->getParent();
		AVLNode<Key,Value>* child = nullptr; 
		if (AVLNODE->getLeft() != nullptr){
			child = static_cast<AVLNode<Key,Value>*>(AVLNODE->getLeft());
		}
		else{
			child = static_cast<AVLNode<Key,Value>*>(AVLNODE->getRight());
		}
		if (child != nullptr){
			child->setParent(parent);
		}
		bool isleft = true; 
		if (parent == nullptr){
			this->root_ = child; 
		}
		else{
			if(AVLNODE == parent->getLeft()){
				parent->setLeft(child);
			}
			else{
				parent->setRight(child);
				isleft = false; 
			}
		}

		delete AVLNODE; 
		removeFix(parent, (isleft ? 1 : -1));
		// int diff; 
		// if (parent == nullptr){
		// 	diff = 0; 
		// }
		// else if (AVLNODE == parent->getLeft()){
		// 	diff = 1; 
		// }
		// else{
		// 	diff = -1; 
		// }
		// if (parent != nullptr){
		// 	removeFix(parent, diff);
		// }
    // int diff = 0; 
		// if (parent == nullptr){
		// 	return; 
		// }
    // if (AVLNODE == parent->getLeft()){
    //     diff = 1; 
    // }
    // else{
    //     diff = -1; 
    // }
    // // diff will be the amount added to updated the balance of p 
    // // delete n and update points 
    // BinarySearchTree<Key,Value>::remove(key);
    // // patch tree by calling removeFix(p,diff)? 
    // removeFix(parent, diff); 
}


template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* inputNode, int diff)
{
    // removeFix(n,diff)
    // if n is null return 
    if (inputNode == nullptr){
        return; 
    }
    // computer next recursive call's arguments before alterning the tree:
    // let p = parent(n) and if p is not NULL let ndiff(nextdiff) = +1 if n is a left child and -1 otherwise 
    AVLNode<Key,Value>* parent = inputNode->getParent(); 
    int nextDiff = 0; 
    // assume diff = -1 and follow rest of psuedocode (if it is diff = +1, mirror the rest of the code)
    if (parent != nullptr){
        if (parent->getLeft() == inputNode){
            nextDiff = 1; 
        }
        else{
            nextDiff = -1; 
        }
    }
    // case 1: b(n) + diff == -2 
    if (inputNode->getBalance() + diff == -2){
        // let c = left(n), the taller of the children 
        AVLNode<Key,Value>* child = inputNode->getLeft(); 
				if (child != nullptr){ 
					  if (child->getBalance() == -1){
            // zig-zig case 
            // case 1a: b(c) == -1 --> zig-zig case 
            // rotateRight(n), b(n) = b(c) = 0, removeFix(p, ndiff)
            rotateRight(inputNode);
            inputNode->setBalance(0);
            child->setBalance(0);
            removeFix(parent, nextDiff);
        	}
					else if (child->getBalance() == 0){
							// case 1b: b(c) == 0 --> zig-zig case 
							// rotateRight(n), b(n) = -1, b(c) = +1 
							rotateRight(inputNode);
							inputNode->setBalance(-1);
							child->setBalance(1); 
							return; 
					}
					else if (child->getBalance() == 1){
							// case 1c: b(c) == +1 --> zig-zag case 
							// let g = right(c) 
							AVLNode<Key,Value>* grand = child->getRight(); 
							// rotateLeft(c) then rotateRight(n)
							rotateLeft(child);
							rotateRight(inputNode);
							// if b(g) was +1 then b(n) = 0, b(c) = -1, b(g) = 0
							if (grand->getBalance() == 1){
									inputNode->setBalance(0); 
									child->setBalance(-1);
									grand->setBalance(0);
							}
							// if b(g) was 0 then b(n) = 0, b(c) = 0, b(g) = 0
							else if (grand->getBalance() == 0){
									inputNode->setBalance(0);
									child->setBalance(0);
									grand->setBalance(0); 
							}
							// if b(g) was -1 then b(n) = +1, b(c) = 0, b(g) = 0
							else if (grand->getBalance() == -1){
									inputNode->setBalance(1);
									child->setBalance(0);
									grand->setBalance(0); 
							}
							removeFix(parent, nextDiff);
							return; 
							// removeFix(p, ndiff)
					}
				}
    	} 
    // perform the check for the mirror case where b(b) + diff == +2, flipping left/right and -1/+1
    // this is the same as above but mirrored 
    else if (inputNode->getBalance() + diff == 2){
        AVLNode<Key,Value>* child = inputNode->getRight(); 
				if (child != nullptr){
					  if (child->getBalance() == 1){
            // zig-zig case 
            // case 1a: b(c) == -1 --> zig-zig case 
            // rotateRight(n), b(n) = b(c) = 0, removeFix(p, ndiff)
            rotateLeft(inputNode);
            inputNode->setBalance(0);
            child->setBalance(0);
            removeFix(parent, nextDiff);
        	}
					else if (child->getBalance() == 0){
							// case 1b: b(c) == 0 --> zig-zig case 
							// rotateRight(n), b(n) = -1, b(c) = +1 
							rotateLeft(inputNode);
							inputNode->setBalance(1);
							child->setBalance(-1); 
							return; 
					}
					else if (child->getBalance() == -1){
							// case 1c: b(c) == +1 --> zig-zag case 
							// let g = right(c) 
							AVLNode<Key,Value>* grand = child->getLeft(); 
							// rotateLeft(c) then rotateRight(n)
							rotateRight(child);
							rotateLeft(inputNode);
							// if b(g) was +1 then b(n) = 0, b(c) = -1, b(g) = 0
							if (grand->getBalance() == -1){
									inputNode->setBalance(0); 
									child->setBalance(1);
									grand->setBalance(0);
							}
							// if b(g) was 0 then b(n) = 0, b(c) = 0, b(g) = 0
							else if (grand->getBalance() == 0){
									inputNode->setBalance(0);
									child->setBalance(0);
									grand->setBalance(0); 
							}
							// if b(g) was -1 then b(n) = +1, b(c) = 0, b(g) = 0
							else if (grand->getBalance() == 1){
									inputNode->setBalance(-1);
									child->setBalance(0);
									grand->setBalance(0); 
							}
							removeFix(parent, nextDiff);
							return; 
							// removeFix(p, ndiff)
					}
			}
    }
    // case 2: b(n) + diff == -1: then b(n) = -1
    else if (inputNode->getBalance() + diff == -1){
			inputNode->setBalance(-1);
			return; 
		}
		else if (inputNode->getBalance() + diff == 1){
			inputNode->setBalance(1);
			return; 
		}
    // case 3: b(n) + diff == 0: then b(n) = 0, removeFix(p, ndiff)
    else if (inputNode->getBalance() + diff == 0){
        inputNode->setBalance(0);
        removeFix(parent, nextDiff);
				return; 
    }
    // note: 
    // p = parent of n 
    // n = current node 
    // c = taller child of n 
    // g = grandchild of n 
}


// need to implement the rotate ones now 

template<class Key, class Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* node)
{
    if (node == nullptr){
        return; 
    }
    // right rotation
    // take a left child 
    AVLNode<Key,Value>* leftChild = node->getLeft(); 
    if (leftChild == nullptr){
        return; 
    }
    //AVLNode<Key, Value>* parent = node->getParent(); 
    // move the right subtree to the node's left 
    node->setLeft(leftChild->getRight());
    if (leftChild->getRight() != nullptr){
        leftChild->getRight()->setParent(node); 
    }
    // make the right child 
    // leftChild->setRight(node);
    // node->setParent(leftChild);
    // make it the parent 
    leftChild->setParent(node->getParent());
		if (leftChild->getParent() == nullptr){
			this->root_ = leftChild;
		}
		else if (node == node->getParent()->getLeft()){
			node->getParent()->setLeft(leftChild);
		}
		else{
			node->getParent()->setRight(leftChild);
		}
    // make the original parent the new right child 
    // if (parent == nullptr){
    //     this->root_ = leftChild; 
    // }
    // else{
    //     if (parent->getLeft() == node){
    //         parent->setLeft(leftChild);
    //     }
    //     else{
    //         parent->setRight(leftChild); 
    //     }
    // }
		leftChild->setRight(node);
		node->setParent(leftChild);
}


template<class Key, class Value>
void AVLTree<Key,Value>::rotateLeft(AVLNode<Key,Value>* node)
{
    // left rotation 
    // similar implementation as above (i just inversed it)
    // take a right child 
    // make it the parent 
    // make the original parent the new left child 
    if (node == nullptr){
        return; 
    }
    // right rotation
    // take a left child 
    AVLNode<Key,Value>* rightChild = node->getRight(); 
		//AVLNode<Key,Value>* leftChild = node->getLeft(); 
    if (rightChild == nullptr){
        return; 
    }
    //AVLNode<Key, Value>* parent = node->getParent(); 
    // move the right subtree to the node's left 
    node->setRight(rightChild->getLeft());
    if (rightChild->getLeft() != nullptr){
        rightChild->getLeft()->setParent(node); 
    }
    // make the right child 
    rightChild->setParent(node->getParent());
		if (rightChild->getParent() == nullptr){
			this->root_ = rightChild;
		}
		else if(node == node->getParent()->getLeft()){
			node->getParent()->setLeft(rightChild);
		}
		else{
			node->getParent()->setRight(rightChild);
		}
		rightChild->setLeft(node);
    node->setParent(rightChild);
    // make it the parent 
    // rightChild->setParent(parent);
    // // make the original parent the new right child 
    // if (parent == nullptr){
    //     this->root_ = rightChild; 
    // }
    // else{
    //     if (parent->getLeft() == node){
    //         parent->setLeft(rightChild);
    //     }
    //     else{
    //         parent->setRight(rightChild); 
    //     }
    // }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
