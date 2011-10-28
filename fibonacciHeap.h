#include<iostream>
using namespace std;

#define MAXDEGREE 10 //max degree for a 500 vertex binomail heap cant be more than lg(500) = 9 <=10 
struct node
{
	int data; //used to denote vertex endPoint for dijkstra
	int distance; //as an estimate of shortest distance : for dijkstra
	int degree;
	bool childCut;
	node *child;
	node *next;
	node *prev;
	node *parent; //needed for doing decrease-key operation
};
extern node * heapArray[500];
node * fibonacciHeapInsert(node * &root,int endPoint, int distance)
{
		node *temp = new node;
		temp->data = endPoint;
		temp->distance = distance;
		temp->child = NULL;
		temp->parent = NULL; 
		temp->next = NULL;
		temp->prev = NULL; 
		childCut = false;
		if(root!=NULL){ node *t = root->next; root->sibling = temp; temp->sibling = t; } 
		else { root = temp; root->next=root;root->degree = 0;} 
		if(root->distance > root->next->distance) root= root->sibling; //fix root pointer to point to node with minimum value.
		return temp; //return the pointer to newly inserted node.
}

void printFibonacciHeap(node *root)
{
	if(root == NULL) return;
	cout<<" "<<root->data<<" "<<root->distance<<endl;
	printFibonacciHeap(root->child);
	node *storedRoot = root;
	root=root->next;
	while(root != storedRoot) {cout<<" " <<(root->data)<< " "<<root->distance<<endl; root = root->next;}

}

node * meld(node * &root1,node * &root2)
{
	cout<<endl<<"Inside meld()" <<endl;
	if(root2 == NULL) return root1;
	if(root1 == NULL) return root2;
	node *temp1 = root1->next;
	root1->next = root2->sibling;
	root1->next->sibling = root2;
	root2->next = temp1;
	temp1->next = root1;
	if(root1->data < root2->data) return root1;
	else return root2;
}

node * pairwiseCombine(node * &root1, node * &root2)
{
	//do pairwise combine
	//node * sibRoot = root->next;
	node * root = root2;
	node * sibRoot = root1;

	if(sibRoot->distance < root->distance)
	{
		node *t = sibRoot->child;
		if(t == NULL) t = root;
		node *sRoot = root->next;//save Root's sibling
		sibRoot->child = root;
		root->parent = sibRoot;
		root->next = t;

		//fix circular link list pointer
		node *temp = t;
		if(temp)
		{
			while(temp->next != t)	temp = temp->sibling;//reach the last node in cicular linked list
			temp->next = root;
		}
		//fix the case when sibRoot's next points to root
		sibRoot->next = sRoot;

		root = sibRoot;//set root to pointer to new root
		++(root->degree);
	}
	else
	{
		node *t  = root->child;
		if(t == NULL) t = sibRoot;
//		cout<< "t->data: "  << t->data<<endl;
		node *sSibRoot = sibRoot->next; //save sibRoot's sibling 
		root->child = sibRoot;
		sibRoot->parent = root;
		sibRoot->next = t;	

		//fix circular link list pointer
		node *temp = t;
		if(temp)
		{
			while(temp->next != t)	temp = temp->sibling;//reach the last node in cicular linked list
			temp->next = sibRoot;
		}

		//fix the case when sibRoot's next points to root
		root->next = sSibRoot;
		++(root->degree);
	}
	cout<<"bheap inside pairwise Combine"<<endl;
	//printFibonacciHeap(root);
	cout<<"root->degree:  " << root->degree<<endl;
	return root;
}

void debug(node * root)
{
	cout<<endl<<endl<<"inside debug function";
	if(root) cout<<"new root's data: "<< root->data<<"|root->distance: " << root->distance<<endl;
	if( root && root->child) cout<<"new root's child data : "<<root->child->data<<"|root->child->distance: " << root->child->distance<<endl;
	if(root && root->next) cout<<"new roots' sibling data : "<<root->sibling->data <<"|root->sibling->distance: " << root->sibling->distance<<endl;
	if( root && root->next && root->sibling->sibling) cout<< "new root's sibling->sibling->data: " <<root->sibling->sibling->data<<"|root->sibling->sibling->distance: " << root->sibling->sibling->distance<<endl;
	if(root && root->child && root->child->next) cout<< "new root's child->sibling->data: " <<root->child->sibling->data<<"|root->child->sibling->distance: " << root->child->sibling->distance<<endl;
	if(root && root->next && root->sibling->child) cout<< "new root's sibling->child->data: " <<root->sibling->child->data<<"|root->sibling->child->distance: " << root->sibling->child->distance<<endl;
	if(root && root->child && root->child->child) cout<<"new root->child->child data : "<<root->child->child->data<<"|root->child->child->distance: " << root->child->child->distance<<endl;
	if(root && root->child && root->child->next && root->child->sibling->sibling) cout<< "new root's child->sibling->sibling->data: " <<root->child->sibling->sibling->data<<"|root->child->sibling->sibling->distance: " << root->child->sibling->sibling->distance<<endl;

}

