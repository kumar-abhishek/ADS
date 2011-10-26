#include<iostream>
using namespace std;
#define MAXDEGREE 10 //max degree for a 500 vertex binomail heap cant be more than lg(500) = 9 <=10 
struct node
{
	int data; //used to denote vertex endPoint for dijkstra
	int distance; //as an estimate of shortest distance : for dijkstra
	int degree;
	node *child;
	node *sibling;
	node *parent; //needed for doing decrease-key operation
};

node * binomialHeapInsert(node * &root,int endPoint, int distance)
{
		node *temp = new node;
		temp->data = endPoint;
		temp->distance = distance;
		temp->child = NULL;
		temp->parent = NULL; 
		if(root!=NULL){ node *t = root->sibling; root->sibling = temp; temp->sibling = t; } 
		else { root = temp; root->sibling=root;root->degree = 0;} 
		if(root->distance > root->sibling->distance) root= root->sibling; //fix root pointer to point to node with minimum value.
		return temp; //return the pointer to newly inserted node.
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

node * removeMin(node * &root) //delete the min node and return it
{
//	cout<<"removeMin root:" << root->data <<endl;
	if(root ==NULL) return NULL;
	node * treeTable[MAXDEGREE];
	for(int i=0;i<MAXDEGREE;i++) treeTable[i] = NULL;

	node * toReturn = root;
	node *rootChild = root->child;
	//cout<<"before return"<< toReturn<<endl;

	node * toCopy = root->sibling;
	if(toCopy !=NULL)
	{
		root->data = toCopy->data; root->degree = toCopy->degree; root->child = toCopy->child; root->sibling = toCopy->sibling; //copy next node to root node
		root->distance = toCopy->distance;
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
			if(sibRoot->distance < root->distance)
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
		if(root->distance < potentialRoot->distance) potentialRoot = root;
	}
	root=potentialRoot;
	//cout<<"new root's data"<< root->data<<endl;
	return toReturn;
}

void decreaseKey(node * &root, node * &t, int newValue )  // t being pointer to the the node whose decrease key is to be performed.
{
	t->data = newValue;//TODO: change data to distance later

	//swap node with its parent if it violates  min heap property
	while( (t->parent != NULL) && (t->distance < t->parent->distance))
	{
		node *t1 = t->child;
		node *t2 = t->sibling;

		node *p = t->parent;
		node *p1 = p->child;
		node *p2 = p->sibling;

		//swap node and its parent.
		node *temp = p;
		p = t; 
		t = temp;
		
		p->child->parent = p;
		p->parent = t; 
		t->parent->child = t;

		node *q = p; //we have to come back to p
		while(q->sibling != p)
		{
			q =  q->sibling;
		}
		q->sibling = p;	


	}
	//fix the root pointer if needed
	if(t->data < root->data)    //TODO:change data to distance later 
		root = t;
}



int main()
{
	node * root=NULL;
	node * t1=binomialHeapInsert(root,0,0);
	node * t2=binomialHeapInsert(root,1,9999);
	node * t3=binomialHeapInsert(root,2,9999);
	node * t4=binomialHeapInsert(root,3,9999);
/*
	binomialHeapInsert(root,12);
	binomialHeapInsert(root,15);
	binomialHeapInsert(root,5);
	binomialHeapInsert(root,10);
	binomialHeapInsert(root,6);
	binomialHeapInsert(root,4);
	binomialHeapInsert(root,3);
*/
	printBinomialHeap(root);
	cout<<" removeMin->distance: "<<removeMin(root)->distance;//delete the min node and return it
//	decreaseKey(root,t2,-1 ); 
	cout<<endl<<endl;
	cout<<" removeMin->distance: "<<removeMin(root)->distance;//delete the min node and return it

	//cout<<"new root's data"<< root->data<<endl;
	cout<<endl<<endl;
	printBinomialHeap(root);
	return 0;
}
