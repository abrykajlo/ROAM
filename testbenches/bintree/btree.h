#ifndef BTREE_H
#define BTREE_H

struct BTNode { 
    int data;
    BTNode *left;
    BTNode *right;
};

class BTree {
  public:
    BTree();
    ~BTree();

    void insert(int new_data);
    BTNode *find(int target);
    void purge();

    BTNode *getroot();
    void preorderPrint(BTNode *rootnode);

  private:
    void insert(int new_data, BTNode *leaf);
    BTNode *find(int target, BTNode *leaf);
    void purge(BTNode *leaf);
    
    BTNode *root;
};

#endif