node * removeMin(node * &root,int k) //delete the min node and return it
{
	if(root ==NULL) return NULL;
	cout<<"removeMin root:" << root->data <<"| removeMin distance:  " <<root->distance <<endl <<" root->next->data: " << root->sibling->data<<endl ;
	node * treeTable[MAXDEGREE];
	for(int i=0;i<MAXDEGREE;i++) treeTable[i] = NULL;

	node * toReturn = new node; toReturn->data =root->data; toReturn->distance = root->distance;
	node *rootChild = root->child;

	cout<< " xx: ";

	cout<<"before copying"<<endl;
	debug(root);
	//printFibonacciHeap(root);

	if(root->next == root)
	{
		delete(root);
		root = rootChild;
	}
	else
	{
		node  * sibRoot = root->next , *tt = root;
		while(tt->next != root) {tt = tt->sibling;}
		tt->next = sibRoot;
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
			root = root->next;
		}
	}

	cout<<"after doing pariwise combine " <<endl;
	debug(root);
	for(int i = 0;i<4;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
	cout<<endl;


	//now fix the root pointer to point to node with min data
	storedRoot = root; //store root pointer temporarily.
	node * potentialRoot=root;
	int counter = 0 ;
	while(root && root->next != storedRoot)
	{
		cout<<"inside while loop" ;
		root=root->next;
		if(root->distance < potentialRoot->distance) potentialRoot = root;
		++counter; 
		if(counter>5){cout<<endl<<endl<<"!!!!!!        infinite loop       !!!!!!!!!"; break;}
	}
	root=potentialRoot;
	if(root)  root->parent = NULL; //TODO: hack remove it later
	for(int i = 0;i<4;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
	cout<<endl;

	return toReturn;
}

void decreaseKey(node * &root, node * &t, int newValue )  // t being pointer to the the node whose decrease key is to be performed.
{
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
		if(t->next != t) t2 = t->sibling;
		node *t3 = NULL;

		node *p = t->parent;
		node *p1 = p->child;
		node *p2 = p->next;
		node *px = p->next;
		node *tx = t2;
		node *t4 = p1->child;
		int cntx = 0; 
		while(tx && tx->next != p1){ tx = tx->sibling; ++cntx; if(cntx >10){cout<<"fell into infinite loop" ;  break;} } 
		while(px->next !=p) px = px->sibling;
		if(p) t3 = p->parent;

		t->child = p;
	 	p->parent =t;
		p->child = t1;
		if(t4 && t4 != t->child && p1 != t) { p->child = t4; t4->parent = p;} //lastest addition
		if(t2) p->next = t2;
		else p->next = p;
		if(p1 != t) p1->next = p; 
		if(p2 != p) t->next = p2;//only if p2!=p
		else t->next = t;
		if(t) t->parent = t3;
		if(t3) t3->child = t;
		//p2->next = t;
		if(px != p) px->next = t;
		if(t1) t1->parent = p;
		if(tx && tx != t) tx->next = p;

	}
	//fix the root pointer if needed
	if(root && t->parent == NULL && t->distance < root->distance)    
		root = t;
	cout<<"inside decrease key function: "<<endl;
	debug(root);
	//printFibonacciHeap(root);
}

void dijkstraFibonacciHeap(graph *g,int sourceVertex)
{
	intializeSingleSource(g,sourceVertex);
	bool visited[g->nVertices];

	for(int i =0;i<g->nVertices;i++) { d[i] = INF; visited[i] = false;} //intial estimate of source to all other vertices and no vertices have been visited yet.

	d[sourceVertex] = 0; //distance of source vertex to source vertex is zero
	
	node * root = NULL;
//	node * heapArray[g->nVertices];
	//heapArray[0]  = binomialHeapInsert(root,sourceVertex,d[sourceVertex]);
	for(int i = 0;i<g->nVertices;i++) heapArray[i] = binomialHeapInsert(root,i,d[i]);
	cout<< "nVertices is " << g->nVertices<<" "<<endl;
	cout<<"1st root->data : "<<root->data<<endl;
	//printBinomialHeap(root) ;
	cout<<endl;
	
	for(int k =0;k<g->nVertices;k++)
	{
		cout<<endl<<endl<<endl<< "binomialHeap when k = " << k<< endl ; 
		for(int i = 0;i<g->nVertices;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
		cout<<endl;
		//printBinomialHeap(root) ;
		node * min = removeMin(root,k);
		if(min == NULL) break; //nothing in heap, break out .
		cout<<"for loop  "<<endl; 
		int minIndex = min->data;		
		cout<<"minIndex is "<<minIndex<<endl    ; //<<"| min->distance is "<<min->distance<< " "<< "min->child->data "<< min->child->data<< endl ;
		heapArray[minIndex] = NULL; //set the pointer to NULL as it has been removed from heap
		struct edgeNode *e = g->edges[minIndex] , *temp;temp = e;
		if(e==NULL) cout<<" ya";		
		cout<<endl<<"before decrease key: bheap after k=" <<k <<endl;
		//cout<< e->endPoint << " "<< e->weight<< endl;
		debug(root); 
		//printBinomialHeap(root);
		while(e != NULL)
		{
			cout<< "here " ;
			int u = minIndex, v = e->endPoint, w = e->weight;
	
			cout << "u is: " << u<< " v is : "<<v<<"w is : " << w;
			if(heapArray[v]) cout<<"| heapArray[v]:data:" << heapArray[v]->data<<endl;	
			if(d[v] > d[u] + w) 
			{
				d[v] = d[u] + w;
				decreaseKey(root, heapArray[v], d[v]);
			}
			e=e->next;
		}
		visited[minIndex] = true;

		cout<<endl<<endl<<"distance array is: ";
		for(int j =0;j<g->nVertices;j++) cout<<  d[j]<<" ";

		cout<<endl<<"bheap after k=" <<k <<endl;
		debug(root);
		//printBinomialHeap(root);
	}
//	cout<<endl<<"distance array is:  ";
//	for(int j =0;j<g->nVertices;j++) cout<<  d[j]<<" ";
}
