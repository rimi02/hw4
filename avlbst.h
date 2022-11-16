#ifndef RBBST_H
#define RBBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
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
    virtual void insertFix(AVLNode<Key,Value>* n);
    virtual void removeFix(AVLNode<Key,Value>* n);
    virtual void rotateLeft(AVLNode<Key,Value>* n);
    virtual void rotateRight(AVLNode<Key,Value>* n);
    virtual void rotateLeftRight(AVLNode<Key,Value>* n);
    virtual void rotateRightLeft(AVLNode<Key,Value>* n);



};

/*
  -------------------------------------------------
  Begin implementations for the AVLTree class.
  -------------------------------------------------
*/

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    // Call the inherited insert function
    BinarySearchTree<Key, Value>::insert(new_item);

    // Get the node that was just inserted
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(new_item.first));

    // Call the insertFix function
    insertFix(n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key &key)
{
    // TODO
    // Call the inherited remove function
    BinarySearchTree<Key, Value>::remove(key);

    // Get the node that was just removed
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    // Call the removeFix function
    removeFix(n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value> *n)
{
    // TODO
    // If n is the root, set the balance to 0 and return
    if(n == this->root_){
        n->setBalance(0);
        return;
    }

    // Get the parent of n
    AVLNode<Key, Value>* parent = n->getParent();

    // If the parent's balance is 0, set the parent's balance to 1 or -1 depending on the
    // position of n and return
    if(parent->getBalance() == 0){
        if(parent->getLeft() == n){
            parent->setBalance(-1);
        }else{
            parent->setBalance(1);
        }
        return;
    }

    // If the parent's balance is 1 or -1, set the parent's balance to 0 and call insertFix
    // on the parent
    if(parent->getBalance() == 1 || parent->getBalance() == -1){
        parent->setBalance(0);
        insertFix(parent);
        return;
    }

    // If the parent's balance is 2 or -2, we need to rotate
    if(parent->getBalance() == 2 || parent->getBalance() == -2){
        // If the parent's balance is 2, we need to rotate left or left-right
        if(parent->getBalance() == 2){
            // If the parent's left child's balance is 1, we need to rotate left
            if(parent->getLeft()->getBalance() == 1){
                rotateLeft(parent);
            }else{
                // If the parent's left child's balance is -1, we need to rotate left-right
                rotateLeftRight(parent);
            }
        }else{
            // If the parent's balance is -2, we need to rotate right or right-left
            // If the parent's right child's balance is -1, we need to rotate right
            if(parent->getRight()->getBalance() == -1){
                rotateRight(parent);
            }else{
                // If the parent's right child's balance is 1, we need to rotate right-left
                rotateRightLeft(parent);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value> *n)
{
    // TODO
    // If n is the root, set the balance to 0 and return
    if(n == this->root_){
        n->setBalance(0);
        return;
    }

    // Get the parent of n
    AVLNode<Key, Value>* parent = n->getParent();

    // If the parent's balance is 0, set the parent's balance to 1 or -1 depending on the
    // position of n and return
    if(parent->getBalance() == 0){
        if(parent->getLeft() == n){
            parent->setBalance(1);
        }else{
            parent->setBalance(-1);
        }
        return;
    }

    // If the parent's balance is 1 or -1, set the parent's balance to 0 and call removeFix
    // on the parent
    if(parent->getBalance() == 1 || parent->getBalance() == -1){
        parent->setBalance(0);
        removeFix(parent);
        return;
    }

    // If the parent's balance is 2 or -2, we need to rotate
    if(parent->getBalance() == 2 || parent->getBalance() == -2){
        // If the parent's balance is 2, we need to rotate left or left-right
        if(parent->getBalance() == 2){
            // If the parent's left child's balance is 1, we need to rotate left
            if(parent->getLeft()->getBalance() == 1){
                rotateLeft(parent);
            }else{
                // If the parent's left child's balance is -1, we need to rotate left-right
                rotateLeftRight(parent);
            }
        }else{
            // If the parent's balance is -2, we need to rotate right or right-left
            // If the parent's right child's balance is -1, we need to rotate right
            if(parent->getRight()->getBalance() == -1){
                rotateRight(parent);
            }else{
                // If the parent's right child's balance is 1, we need to rotate right-left
                rotateRightLeft(parent);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value> *n)
{
    // TODO
    // Get the right child of n
    AVLNode<Key, Value>* right = n->getRight();

    // If the right child has a left child, set the right child's left child's parent to n
    if(right->getLeft() != NULL){
        right->getLeft()->setParent(n);
    }

    // Set the right child's left child to n
    right->setLeft(n);

    // Set the right child's parent to n's parent
    right->setParent(n->getParent());

    // If n is the root, set the right child to be the new root
    if(n == this->root_){
        this->root_ = right;
    }else{
        // If n is not the root, set n's parent's left or right child to be the right child
        // depending on the position of n
        if(n->getParent()->getLeft() == n){
            n->getParent()->setLeft(right);
        }else{
            n->getParent()->setRight(right);
        }
    }

    // Set n's parent to be the right child
    n->setParent(right);

    // Set n's right child to be the right child's left child
    n->setRight(right->getLeft());

    // Set the right child's balance to be 0
    right->setBalance(0);

    // Set n's balance to be 0
    n->setBalance(0);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value> *n)
{
    // TODO
    // Get the left child of n
    AVLNode<Key, Value>* left = n->getLeft();

    // If the left child has a right child, set the left child's right child's parent to n
    if(left->getRight() != NULL){
        left->getRight()->setParent(n);
    }

    // Set the left child's right child to n
    left->setRight(n);

    // Set the left child's parent to n's parent
    left->setParent(n->getParent());

    // If n is the root, set the left child to be the new root
    if(n == this->root_){
        this->root_ = left;
    }else{
        // If n is not the root, set n's parent's left or right child to be the left child
        // depending on the position of n
        if(n->getParent()->getLeft() == n){
            n->getParent()->setLeft(left);
        }else{
            n->getParent()->setRight(left);
        }
    }

    // Set n's parent to be the left child
    n->setParent(left);

    // Set n's left child to be the left child's right child
    n->setLeft(left->getRight());

    // Set the left child's balance to be 0
    left->setBalance(0);

    // Set n's balance to be 0
    n->setBalance(0);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeftRight(AVLNode<Key, Value> *n)
{
    // TODO
    // Get the left child of n
    AVLNode<Key, Value>* left = n->getLeft();

    // Get the left child's right child
    AVLNode<Key, Value>* leftRight = left->getRight();

    // If the left child's right child has a left child, set the left child's right child's
    // left child's parent to the left child
    if(leftRight->getLeft() != NULL){
        leftRight->getLeft()->setParent(left);
    }

    // Set the left child's right child's left child to the left child
    leftRight->setLeft(left);

    // Set the left child's right child's parent to n
    leftRight->setParent(n);

    // Set the left child's parent to the left child's right child
    left->setParent(leftRight);

    // Set the left child's right child to the left child's right child's left child
    left->setRight(leftRight->getLeft());

    // Set the left child's right child's left child to the left child
    leftRight->setLeft(left);

    // If the left child's right child's balance is 1, set the left child's balance to -1
    if(leftRight->getBalance() == 1){
        left->setBalance(-1);
    }else{
        // If the left child's right child's balance is not 1, set the left child's balance to 0
        left->setBalance(0);
    }

    // If the left child's right child's balance is -1, set the n's balance to 1
    if(leftRight->getBalance() == -1){
        n->setBalance(1);
    }else{
        // If the left child's right child's balance is not -1, set the n's balance to 0
        n->setBalance(0);
    }

    // Set the left child's right child's balance to 0
    leftRight->setBalance(0);

    // Rotate right on n
    rotateRight(n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRightLeft(AVLNode<Key, Value> *n)
{
    // TODO
    // Get the right child of n
    AVLNode<Key, Value>* right = n->getRight();

    // Get the right child's left child
    AVLNode<Key, Value>* rightLeft = right->getLeft();

    // If the right child's left child has a right child, set the right child's left child's
    // right child's parent to the right child
    if(rightLeft->getRight() != NULL){
        rightLeft->getRight()->setParent(right);
    }

    // Set the right child's left child's right child to the right child
    rightLeft->setRight(right);

    // Set the right child's left child's parent to n
    rightLeft->setParent(n);

    // Set the right child's parent to the right child's left child
    right->setParent(rightLeft);

    // Set the right child's left child to the right child's left child's right child
    right->setLeft(rightLeft->getRight());

    // Set the right child's left child's right child to the right child
    rightLeft->setRight(right);

    // If the right child's left child's balance is 1, set the right child's balance to 1
    if(rightLeft->getBalance() == 1){
        right->setBalance(1);
    }else{
        // If the right child's left child's balance is not 1, set the right child's balance to 0
        right->setBalance(0);
    }

    // If the right child's left child's balance is -1, set the n's balance to -1
    if(rightLeft->getBalance() == -1){
        n->setBalance(-1);
    }else{
        // If the right child's left child's balance is not -1, set the n's balance to 0
        n->setBalance(0);
    }

    // Set the right child's left child's balance to 0
    rightLeft->setBalance(0);

    // Rotate left on n
    rotateLeft(n);
}






#endif

