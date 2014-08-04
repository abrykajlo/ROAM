#include "node.h"

void Node::NextNode(int n) {
	next = new Node();
	next->data = n;
}

Node::Node() {
	next = 0;
	data = 0;
}
