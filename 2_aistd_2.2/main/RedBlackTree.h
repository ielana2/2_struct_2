#pragma once
#include "Stack.h"
#include "Stack.cpp"
#include <iostream>
using namespace std;

template <class K, class V>
class RBTree
{
private:
	class Node //class for each element in red-black tree
	{
	public:
		Node(K key, V info, bool color = 0, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr) {
			this->key = key;
			this->value = info;
			this->color = color;
			this->left = left;
			this->right = right;
			this->parent = parent;
		}

		~Node() {};

		K key;
		V value;
		Node* left;
		Node* right;
		Node* parent;
		bool color; // black = 0, red = 1
	};

	class dft_Iterator // depth-first traverse
	{
	private:
		Stack<Node*> stack;
		Node* current;
		Node* zero; //element with no value. always black

	public:
		dft_Iterator(Node* root = nullptr, Node* zero = nullptr) {
			this->zero = zero;
			current = root;
			stack.push(current);
		}

		Node* next() { //to go to next element
			if (!has_next()) {
				throw "No more elements";
			}
			Node* tmp = current;
			if (current->right != zero) //add right leaf of the tree to the stack
			{
				stack.push(current->right);
			}
			if (current->left != zero) //go left
			{
				current = current->left;
			}
			else { //no left to go
				current = stack.pop();
			}
			return tmp;
		}

		bool has_next() { //check on last element
			if (!stack.isEmpty())
				return true;
			else
				return false;
		}

		~dft_Iterator() {};
	};

	Node* root;
	Node* zero;
	int tree_size;

	void coloring(Node* node) { //to color elements of the three
		if (node == root)
		{
			node->color = 0; //root is always black
			return;
		}

		Node* uncle;
		while ((node->parent->color) && (node != root)) //parent is red
		{
			if (node->parent == node->parent->parent->left) //parent is left child
			{
				uncle = node->parent->parent->right;
				if (uncle->color) //uncle is red too
				{
					node->parent->color = false; //repaint parent
					uncle->color = false; //repaint uncle
					node->parent->parent->color = true; //repaint grand parent
					node = node->parent->parent; //check grand parent 
				}
				else { //uncle is black
					if (node == node->parent->right) { //node is right child
						node = node->parent;
						RotateLeft(node);
					}
					node->parent->color = false; //repaint node's parent
					node->parent->parent->color = true;  //repaint node's grand parent
					RotateRight(node->parent->parent);
				}
			}
			else {  //parent is right child
				uncle = node->parent->parent->left;
				if (uncle->color) //uncle is red
				{
					node->parent->color = false;
					uncle->color = false;
					node->parent->parent->color = true;
					node = node->parent->parent;
				}
				else { //uncle is black
					if (node == node->parent->left) {
						node = node->parent;
						RotateRight(node);
					}
					node->parent->color = false;
					node->parent->parent->color = true;
					RotateLeft(node->parent->parent);
				}
			}
		}
		root->color = false; //paint root into black, cause root is always black
	};

	void RotateLeft(Node* node) { //to fix right part of the three
		Node* child = node->right;
		node->right = child->left;
		if (child->left != zero)  //need to change parents
			child->left->parent = node;

		if (child != zero)
			child->parent = node->parent;

		if (node->parent != zero)
		{
			if (node == node->parent->left) //need to change place
				node->parent->left = child;
			else
				node->parent->right = child;
		}
		else
			root = child;

		child->left = node;

		if (node != zero)
			node->parent = child;
	};

	void RotateRight(Node* node) { //to fix left part of the three
		Node* child = node->left;
		node->left = child->right;
		if (child->right != zero)
			child->right->parent = node;

		if (child != zero)
			child->parent = node->parent;

		if (node->parent != zero)
		{
			if (node == node->parent->right)  // change place
				node->parent->right = child;
			else
				node->parent->left = child;
		}
		else
			root = child;

		child->right = node;

		if (node != zero)
			node->parent = child;
	};

