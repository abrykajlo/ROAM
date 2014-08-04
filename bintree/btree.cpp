#include <iostream>
using namespace std;

#include "btree.h"

BTree::BTree() {
	root=NULL;
}

BTree::~BTree() {
	purge();
}

void BTree::insert(int new_data) {
	if(root != NULL)
		insert(new_data, root);
	else
	{
		root = new BTNode;
		root->data = new_data;
		root->left = NULL;
		root->right = NULL;
	}
}

BTNode *BTree::find(int target) {
	return find(target, root);
}

void BTree::purge() {
	purge(root);
}

BTNode *BTree::getroot() {
	return root;
}

//----------

void BTree::insert(int new_data, BTNode *leaf) {
	if(new_data < leaf->data) {
		if(leaf->left != NULL) insert(new_data, leaf->left);
		else {
			leaf->left = new BTNode;
			leaf->left->data = new_data;
			leaf->left->left = NULL;
			leaf->left->right = NULL;
		}	
	}
	else if(new_data>=leaf->data) {
		if(leaf->right!=NULL) insert(new_data, leaf->right);
		else {
			leaf->right = new BTNode;
			leaf->right->data = new_data;
			leaf->right->left = NULL;	
			leaf->right->right = NULL; 
		}
	}
}

BTNode *BTree::find(int target, BTNode *leaf) {
	if(leaf != NULL) {
		if(target == leaf->data) return leaf;
		if(target < leaf->data) return find(target, leaf->left);
		else return find(target, leaf->right);
	}
	else return NULL;
}

void BTree::purge(BTNode *leaf) {
	if(leaf != NULL) {
		purge(leaf->left);
		purge(leaf->right);
		delete leaf;
	}
}