#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

enum Color { RED, BLACK, DOUBLE_BLACK };

template<typename T>
struct Element
{
	T data;
	int colour;
	Element<T> *left;
	Element<T> *right;
	Element<T> *parent;

	explicit Element(T data);
};

template<typename T>
Element<T>::Element(T data)
{
	this->data = data;
	colour = RED;
	left = right = parent = nullptr;
}

template<typename T>
class RBTree
{
public:
	RBTree();

	void insertValue(T info);
	void deleteValue(T info);

	void merge(RBTree rbTree2);

	void postorder();
	void inorder();
	void preorder();

private:
	Element<T> *root;

	void rotateLeft(Element<T> *leaf);
	void rotateRight(Element<T> *leaf);

	void fixInsert(Element<T> *leaf);
	void fixDelete(Element<T> *element);

	Element<T> *postorderBST(Element<T>* leaf);
	Element<T> *inorderBST(Element<T> *leaf);
	Element<T> *preorderBST(Element<T> *leaf);

	int getColor(Element<T> *element);
	Element<T> *setColor(Element<T> *element, int colour);

	Element<T> *minValueElement(Element<T> *element);
	Element<T> *maxValueElement(Element<T> *element);

	Element<T> *insertBST(Element<T> *root, Element<T> *leaf);
	Element<T> *deleteBST(Element<T> *root, T data);

	int getBlackHeight(Element<T> *leaf);
};

template<typename T>
RBTree<T>::RBTree() : root(nullptr) {}

template<typename T>
void RBTree<T>::insertValue(T info)
{
	Element<T> *element = new Element<T>(info);
	root = insertBST(root, element);
	fixInsert(element);
}

template<typename T>
void RBTree<T>::deleteValue(T info)
{
	Element<T> *element = deleteBST(root, info);
	fixDelete(element);
}

template<typename T>
void RBTree<T>::merge(RBTree rbTree2)
{
	int temp;
	Element<T> *c, *temp_ptr;
	Element<T> *root1 = root;
	Element<T> *root2 = rbTree2.root;
	int blackHeight1 = getBlackHeight(root1);
	int blackHeight2 = getBlackHeight(root2);
	if (blackHeight1 > blackHeight2)
	{
		c = maxValueNode(root1);
		temp = c->data;
		deleteValue(c->data);
		root1 = root;
	}
	else if (blackHeight2 > blackHeight1)
	{
		c = minValueNode(root2);
		temp = c->data;
		rbTree2.deleteValue(c->data);
		root2 = rbTree2.root;
	}
	else
	{
		c = minValueNode(root2);
		temp = c->data;
		rbTree2.deleteValue(c->data);
		root2 = rbTree2.root;
		if (blackHeight1 != getBlackHeight(root2))
		{
			rbTree2.insertValue(c->data);
			root2 = rbTree2.root;
			c = maxValueNode(root1);
			temp = c->data;
			deleteValue(c->data);
			root1 = root;
		}
	}
	setColor(c, RED);
	int finalblackheight1 = getBlackHeight(root1);
	int finalblackheight2 = getBlackHeight(root2);
	if (finalblackheight1 == finalblackheight2)
	{
		c->left = root1;
		root1->parent = c;
		c->right = root2;
		root2->parent = c;
		setColor(c, BLACK);
		c->data = temp;
		root = c;
	}
	else if (finalblackheight2 > finalblackheight1)
	{
		Element<T> *ptr = root2;

		while (finalblackheight1 != getBlackHeight(ptr))
		{
			temp_ptr = ptr;
			ptr = ptr->left;
		}
		Element<T> *ptr_parent;
		if (ptr == nullptr)
		{
			ptr_parent = temp_ptr;
		}
		else
		{
			ptr_parent = ptr->parent;
		}
		c->left = root1;
		if (nullptr != root1)
		{
			root1->parent = c;
		}
		c->right = ptr;
		if (nullptr != ptr)
		{
			ptr->parent = c;
		}
		ptr_parent->left = c;
		c->parent = ptr_parent;
		if (getColor(ptr_parent) == RED)
		{
			fixInsertRBTree(c);
		}
		else if (getColor(ptr) == RED)
		{
			fixInsertRBTree(ptr);
		}
		c->data = temp;
		root = root2;
	}
	else
	{
		Element<T> *ptr = root1;
		while (finalblackheight2 != getBlackHeight(ptr))
		{
			ptr = ptr->right;
		}
		Element<T> *ptr_parent = ptr->parent;
		c->right = root2;
		root2->parent = c;
		c->left = ptr;
		ptr->parent = c;
		ptr_parent->right = c;
		c->parent = ptr_parent;
		if (getColor(ptr_parent) == RED)
		{
			fixInsertRBTree(c);
		}
		else if (getColor(ptr) == RED)
		{
			fixInsertRBTree(ptr);
		}
		c->data = temp;
		root = root1;
	}
	return;
}

