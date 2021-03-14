/* Refernce: CLRS codes */

#include <iostream>
using namespace std;

// define the data stored in each node of the tree. each node contains the value
// contains, it's parent node, it's siblings and the color of the node. The
// color can be either RED(stored as 1) or BLACK(stored as 0).
typedef struct Node
{
	int data;
	Node *parent;
	Node *left;
	Node *right;
	int color;
} * NodePtr;

// the class defining the functions present for. the public functions are used
// to call private functions as the private functions are implemented using
// recursion so calling a single function was not possible.
class RedBlackTree
{
	NodePtr root, nullnode;											   // create a root for tree and a null node to compare null nodes
	void traversalrecursive(NodePtr node, int *countkey, int *values); //recursively traverse the tree
	NodePtr searchkeyrecursive(NodePtr node, int key);				   // recursively search the tree for a key
	void deletefix(NodePtr x);										   // fuction to fix the rb tree after deletion
	void transplant(NodePtr u, NodePtr v);							   // fucntion to swap nodes
	// bool deleteprivate(NodePtr node, int key); // function to delete node
	void insertfix(NodePtr k);									// function to fix rb tree after insert
	void countkeyrecursive(NodePtr node, int k, int *countkey); // count the number of keys
	NodePtr getparent(NodePtr node);							// return parent of current node
	NodePtr getgrandparent(NodePtr node);						// return grandparent of current node
	NodePtr getsibling(NodePtr node);							// return siblings of current node
	NodePtr getuncle(NodePtr node);								// return uncle of current node
	void insertcase1(NodePtr u, NodePtr k);
	void insertcase2(NodePtr u, NodePtr k);
	void insertcase3(NodePtr u, NodePtr k);
	void insertcase4(NodePtr k);
	void insertcase5(NodePtr k);

public:
	RedBlackTree();				   // constructor
	int *traversal(int num_nodes); //  tree traversal
	NodePtr searchkey(int k);	   // search a given key
	NodePtr minimum(NodePtr node); // find the predecessor
	void leftrotate(NodePtr x);	   // left rotate
	void rightrotate(NodePtr x);   // right rotate
	void insertkey(int key);	   // insert a key
	bool deletekey(int data);	   // delete a key
	int countkey(int k);		   // count number of occurrances of a key
};

NodePtr RedBlackTree::getparent(NodePtr node)
{
	return node == nullptr ? nullptr : node->parent;
}

NodePtr RedBlackTree::getgrandparent(NodePtr node)
{
	return getparent(getparent(node));
}

NodePtr RedBlackTree::getsibling(NodePtr node)
{
	NodePtr p = getparent(node);
	if (p == nullptr)
		return nullptr;
	if (node == p->left)
	{
		return p->right;
	}
	else
	{
		return p->left;
	}
}

NodePtr RedBlackTree::getuncle(NodePtr node)
{
	NodePtr p = getparent(node);
	return getsibling(p);
}

void RedBlackTree::traversalrecursive(NodePtr node, int *countkey, int *values)
{
	// traverse the tree
	if (node != nullnode)
	{ // check only if node is not null
		values[*countkey] = node->data;
		(*countkey)++; // count sets the position of the key
		traversalrecursive(node->left, countkey, values);

		traversalrecursive(node->right, countkey, values);
	}
}

NodePtr RedBlackTree::searchkeyrecursive(NodePtr node, int key)
{
	// searches the tree for a key
	if (node == nullnode)
	{
		// if key not found
		return NULL;
	}
	if (key == node->data)
	{
		//if key found
		return node;
	}

	if (key < node->data)
	{
		// if key to be found is less than current node
		return searchkeyrecursive(node->left, key);
	}
	// if key to be found is more than current node
	return searchkeyrecursive(node->right, key);
}

