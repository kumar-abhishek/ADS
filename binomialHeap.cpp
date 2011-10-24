#include<iostream>
using namespace std;
#define MAXDEGREE 500
struct node
{
	int data;
	int degree;
	node *child;
	node *sibling;
};

void binomialHeapInsert(node * &root,int value)
{
		node *temp = new node;
		temp->data = value;
		temp->child = NULL;
		if(root!=NULL){ node *t = root->sibling; root->sibling = temp; temp->sibling = t; } 
		else { root = temp; root->sibling=root;root->degree = 0;      } 
		if(root->data > root->sibling->data) root= root->sibling; //fix root pointer to point to node with minimum value.
}

void printBinomialHeap(node *root)
{
	if(root == NULL) return;
	cout<<" "<<root->data<<" ";

	printBinomialHeap(root->child);
	node *storedRoot = root;
	root=root->sibling;
	while(root != storedRoot) {cout<<" " <<(root->data)<< " "; root = root->sibling;}

}

node * meld(node * &root1,node * &root2)
{
	node *temp1 = root1->sibling;
	root1->sibling = root2->sibling;
	root1->sibling->sibling = root2;
	root2->sibling = temp1;
	temp1->sibling = root1;
	if(root1->data < root2->data) return root1;
	else return root2;
}

int removeMin(node * &root) //delete the min node and return its data
{
//	cout<<"removeMin root:" << root->data <<endl;
	if(root ==NULL) return NULL;
	node * treeTable[MAXDEGREE];
	for(int i=0;i<MAXDEGREE;i++) treeTable[i] = NULL;

	int toReturn = root->data;
	node *rootChild = root->child;
	//cout<<"before return"<< toReturn<<endl;

	node * toCopy = root->sibling;
	if(toCopy !=NULL)
	{
		root->data = toCopy->data; root->degree = toCopy->degree; root->child = toCopy->child; root->sibling = toCopy->sibling; //copy next node to root node
		delete(toCopy);//delete next node
	}
	if(rootChild !=NULL)	root = meld(rootChild,root); //meld the new root and old root's child

	//do pairwise combine
	node *storedRoot = root; //store root pointer temporarily.
	//cout<< "storedRoot->Data: "<<storedRoot->data<<endl;
	treeTable[root->degree] = root; //hash root to treeTable

	while(root->sibling == storedRoot) //if we reach again to the beginning of circular linked list
	{
		node * sibRoot = root->sibling;
		int tempDegree = sibRoot->degree;
		if(treeTable[sibRoot->degree] != NULL )  //entry exists in treeTable
		{
			if(sibRoot->data < root->data)
			{
				//node *t = sibRoot->sibling;
				sibRoot->child = root;
				//root->sibling = t;
				root = sibRoot;//set root to pointer to new root
			}
			else
			{
				//node *t  = root->sibling;
				root->child = sibRoot;
				//sibRoot->sibling = t;	
			}
			treeTable[root->degree] = root;
			treeTable[tempDegree] = NULL;
		}
		else
			treeTable[sibRoot->degree] = sibRoot;
	}


	//now fix the root pointer to point to node with min data
	storedRoot = root; //store root pointer temporarily.
	node * potentialRoot=root;
	while(root->sibling != storedRoot)
	{
		root=root->sibling;
		if(root->data < potentialRoot->data) potentialRoot = root;
	}
	root=potentialRoot;
	//cout<<"new root's data"<< root->data<<endl;
	return toReturn;
}

void decreaseKey();



int main()
{
	node * root=NULL;
/*
	binomialHeapInsert(root,3);
	binomialHeapInsert(root,2);
	binomialHeapInsert(root,1);
*/
	binomialHeapInsert(root,12);
	binomialHeapInsert(root,15);
	binomialHeapInsert(root,5);
	binomialHeapInsert(root,10);
	binomialHeapInsert(root,6);
	binomialHeapInsert(root,4);
	binomialHeapInsert(root,3);
	printBinomialHeap(root);

	 (removeMin(root));

	//cout<<"new root's data"<< root->data<<endl;
	cout<<endl<<endl;
	printBinomialHeap(root);
	return 0;
}