template<typename T>
void RBTree<T>::postorder()
{
	postorderBST(root);
	cout << "-------" << endl;
}

template<typename T>
void RBTree<T>::inorder()
{
	inorderBST(root);
	cout << "-------" << endl;
}

template<typename T>
void RBTree<T>::preorder()
{
	preorderBST(root);
	cout << "-------" << endl;
}

template<typename T>
void RBTree<T>::rotateLeft(Element<T>* leaf)
{
	Element<T> *rightChild = leaf->right;
	leaf->right = rightChild->left;

	if (nullptr != leaf->right)
	{
		leaf->right->parent = leaf;
	}

	rightChild->parent = leaf->parent;

	if (nullptr == leaf->parent)
	{
		root = rightChild;
	}
	else if (leaf == leaf->parent->left)
	{
		leaf->parent->left = rightChild;
	}
	else
	{
		leaf->parent->right = rightChild;
	}

	rightChild->left = leaf;
	leaf->parent = rightChild;
}

template<typename T>
void RBTree<T>::rotateRight(Element<T>* leaf)
{
	Element<T> *leftChild = leaf->left;
	leaf->left = leftChild->right;

	if (nullptr != leaf->left)
	{
		leaf->left->parent = leaf;
	}

	leftChild->parent = leaf->parent;

	if (nullptr == leaf->parent)
	{
		root = leftChild;
	}
	else if (leaf == leaf->parent->left)
	{
		leaf->parent->left = leftChild;
	}
	else
	{
		leaf->parent->right = leftChild;
	}

	leftChild->right = leaf;
	leaf->parent = leftChild;
}

template<typename T>
void RBTree<T>::fixInsert(Element<T>* leaf)
{
	Element<T> *parent = nullptr;
	Element<T> *grandparent = nullptr;
	while (leaf != root 
		&& getColor(leaf) == RED 
		&& getColor(leaf->parent) == RED)
	{
		parent = leaf->parent;
		grandparent = parent->parent;

		if (parent == grandparent->left)
		{
			Element<T> *uncle = grandparent->right;
			if (getColor(uncle) == RED)
			{
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				leaf = grandparent;
			}
			else {
				if (leaf == parent->right)
				{
					rotateLeft(parent);
					leaf = parent;
					parent = leaf->parent;
				}
				rotateRight(grandparent);
				swap(parent->colour, grandparent->colour);
				leaf = parent;
			}
		}
		else {
			Element<T> *uncle = grandparent->left;
			if (getColor(uncle) == RED)
			{
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				leaf = grandparent;
			}
			else
			{
				if (leaf == parent->left)
				{
					rotateRight(parent);
					leaf = parent;
					parent = leaf->parent;
				}
				rotateLeft(grandparent);
				swap(parent->colour, grandparent->colour);
				leaf = parent;
			}
		}
	}
	setColor(root, BLACK);
}
template<typename T>
void RBTree<T>::fixDelete(Element<T>* element)
{
	if (nullptr == element)
	{
		return nullptr;
	}

	if (element == root)
	{
		root = nullptr;
		return root;
	}

	if (getColor(element) == RED 
		|| getColor(element->left) == RED
		 || getColor(element->right) == RED)
	{
		Element<T> *child = element->left != nullptr ? element->left : element->right;

		if (element == element->parent->left)
		{
			element->parent->left = child;
			if (nullptr != child)
			{
				child->parent = element->parent;
			}
			setColor(child, BLACK);
			delete (element);
		}
		else
		{
			element->parent->right = child;
			if (nullptr != child)
				child->parent = element->parent;
			setColor(child, BLACK);
			delete (element);
		}
	}
	else
	{
		Element<T> *sibling = nullptr;
		Element<T> *parent = nullptr;
		Element<T> *ptr = element;
		setColor(ptr, DOUBLE_BLACK);
		while (ptr != root && getColor(ptr) == DOUBLE_BLACK)
		{
			parent = ptr->parent;
			if (ptr == parent->left) {
				sibling = parent->right;
				if (getColor(sibling) == RED)
				{
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateLeft(parent);
				}
				else
				{
					if (getColor(sibling->left) == BLACK 
						&& getColor(sibling->right) == BLACK)
					{
						setColor(sibling, RED);
						if (getColor(parent) == RED)
							setColor(parent, BLACK);
						else
							setColor(parent, DOUBLE_BLACK);
						ptr = parent;
					}
					else
					{
						if (getColor(sibling->right) == BLACK)
						{
							setColor(sibling->left, BLACK);
							setColor(sibling, RED);
							rotateRight(sibling);
							sibling = parent->right;
						}
						setColor(sibling, parent->color);
						setColor(parent, BLACK);
						setColor(sibling->right, BLACK);
						rotateLeft(parent);
						break;
					}
				}
			}
			else
			{
				sibling = parent->left;
				if (getColor(sibling) == RED)
				{
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateRight(parent);
				}
				else
				{
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK)
					{
						setColor(sibling, RED);
						if (getColor(parent) == RED)
						{
							setColor(parent, BLACK);
						}
						else
						{
							setColor(parent, DOUBLE_BLACK);
						}
						ptr = parent;
					}
					else
					{
						if (getColor(sibling->left) == BLACK)
						{
							setColor(sibling->right, BLACK);
							setColor(sibling, RED);
							rotateLeft(sibling);
							sibling = parent->left;
						}
						setColor(sibling, parent->color);
						setColor(parent, BLACK);
						setColor(sibling->left, BLACK);
						rotateRight(parent);
						break;
					}
				}
			}
		}
		if (element == element->parent->left)
		{
			element->parent->left = nullptr;
		}
		else
		{
			element->parent->right = nullptr;
		}
		delete(element);
		setColor(root, BLACK);
	}
}