void RedBlackTree::deletefix(NodePtr x)
{
	// fix the tree after deleting a key
	NodePtr w;
	while (x != root && x->color == 0)
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == 1)
			{
				w->color = 0;		   // case 1
				x->parent->color = 1;  // case 1
				leftrotate(x->parent); // case 1
				w = x->parent->right;  // case 1
			}

			if (w->left->color == 0 && w->right->color == 0)
			{
				w->color = 1;  // case 2
				x = x->parent; // case 2
			}
			else
			{
				if (w->right->color == 0)
				{
					w->left->color = 0;	  // case 3
					w->color = 1;		  // case 3
					rightrotate(w);		  // case 3
					w = x->parent->right; // case 3
				}

				w->color = x->parent->color; // case 4
				x->parent->color = 0;		 // case 4
				w->right->color = 0;		 // case 4
				leftrotate(x->parent);		 // case 4
				x = root;					 // case 4
			}
		}
		else
		{
			// reverse the delete fix for right node
			w = x->parent->left;
			if (w->color == 1)
			{
				w->color = 0;
				x->parent->color = 1;
				rightrotate(x->parent);
				w = x->parent->left;
			}

			if (w->right->color == 0 && w->right->color == 0)
			{
				w->color = 1;
				x = x->parent;
			}
			else
			{
				if (w->left->color == 0)
				{
					w->right->color = 0;
					w->color = 1;
					leftrotate(w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = 0;
				w->left->color = 0;
				rightrotate(x->parent);
				x = root;
			}
		}
	}
	x->color = 0;
}

void RedBlackTree::transplant(NodePtr u, NodePtr v)
{ // swap the items in two given nodes
	if (u->parent == nullptr)
	{
		root = v;
	}
	else if (u == u->parent->left)
	{
		u->parent->left = v;
	}
	else
	{
		u->parent->right = v;
	}
	v->parent = u->parent;
}

void RedBlackTree::insertcase1(NodePtr u, NodePtr k)
{
	u = k->parent->parent->left;
	if (u->color == 1)
	{
		insertcase3(u, k);
	}
	else
	{
		insertcase4(k);
	}
}

void RedBlackTree::insertcase2(NodePtr u, NodePtr k)
{
	u = k->parent->parent->right;

	if (u->color == 1)
	{
		insertcase3(u, k);
	}
	else
	{
		insertcase5(k);
	}
}

void RedBlackTree::insertcase3(NodePtr u, NodePtr k)
{
	u->color = 0;
	k->parent->color = 0;
	k->parent->parent->color = 1;
	k = k->parent->parent;
}

void RedBlackTree::insertcase4(NodePtr k)
{
	if (k == k->parent->left)
	{
		k = k->parent;
		rightrotate(k);
	}
	k->parent->color = 0;
	k->parent->parent->color = 1;
	leftrotate(k->parent->parent);
}

void RedBlackTree::insertcase5(NodePtr k)
{
	if (k == k->parent->right)
	{
		k = k->parent;
		leftrotate(k);
	}
	k->parent->color = 0;
	k->parent->parent->color = 1;
	rightrotate(k->parent->parent);
}

void RedBlackTree::insertfix(NodePtr k)
{
	// fix tree after inserting a key in the tree
	NodePtr u;
	while (k->parent->color == 1)
	{
		if (k->parent == k->parent->parent->right)
		{
			insertcase1(u, k);
		}
		else
		{
			insertcase2(u, k);
		}
		if (k == root)
		{
			break;
		}
	}
	root->color = 0;
}

void RedBlackTree::countkeyrecursive(NodePtr node, int k, int *countkey)
{
	// count the numebr of recurrance of a key in the tree
	if (node != nullnode)
	{

		if (k == node->data)
		{
			(*countkey)++;
		}
		countkeyrecursive(node->left, k, countkey);
		countkeyrecursive(node->right, k, countkey);
	}
}

RedBlackTree::RedBlackTree()
{
	// constructor
	nullnode = new Node;
	nullnode->color = 0;
	nullnode->left = nullptr;
	nullnode->right = nullptr;
	root = nullnode;
}

int *RedBlackTree::traversal(int num_nodes)
{
	// traverse the tree
	int countkey = 0;
	int *values = new int[num_nodes];				   // to store values present in the tree
	traversalrecursive(this->root, &countkey, values); // recurive method to traverse the tree
	return values;
}

NodePtr RedBlackTree::searchkey(int k)
{
	return searchkeyrecursive(this->root, k);
}

NodePtr RedBlackTree::minimum(NodePtr node)
{
	while (node->left != nullnode)
	{
		node = node->left;
	}
	return node;
}

