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
extern node * heapArray[500];
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
	cout<<" "<<root->data<<" "<<root->distance<<endl;
	printBinomialHeap(root->child);
	node *storedRoot = root;
	root=root->sibling;
	while(root != storedRoot) {cout<<" " <<(root->data)<< " "<<root->distance<<endl; root = root->sibling;}

}

node * meld(node * &root1,node * &root2)
{
	cout<<endl<<"Inside meld()" <<endl;
	if(root2 == NULL) return root1;
	if(root1 == NULL) return root2;
	node *temp1 = root1->sibling;
	root1->sibling = root2->sibling;
	root1->sibling->sibling = root2;
	root2->sibling = temp1;
	temp1->sibling = root1;
	if(root1->data < root2->data) return root1;
	else return root2;
}

node * pairwiseCombine(node * &root1, node * &root2)
{
	//do pairwise combine
	//node * sibRoot = root->sibling;
	node * root = root2;
	node * sibRoot = root1;

	if(sibRoot->distance < root->distance)
	{
		node *t = sibRoot->child;
		if(t == NULL) t = root;
		node *sRoot = root->sibling;//save Root's sibling
		sibRoot->child = root;
		root->parent = sibRoot;
		root->sibling = t;

		//fix circular link list pointer
		node *temp = t;
		if(temp)
		{
			while(temp->sibling != t)	temp = temp->sibling;//reach the last node in cicular linked list
			temp->sibling = root;
		}
		//fix the case when sibRoot's sibling points to root
		sibRoot->sibling = sRoot;

		root = sibRoot;//set root to pointer to new root
		++(root->degree);
	}
	else
	{
		node *t  = root->child;
		if(t == NULL) t = sibRoot;
//		cout<< "t->data: "  << t->data<<endl;
		node *sSibRoot = sibRoot->sibling; //save sibRoot's sibling 
		root->child = sibRoot;
		sibRoot->parent = root;
		sibRoot->sibling = t;	

		//fix circular link list pointer
		node *temp = t;
		if(temp)
		{
			while(temp->sibling != t)	temp = temp->sibling;//reach the last node in cicular linked list
			temp->sibling = sibRoot;
		}

		//fix the case when sibRoot's sibling points to root
		root->sibling = sSibRoot;
		++(root->degree);
	}
	cout<<"bheap inside pairwise Combine"<<endl;
	//printBinomialHeap(root);
	cout<<"root->degree:  " << root->degree<<endl;
	return root;
}

void debug(node * root)
{
	cout<<endl<<endl<<"inside debug function";
	if(root) cout<<"new root's data: "<< root->data<<"|root->distance: " << root->distance<<endl;
	if( root && root->child) cout<<"new root's child data : "<<root->child->data<<"|root->child->distance: " << root->child->distance<<endl;
	if(root && root->sibling) cout<<"new roots' sibling data : "<<root->sibling->data <<"|root->sibling->distance: " << root->sibling->distance<<endl;
	if( root && root->sibling && root->sibling->sibling) cout<< "new root's sibling->sibling->data: " <<root->sibling->sibling->data<<"|root->sibling->sibling->distance: " << root->sibling->sibling->distance<<endl;
	if(root && root->child && root->child->sibling) cout<< "new root's child->sibling->data: " <<root->child->sibling->data<<"|root->child->sibling->distance: " << root->child->sibling->distance<<endl;
	if(root && root->sibling && root->sibling->child) cout<< "new root's sibling->child->data: " <<root->sibling->child->data<<"|root->sibling->child->distance: " << root->sibling->child->distance<<endl;
	if(root && root->child && root->child->child) cout<<"new root->child->child data : "<<root->child->child->data<<"|root->child->child->distance: " << root->child->child->distance<<endl;
	if(root && root->child && root->child->sibling && root->child->sibling->sibling) cout<< "new root's child->sibling->sibling->data: " <<root->child->sibling->sibling->data<<"|root->child->sibling->sibling->distance: " << root->child->sibling->sibling->distance<<endl;

}

