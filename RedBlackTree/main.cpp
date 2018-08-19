#include <iostream>
#include <conio.h>

#include "red_black_tree.h"

using namespace std;


int main()
{
	//int data;
	RBTree<char> rbTree1;

	rbTree1.insertValue('S');
	rbTree1.insertValue('E');
	rbTree1.insertValue('A');
	rbTree1.insertValue('R');
	rbTree1.insertValue('C');
	rbTree1.insertValue('H');
	rbTree1.insertValue('X');
	rbTree1.insertValue('M');
	rbTree1.insertValue('P');
	rbTree1.insertValue('L');

	cout << "Preorder:\n";
	rbTree1.preorder();
	cout << "Inorder:\n";
	rbTree1.inorder();
	cout << "Postorder:\n";
	rbTree1.postorder();

	_getch();
	return 0;
}