void RedBlackTree::leftrotate(NodePtr x)
{
	NodePtr y = x->right; // set y
	x->right = y->left;	  // turn y's left subtree into x's right subtree
	if (y->left != nullnode)
	{
		y->left->parent = x;
	}
	y->parent = x->parent; // link x's parent to y
	if (x->parent == nullptr)
	{
		this->root = y;
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}
	y->left = x; // put x on y's left
	x->parent = y;
}

void RedBlackTree::rightrotate(NodePtr x)
{
	NodePtr y = x->left; //set y
	x->left = y->right;	 // turn y's right subtree into x's left subtree
	if (y->right != nullnode)
	{
		y->right->parent = x;
	}
	y->parent = x->parent; // link x's parent to y
	if (x->parent == nullptr)
	{
		this->root = y;
	}
	else if (x == x->parent->right)
	{
		x->parent->right = y;
	}
	else
	{
		x->parent->left = y;
	}
	y->right = x; //put x on y's right
	x->parent = y;
}

void RedBlackTree::insertkey(int key)
{
	NodePtr node = new Node;
	node->parent = nullptr;
	node->data = key;
	node->left = nullnode;
	node->right = nullnode;
	node->color = 1;

	NodePtr y = nullptr;
	NodePtr x = this->root;

	while (x != nullnode)
	{
		y = x;
		if (node->data < x->data)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}

	node->parent = y;
	if (y == nullptr)
	{
		root = node;
	}
	else if (node->data < y->data)
	{
		y->left = node;
	}
	else
	{
		y->right = node;
	}

	if (node->parent == nullptr)
	{
		node->color = 0;
		return;
	}

	insertfix(node);
}

bool RedBlackTree::deletekey(int key)
{
	// bool success;
	NodePtr node = this->root;
	// success = deletekeyrecursive(this->root, data);
	NodePtr z = nullnode;
	NodePtr x, y;
	while (node != nullnode)
	{
		if (node->data == key)
		{
			z = node;
		}

		if (node->data <= key)
		{
			node = node->right;
		}
		else
		{
			node = node->left;
		}
	}

	if (z == nullnode)
	{

		return false;
	}

	y = z;
	int y_original_color = y->color;
	if (z->left == nullnode)
	{
		x = z->right;
		transplant(z, z->right);
	}
	else if (z->right == nullnode)
	{
		x = z->left;
		transplant(z, z->left);
	}
	else
	{
		y = minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z)
		{
			x->parent = y;
		}
		else
		{
			transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}

		transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	delete z;
	if (y_original_color == 0)
	{
		deletefix(x);
	}
	return true;
	// return success;
}

int RedBlackTree::countkey(int k)
{
	// count the number of occurrances of a key
	int countkey = 0;
	countkeyrecursive(this->root, k, &countkey); // recursively iterate and count occurrances
	return countkey;
}

int main()
{
	RedBlackTree bst;
	int num_nodes = 0;
	while (1)
	{
		bool success;
		char arg;
		int num1, num2, target, c, targets_reached = 0;
		int *values;
		cin >> arg;
		switch (arg)
		{
		case 'I': // insert key
			cin >> num1;
			bst.insertkey(num1);
			num_nodes++;
			break;
		case 'D': // delete key
			cin >> num1;
			success = bst.deletekey(num1);
			if (success)
			{
				num_nodes--;
			}
			else
			{
				continue;
			}
			break;
		case 'Q': // query range
		{
			cin >> num1 >> num2;
			targets_reached = 0;
			values = bst.traversal(num_nodes);
			for (int i = num1; i < num2; i++)
			{
				for (int j = 0; j < num_nodes; j++)
				{
					target = i - values[j];
					NodePtr node = bst.searchkey(target);
					if (node != NULL)
					{
						if (target == values[j])
						{
							c = bst.countkey(target);
							if (c > 1)
							{
								targets_reached++;
								break;
							}
						}
						else
						{
							targets_reached++;
							break;
						}
					}
				}
			}
			cout << targets_reached;
			break;
		}
		case 'E': // exit the program
			exit(0);
		default: // when incorrect input
			continue;
		}
	}
}