template<typename T>
Element<T> *RBTree<T>::postorderBST(Element<T>* leaf)
{
	if (nullptr == leaf)
	{
		return nullptr;
	}
	postorderBST(leaf->left);
	postorderBST(leaf->right);
	cout << leaf->data << " " << leaf->colour << endl;
}

template<typename T>
Element<T> *RBTree<T>::inorderBST(Element<T>* leaf)
{
	if (nullptr == leaf)
	{
		return nullptr;
	}
	inorderBST(leaf->left);
	cout << leaf->data << " " << leaf->colour << endl;
	inorderBST(leaf->right);
}

template<typename T>
Element<T> *RBTree<T>::preorderBST(Element<T>* leaf)
{
	if (nullptr == leaf)
	{
		return nullptr;
	}

	cout << leaf->data << " " << leaf->colour << endl;
	preorderBST(leaf->left);
	preorderBST(leaf->right);
}

template<typename T>
int RBTree<T>::getColor(Element<T>* element)
{
	if (element == nullptr)
	{
		return BLACK;
	}
	return element->colour;
}

template<typename T>
Element<T> *RBTree<T>::setColor(Element<T>* element, int colour)
{
	if (nullptr == element)
	{
		return nullptr;
	}
	element->colour = colour;
}

template<typename T>
Element<T>* RBTree<T>::minValueElement(Element<T>* element)
{
	Element<T> *ptr = element;

	while (nullptr != ptr->left)
	{
		ptr = ptr->left;
	}

	return ptr;
}

template<typename T>
Element<T>* RBTree<T>::maxValueElement(Element<T>* element)
{
	Element<T> *ptr = element;

	while (nullptr != ptr->right)
	{
		ptr = ptr->right;
	}

	return ptr;
}

template<typename T>
Element<T>* RBTree<T>::insertBST(Element<T>* root, Element<T>* leaf)
{
	if (root == nullptr)
	{
		return leaf;
	}

	if (leaf->data < root->data)
	{
		root->left = insertBST(root->left, leaf);
		root->left->parent = root;
	}
	else if (leaf->data > root->data)
	{
		root->right = insertBST(root->right, leaf);
		root->right->parent = root;
	}

	return root;
}

template<typename T>
Element<T>* RBTree<T>::deleteBST(Element<T>* root, T data)
{
	if (root == nullptr)
	{
		return root;
	}
	if (data < root->data)
	{
		return deleteBST(root->left, data);
	}
	if (data > root->data)
	{
		return deleteBST(root->right, data);
	}
	if (nullptr == root->left || nullptr == root->right)
	{
		return root;
	}
	Element<T> *temp = minValueNode(root->right);
	root->data = temp->data;
	return deleteBST(root->right, temp->data);
}

template<typename T>
int RBTree<T>::getBlackHeight(Element<T>* leaf)
{
	int blackheight = 0;
	while (nullptr != leaf)
	{
		if (getColor(leaf) == BLACK)
		{
			blackheight++;
		}
		leaf = leaf->left;
	}
	return blackheight;
}
#endif // !RED_BLACK_TREE_H

