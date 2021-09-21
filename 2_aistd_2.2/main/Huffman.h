#pragma once
#include "PriorityQueue.h"
#include "PriorityQueue.cpp"
#include "RedBlackTree.h"
#include "Stack.h"

class Huffman
{
private:
	class Node
	{
	public:
		Node(int freq = 0, Node* left_ = nullptr, Node* right_ = nullptr, char lett = '\n') {
			frequence_of_letter = freq;
			letter = lett;
			left = left_;
			right = right_;
			parent = nullptr;
		};

		~Node() {};

		int frequence_of_letter; //frequence of each letter
		char letter;
		Node* left;
		Node* right;
		Node* parent;
	};

	void copyArrayToBool(bool* copy_array, bool* str_array, int str_size) {
		for (size_t i = 0; i < str_size; i++)
			copy_array[i] = str_array[i];
	};

	Node* root;
	char* str_array; //array for input line
	size_t str_size; //size of the iput line array
	bool* coded; //array for coded line
	size_t size_coded; //size of the coded line array
	struct strArray
	{
		bool* str = NULL;
		int size = 0;
	};

	RBTree<char, strArray*> CodeTree;

public:
	Huffman(char* _str) {
		if (_str == NULL) //no line to code
			throw exception("null argument");

		CodeTree = RBTree<char, strArray*>(); //map array for saving letters and code of them
		str_array = _str; //original line which will be coded
		str_size = 0;  //size of the line

		for (size_t i = 0; str_array[i] != '\n'; i++) //counting line length
			str_size++;

		if (str_size > 1)
		{
			PriorityQueue<Node*> PriorityQ;
			Node* buffNode1;
			Node* buffNode2;
			int chars[256] = { 0 };

			for (int i = 0; str_array[i] != '\n'; i++)  //count quantity of each letter
				chars[str_array[i] + 128]++;

			int letter_count = 0;
			cout << "Frequency table: " << endl;

			for (int i = 0; i < 256; i++)
				if (chars[i] != 0) {
					letter_count++;
					cout << (char)(i - 128) << " : " << chars[i] << endl;
					buffNode1 = new Node(chars[i], nullptr, nullptr, (i - 128));
					PriorityQ.push(buffNode1, chars[i]);
				}

			int buff_int;
			buff_int = PriorityQ.tail_priority();
			buffNode1 = PriorityQ.pop();

			for (size_t i = 0; (i < letter_count) && (!PriorityQ.isEmpty()); i++)
			{
				buff_int += PriorityQ.tail_priority();
				buffNode2 = PriorityQ.pop();
				root = new Node(buff_int, buffNode1, buffNode2); //create a parent node for two with minimum priority
				buffNode1->parent = root;
				buffNode2->parent = root;
				PriorityQ.push(root, root->frequence_of_letter);  //return it to the queue
				buff_int = PriorityQ.tail_priority();
				buffNode1 = PriorityQ.pop();
			}

			bool* buff_str = new bool[letter_count / 2 + 1];
			memset(buff_str, 0, letter_count / 2 + 1);
			size_t index_of_str = 0;
			unsigned int buff_code_letter = 0;
			Stack<Node*> dft;

			if (root->right != nullptr)
			{
				dft.push(root->right);
			}
			buffNode1 = root;
			strArray* buffStrArray = new strArray;
			cout << "Code table: " << endl;
			while (!dft.isEmpty()) //leaf traversal
			{
				while (buffNode1->left != nullptr)  //go down to the sheet
				{
					buff_str[index_of_str] = 0;
					index_of_str++;
					buffNode1 = buffNode1->left;
					if (buffNode1->right != nullptr) //add the right child to the Stack
						dft.push(buffNode1->right);
				}

				buffStrArray = new strArray;
				buffStrArray->str = new bool[index_of_str];
				buffStrArray->size = index_of_str;
				copyArrayToBool(buffStrArray->str, buff_str, index_of_str);

				cout << buffNode1->letter << " : ";
				for (size_t i = 0; i < index_of_str; i++)
					cout << buffStrArray->str[i];
				cout << endl;

				CodeTree.insert(buffNode1->letter, buffStrArray);  //Add correspondence between letter and code
				index_of_str--;

				while ((buffNode1->parent != nullptr) && (buffNode1->parent->right != dft.get_last())) //go up
				{
					buffNode1 = buffNode1->parent;
					index_of_str--;
				}

				buffNode1 = dft.pop();

				if (buffNode1->right != nullptr)
					dft.push(buffNode1->right);

				buff_str[index_of_str] = 1;
				index_of_str++;
			}

			buffStrArray = new strArray;
			buffStrArray->size = index_of_str;
			buffStrArray->str = new bool[index_of_str];
			copyArrayToBool(buffStrArray->str, buff_str, index_of_str);

			cout << buffNode1->letter << " : ";
			for (size_t i = 0; i < index_of_str; i++)
				cout << buffStrArray->str[i];
			cout << endl;

			CodeTree.insert(buffNode1->letter, buffStrArray);
			coded = NULL;
			size_coded = 0;
			delete[] buff_str;
		}
		else {
			cout << "Frequency table: " << str_array[0] << " " << 1 << endl;
			cout << "Code table: " << str_array[0] << " " << 0 << endl;
			root = new Node(1, nullptr, nullptr, str_array[0]);

			size_coded = 0;
			coded = NULL;
			strArray* buffStrArray = new strArray;
			buffStrArray->str = new bool[1];
			buffStrArray->str[0] = false;
			buffStrArray->size = 1;
			CodeTree.insert(str_array[0], buffStrArray);
		}
	}

	bool* Code(int& size_of_coded) {
		bool* copy_str;
		size_of_coded = 0;
		for (int i = 0; str_array[i] != '\n'; i++)
			size_of_coded += CodeTree.find(str_array[i])->size;

		size_coded = size_of_coded;
		coded = new bool[size_of_coded];

		size_t index_in_str = 0;
		for (int i = 0; str_array[i] != '\n'; i++) {
			copy_str = CodeTree.find(str_array[i])->str;  //we find a match in the tree
			for (size_t j = 0; j < CodeTree.find(str_array[i])->size; j++) {
				coded[index_in_str] = copy_str[j];
				index_in_str++;
			}
		}
		return coded;
	}

	char* Decode(int& size_str_decoded) {
		char* decode_str = new char[str_size + 1];
		size_str_decoded = str_size;

		if (str_size == 1)
		{
			decode_str[0] = root->letter;
			decode_str[0] = '\n';
		}
		else {
			size_t index_code = 0;
			Node* currentNode = root;
			size_t index_decode = 0;
			while (index_code < size_coded)
			{
				if ((coded[index_code] == 0) && (currentNode->left != nullptr))  //go left
				{
					currentNode = currentNode->left;
					index_code++;
				}
				else if ((coded[index_code] == 1) && (currentNode->right != nullptr)) {  //go right
					currentNode = currentNode->right;
					index_code++;
				}
				else { //came to the desired node
					decode_str[index_decode] = currentNode->letter;
					index_decode++;
					currentNode = root;
				}
			}
			decode_str[index_decode] = currentNode->letter;
			decode_str[index_decode + 1] = '\n';
		}
		return decode_str;
	}

	~Huffman() {}
};