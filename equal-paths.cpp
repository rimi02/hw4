#include "equal-paths.h"
using namespace std;

int getDepth(Node* root){
    if(root == NULL){
        return 0;
    }
    int left = getDepth(root->left);
    int right = getDepth(root->right);
    if(left > right){
        return left + 1;
    }
    return right + 1;
}

bool equalPaths(Node* root){
    if(root == NULL){
        return true;
    }
    if(root->left == NULL && root->right == NULL){
        return true;
    }
    if(root->left == NULL){
        return getDepth(root->right) == 1;
    }
    if(root->right == NULL){
        return getDepth(root->left) == 1;
    }
    return getDepth(root->left) == getDepth(root->right) && equalPaths(root->left) && equalPaths(root->right);
}