node * removeMin(node * &root,int k) //delete the min node and return it
{
	if(root ==NULL) return NULL;
	cout<<"removeMin root:" << root->data <<"| removeMin distance:  " <<root->distance <<endl <<" root->sibling->data: " << root->sibling->data<<endl ;
	node * treeTable[MAXDEGREE];
	for(int i=0;i<MAXDEGREE;i++) treeTable[i] = NULL;

	node * toReturn = new node; toReturn->data =root->data; toReturn->distance = root->distance;
	node *rootChild = root->child;
//	cout<<endl<<"rootChild->data: "<< root->child->data<<endl;
	//cout<<"before return"<< toReturn<<endl;

	cout<< " xx: ";
//	node * toCopy = root->sibling;

	cout<<"before copying"<<endl;
	debug(root);
	//printBinomialHeap(root);
/*
	if(toCopy !=NULL && toCopy != root)
	{
		root->data = toCopy->data; root->degree = toCopy->degree; root->child = toCopy->child; root->sibling = toCopy->sibling; //copy next node to root node
		root->distance = toCopy->distance;
		delete(toCopy);//delete next node
	}
	for(int i = 0;i<4;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
	cout<<endl;

	if(toCopy == root) 
	{
		delete(root);
		root = rootChild;		
	}
*/
	if(root->sibling == root)
	{
		delete(root);
		root = rootChild;
	}
	else
	{
		node  * sibRoot = root->sibling , *tt = root;
		while(tt->sibling != root) {tt = tt->sibling;}
		tt->sibling = sibRoot;
		//	delete(root);
		root = sibRoot;
	}

	cout<<"before melding"<<endl;
	debug(root);
	for(int i = 0;i<6;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
	cout<<endl;

	if(rootChild !=NULL && root != rootChild)	root->child = meld(rootChild,root->child); //meld the new root and old root's child

	cout<<"after melding"<<endl;
	debug(root);


	//do pairwise combine
	node *storedRoot = root; //store root pointer temporarily.
	cout<<"before doing pairwise combine " <<endl;
	debug(root);
	int cnt = 0;
	while(1)
	{
		if(root == NULL) break;
		if( treeTable[root->degree] != NULL)
		{
			if(treeTable[root->degree] == root) 
				break;
			else
			{
				int tempDegree = root->degree;
			//	cout<<"pairwise combining: "<<root->data<<" " <<treeTable[root->degree]->data<<endl;
				root = pairwiseCombine(root, treeTable[root->degree]);
				debug(root);
				treeTable[tempDegree] = NULL;
			}
		}
		else
		{
			treeTable[root->degree] = root;
			root = root->sibling;
		}
//		cout<<endl;
		//printBinomialHeap(root);
//		cout<<endl;
//		++cnt;
//		if(cnt > 5) break;
	}

	//cout<<"Bheap after pairwise combine " <<endl;
	//printBinomialHeap(root);
	cout<<"after doing pariwise combine " <<endl;
	debug(root);
	for(int i = 0;i<4;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
	cout<<endl;


	//now fix the root pointer to point to node with min data
	storedRoot = root; //store root pointer temporarily.
	node * potentialRoot=root;
	int counter = 0 ;
	while(root && root->sibling != storedRoot)
	{
		cout<<"inside while loop" ;
		root=root->sibling;
		if(root->distance < potentialRoot->distance) potentialRoot = root;
		++counter; 
		if(counter>5){cout<<endl<<endl<<"!!!!!!        infinite loop       !!!!!!!!!"; break;}
	}
	root=potentialRoot;
	if(root)  root->parent = NULL; //TODO: hack remove it later
	//cout<<"new root's data"<< root->data<<endl<< "new root's child "<<root->child->data<<endl<<" new roots' sibling"<<root->sibling->data <<endl;
	for(int i = 0;i<4;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
	cout<<endl;

	return toReturn;
}

void decreaseKey(node * &root, node * &t, int newValue )  // t being pointer to the the node whose decrease key is to be performed.
{
	//if(t->data == 0 ) return;
	if(t == NULL) return; 
	if(root ) cout<<endl<<"root->data "<<root->data; if(t) cout<< "|t->data " << t->data<<endl;
	t->distance = newValue;
	if(t->parent !=NULL) cout<<"deckey n"<<endl;
	int count = 0 ;
	//swap node with its parent if it violates  min heap property
	while( (t->parent != NULL) && (t->distance < t->parent->distance))
	{
		cout<<"count: "<<count<<endl;
		++count;
		node *t1 = t->child;
		node *t2 = NULL;
		if(t->sibling != t) t2 = t->sibling;
		node *t3 = NULL;

		node *p = t->parent;
		node *p1 = p->child;
		node *p2 = p->sibling;
		node *px = p->sibling;
		node *tx = t2;
		node *t4 = p1->child;
		int cntx = 0; 
		while(tx && tx->sibling != p1){ tx = tx->sibling; ++cntx; if(cntx >10){cout<<"fell into infinite loop" ;  break;} } 
		while(px->sibling !=p) px = px->sibling;
		if(p) t3 = p->parent;

		t->child = p;
	 	p->parent =t;
		p->child = t1;
		if(t4 && t4 != t->child && p1 != t) { p->child = t4; t4->parent = p;} //lastest addition
		if(t2) p->sibling = t2;
		else p->sibling = p;
		if(p1 != t) p1->sibling = p; 
		if(p2 != p) t->sibling = p2;//only if p2!=p
		else t->sibling = t;
		if(t) t->parent = t3;
		if(t3) t3->child = t;
		//p2->sibling = t;
		if(px != p) px->sibling = t;
		if(t1) t1->parent = p;
		if(tx && tx != t) tx->sibling = p;
/*	
		node *q = p; //we have to come back to p
		while(q->sibling != p)
		{
			q =  q->sibling;
		}
		q->sibling = p;	
*/
	}
	//fix the root pointer if needed
	if(root && t->parent == NULL && t->distance < root->distance)    
		root = t;
	cout<<"inside decrease key function: "<<endl;
	debug(root);
	//printBinomialHeap(root);
}