	void FixAfterRemoval(Node* node) { //to fix three after removal of an element
		Node* brother;
		while ((node != root) && !(node->color)) //node is not a rootand is black
		{
			if (node == node->parent->left) //node is left child
			{
				brother = node->parent->right;
				if (brother->color) //brother is red
				{
					brother->color = false;//swap parent and brother color 
					node->parent->color = true;
					RotateLeft(node->parent);
					brother = node->parent->right;
				}

				if ((brother->left->color == false) && (brother->right->color == false)) //brother's children are both black
				{
					brother->color = true; //repaint brother
					node = node->parent;
				}
				else {
					if (brother->right->color == false) //brother's right child is black
					{
						brother->left->color = false; //swap colors between brother and his left child
						brother->color = true;
						RotateRight(brother);
						brother = node->parent->right;
					}
					brother->color = node->parent->color;
					node->parent->color = false;
					brother->right->color = false;
					RotateLeft(node->parent);
					node = root;
				}
			}
			else {  //node is right child. Evrything same as previous but in other direction
				brother = node->parent->left;
				if (brother->color)
				{
					brother->color = false;
					node->parent->color = true;
					RotateRight(node->parent);
					brother = node->parent->left;
				}

				if ((brother->right->color == false) && (brother->left->color == false))
				{
					brother->color = true;
					node = node->parent;
				}
				else {
					if (brother->left->color == false)
					{
						brother->right->color = false;
						brother->color = true;
						RotateLeft(brother);
						brother = node->parent->left;
					}
					brother->color = node->parent->color;
					node->parent->color = false;
					brother->left->color = false;
					RotateRight(node->parent);
					node = root;
				}
			}
		}
		node->color = false; //root color
	};

public:
	RBTree() {
		zero = new Node(0, NULL, 0);
		this->root = zero;
		this->tree_size = 0;
	}
	~RBTree() {
		clear();
		delete root;
	}

	void insert(K insKey, V insValue) //insert element with value by key
	{
		if (tree_size == 0) //adding root
			root = new Node(insKey, insValue, false, zero, zero, zero);
		else {
			Node* newNode = new Node(insKey, insValue, true, zero, zero, zero); //new element is red
			Node* parent = root;
			Node* leaf = parent;
			while (parent != zero) //looking for a right place by key
			{
				leaf = parent;
				if (parent->key < newNode->key)
					parent = parent->right;
				else
					parent = parent->left;
			}

			newNode->parent = leaf;
			if (leaf->key < newNode->key)
				leaf->right = newNode;
			else
				leaf->left = newNode;

			coloring(newNode);
		}

		tree_size++;
	};

	void remove(K removal_key) //remove element by key
	{
		tree_size--;
		Node* node = root;
		Node* nodeOne;
		Node* nodeTwo;
		while (node->key != removal_key)//looking for element to remove
		{
			if ((node == zero) || (node == nullptr))
				throw out_of_range("Element not found");

			if (node->key < removal_key)
				node = node->right;
			else
				node = node->left;
		}

		if ((node->left == zero) && (node->right == zero))
		{
			if (node == root)
				root = zero;

			else
				if (node == node->parent->left)
					node->parent->left = zero;
				else
					node->parent->right = zero;

			return;
		}

		if ((node->left == zero) || (node->right == zero))
			nodeOne = node;

		else { //element which will be removed has two children
			nodeOne = node->right;
			while (nodeOne->left != zero) //looking for node to swap
				nodeOne = nodeOne->left;
		}

		if (nodeOne->left != zero) //fixing node
			nodeTwo = nodeOne->left;
		else
			nodeTwo = nodeOne->right;

		nodeTwo->parent = nodeOne->parent;

		if (nodeOne->parent != zero)
		{
			if (nodeOne == nodeOne->parent->left)
				nodeOne->parent->left = nodeTwo;
			else
				nodeOne->parent->right = nodeTwo;
		}
		else
			root = nodeTwo;

		if (nodeOne != node)
		{
			node->key = nodeOne->key;
			node->value = nodeOne->value;
		}
		if (!(nodeOne->color)) //parent shouldn't be red
			FixAfterRemoval(nodeTwo);

		delete nodeOne;
	};

	V find(K search_key) //search element by key
	{
		Node* node = root;
		while (node->key != search_key) //looking for element with right key
		{
			if ((node == zero) || (node == nullptr))
				throw out_of_range("Element not found");

			if (node->key < search_key)
				node = node->right;
			else
				node = node->left;
		}

		return node->value;
	};

	void clear() //delete three
	{
		while ((root != zero) && (root != nullptr))
			remove(root->key);
		root = zero;
		tree_size = 0;
	};

	size_t getSize() { //get three size
		return tree_size;
	};

	K* get_keys() //get list of three elements' keys
	{
		dft_Iterator itt(root, zero);
		K* key_array = new K[tree_size];
		int index = 0;
		Node* current;

		while (itt.has_next())
		{
			current = itt.next();
			key_array[index] = current->key;
			index++;
		}

		return key_array;
	};

	V* get_values() //get list of three elements' values
	{
		dft_Iterator itt(root, zero);
		V* value_array = new V[tree_size];
		int index = 0;
		Node* current;

		while (itt.has_next())
		{
			current = itt.next();
			value_array[index] = current->value;
			index++;
		}

		return value_array;
	};

	void print()
	{
		dft_Iterator itt(root, zero);
		Node* current;
		while (itt.has_next())
		{
			current = itt.next();
			cout << "Key: " << current->key  << endl;
		}
		cout << endl;
	};
};