// Alexa Gomez, apg2hv, AVLTree.cpp, 03/01/19
#include "AVLTree.h"
#include <string>
#include "AVLNode.h"
#include <iostream>
using namespace std;

AVLTree::AVLTree() { root = NULL; }

AVLTree::~AVLTree() {
  delete root;
  root = NULL;
}

// helper method for insert
void AVLTree::insert(const string& x, AVLNode*& curNode){
  if(curNode == NULL){
    curNode  = new AVLNode();
    curNode->value = x;
  }
  if(x < curNode->value){
    insert(x, curNode->left);

  }
  else if(x > curNode->value){

    insert(x, curNode->right);

  }

  curNode->height = 1 + max(height(curNode->left), height(curNode->right));
  balance(curNode);

  

}


// insert finds a position for x in the tree and places it there, rebalancing
// as necessary.
void AVLTree::insert(const string& x) {
  insert(x, root);
  
}

// remove finds x's position in the tree and removes it, rebalancing as
// necessary.
void AVLTree::remove(const string& x) { root = remove(root, x); }

// pathTo finds x in the tree and returns a string representing the path it
// took to get there.
string AVLTree::pathTo(const string& x) const {
  string listNodes = "";
  if(find(x)==false){
    return listNodes;
  }
  else if(root == NULL){
    return listNodes;
  }
  else if(x == root->value){
    listNodes = (listNodes  + root->value);
    return listNodes;
  }
  // x is less than the root
  else if(x < root->value){
    listNodes = (listNodes + root->value + " ");
    AVLTree *tree = new AVLTree();
    tree->root = root->left;
    return listNodes + tree->pathTo(x);
  }
  // greater than the root
  else if(x > root->value){
    listNodes = (listNodes + root->value + " ");
    AVLTree *tree = new AVLTree();
    tree->root = root->right;
    return listNodes + tree->pathTo(x);
  }
  // can not find the item
  return "";
}

// find determines whether or not x exists in the tree.
bool AVLTree::find(const string& x) const {
    if(root == NULL){
    return false;
  }
   else if(x == root->value){
    return true;
  }
  // x is less than the root
  else if(x < root->value){
    AVLTree *tree = new AVLTree();
    tree->root = root->left;
    return tree->find(x);
  }
  // greater than the root
  else if(x > root->value){
    AVLTree *tree = new AVLTree();
    tree->root = root->right;
    return tree->find(x);
  }
  // can not find the item
  return false;
}

// numNodes returns the total number of nodes in the tree.
int AVLTree::numNodes() const {
  AVLNode* curNode = root;
  if(curNode == NULL){
    return 0;
  }
  else{
    AVLTree *leftTree = new AVLTree();
    leftTree->root = root->left;
    AVLTree *rightTree = new AVLTree();
    rightTree->root = root->right;
    return (leftTree->numNodes() + rightTree->numNodes() + 1);
  }
}

int AVLTree::balanceFactor(AVLNode* node) const {
  return (height(node->right)-height(node->left));
}

// balance makes sure that the subtree with root n maintains the AVL tree
// property, namely that the balance factor of n is either -1, 0, or 1.
void AVLTree::balance(AVLNode*& n) {
  if(n == NULL){
    ;
  }
  else{
  int bf = balanceFactor(n);
    if(bf == 2){
      if(balanceFactor(n->right) < 0){
	n->right = rotateRight(n->right);
      }
      n = rotateLeft(n);
    }
    else if(bf == -2){
      if(balanceFactor(n->left) > 0){
	n->left = rotateLeft(n->left);
      }
      n = rotateRight(n);
    }
  }
  

}

// rotateLeft performs a single rotation on node n with its right child.
AVLNode* AVLTree::rotateLeft(AVLNode*& n) {
  // if the right child has left child, make it the right child of the node n
  
  AVLNode* rightChild = n->right; 
  
  n->right = rightChild->left;

  //make the node n the left child of the right child
  rightChild->left = n;

  
  n->height = 1 + max(height(n->left), height(n->right));
  rightChild->height = 1+ max(height(rightChild->left), height(rightChild->right));

  // return the right child which is now the root
  return rightChild;
}

// rotateRight performs a single rotation on node n with its left child.
AVLNode* AVLTree::rotateRight(AVLNode*& n) {
  // if the left child has a right child, make it the left child of node n
  AVLNode* leftChild = n->left;

  n->left= leftChild->right;
  // make the node n the right child of the left child
  leftChild->right = n;

  n->height = 1 + max(height(n->left), height(n->right));

  leftChild->height = 1 + max(height(leftChild->left), height(leftChild->right));


  // return the left child which is now the root
  return leftChild;
}

// private helper for remove to allow recursion over different nodes. returns
// an AVLNode* that is assigned to the original node.
AVLNode* AVLTree::remove(AVLNode*& n, const string& x) {
  if (n == NULL) {
    return NULL;
  }
  // first look for x
  if (x == n->value) {
    // found
    // no children
    if (n->left == NULL && n->right == NULL) {
      delete n;
      n = NULL;
      return NULL;
    }
    // single child
    if (n->left == NULL) {
      AVLNode* temp = n->right;
      n->right = NULL;
      delete n;
      n = NULL;
      return temp;
    }
    if (n->right == NULL) {
      AVLNode* temp = n->left;
      n->left = NULL;
      delete n;
      n = NULL;
      return temp;
    }
    // two children -- tree may become unbalanced after deleting n
    string sr = min(n->right);
    n->value = sr;
    n->right = remove(n->right, sr);
  } else if (x < n->value) {
    n->left = remove(n->left, x);
  } else {
    n->right = remove(n->right, x);
  }
  n->height = 1 + max(height(n->left), height(n->right));
  balance(n);
  return n;
}

// min finds the string with the smallest value in a subtree.
string AVLTree::min(AVLNode* node) const {
  // go to bottom-left node
  if (node->left == NULL) {
    return node->value;
  }
  return min(node->left);
}

// height returns the value of the height field in a node. If the node is
// null, it returns -1.
int AVLTree::height(AVLNode* node) const {
  if (node == NULL) {
    return -1;
  }
  return node->height;
}

// max returns the greater of two integers.
int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

// Helper function to print branches of the binary tree
void showTrunks(Trunk* p) {
  if (p == nullptr) return;
  showTrunks(p->prev);
  cout << p->str;
}

// Recursive function to print binary tree
// It uses inorder traversal
void AVLTree::printTree(AVLNode* root, Trunk* prev, bool isLeft) {
  if (root == NULL) return;

  string prev_str = "    ";
  Trunk* trunk = new Trunk(prev, prev_str);

  printTree(root->left, trunk, true);

  if (!prev)
    trunk->str = "---";
  else if (isLeft) {
    trunk->str = ".---";
    prev_str = "   |";
  } else {
    trunk->str = "`---";
    prev->str = prev_str;
  }

  showTrunks(trunk);
  cout << root->value << endl;

  if (prev) prev->str = prev_str;
  trunk->str = "   |";

  printTree(root->right, trunk, false);
}

void AVLTree::printTree() { printTree(root, NULL, false); }
