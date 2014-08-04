#include <iostream>
using namespace std;

#include "btree.h"

void preorderPrint(BTNode *rootnode) {
    if (rootnode != NULL) {
       cout << rootnode->data << " ";
       preorderPrint(rootnode->left);
       preorderPrint(rootnode->right);
    }
}

void inorderPrint(BTNode *rootnode) {
	if (rootnode != NULL) {  
		inorderPrint(rootnode->left); 
		cout << rootnode->data << " "; 
		inorderPrint(rootnode->right); 
	}
}

void postorderPrint(BTNode *rootnode) {
	if (rootnode != NULL) {
		postorderPrint(rootnode->left);
		postorderPrint(rootnode->right);
		cout << rootnode->data << " ";
	}
}

int main(int argc, char const *argv[]) {
	BTree test;
	for (int i = 0; i < 10; ++i) {
		test.insert(i);
	}

	preorderPrint(test.getroot());
	cout << endl;
	inorderPrint(test.getroot());
	cout << endl;
	postorderPrint(test.getroot());
	
	return 0;
}
