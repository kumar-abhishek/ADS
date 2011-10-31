#include<iostream>
#include<sstream>
#include<queue>
#include<cassert>
using namespace std;

#define MAXDEGREE 10 //max degree for a 500 vertex binomail heap cant be more than lg(500) = 9 <=10 
#define MAXV 1000       /* maximum number of vertices */
#define INF 99999

extern int nVertices ;//TODO:remove this global variable 
int nver ; //TODO:remove this global variable:

struct fibNode
{
	int data; //used to denote vertex endPoint for dijkstra
	int distance; //as an estimate of shortest distance : for dijkstra
	int degree;
	bool childCut;
	fibNode *child;
	fibNode *next;
	fibNode *prev;
	fibNode *parent; //needed for doing decrease-key operation
};

fibNode * fibHeapArray[500];

fibNode * fibonacciHeapInsert(fibNode * &root,int endPoint, int distance)
{
		fibNode *temp = new fibNode;
		temp->data = endPoint;
		temp->distance = distance;
		temp->child = NULL;
		temp->parent = NULL; 
		temp->childCut = false; 
		if(root!=NULL) { fibNode *t = root->next; root->next = temp;temp->prev = root; temp->next = t; t->prev = temp; } 
		else { root = temp; root->next=root; root->prev = root; root->degree = 0;} 
		if(root->distance > temp->distance) root= temp; //fix root pointer to point to fibNode with minimum value.
		return temp; //return the pointer to newly inserted fibNode.
}

void printFibonacciRec(fibNode *root,int nVertices)
{
	static int level =0 ;
	if(root == NULL) return;
//	if(nver>nVertices) return;
	assert(nver<nVertices);
	fibNode *storedRoot = root;
	cout<<"| level: "<<level <<", data: " <<(root->data)<< ", distance: "<<root->distance ; 
	do
	{
		cout<<"| level: "<<level <<", data: " <<(root->data)<< ", distance: "<<root->distance ; 
		++nver;
		++level;
		printFibonacciRec(storedRoot->child,nVertices);
		--level;
		root = root->next;
	} while(root!=storedRoot);

	cout<<endl;
}

void printFibonacciHeap(fibNode *root,int nVertices)
{
	nver=0;
	cout<<endl<<endl<<"Printing Fibonacci Heap" <<endl<<endl;
	printFibonacciRec(root,nVertices);

}


fibNode * meldWithOriginalRoot(fibNode * &root1,fibNode * &root)
{
	//root1 is a root with subtree and root is root of original tree which is in the top level linked list of fibHeap.
	//cout<<endl<<"Inside meld()" <<endl;
	if(root == NULL) return root1;
	if(root1 == NULL) return root;

	fibNode *rootPrev = root->prev;
	root1->next = root;
	root->prev = root1;
	rootPrev->next = root1;
	root1->prev = rootPrev;

	if(root1->distance < root->distance) return root1;
	else return root;
}


void debug(fibNode * root)
{
	//cout<<endl<<endl<<"inside debug function";
	if(root) cout<<"new root's data: "<< root->data<<"|root->distance: " << root->distance<<endl;
	if( root && root->child) cout<<"new root's child data : "<<root->child->data<<"|root->child->distance: " << root->child->distance<<endl;
	if(root && root->next) cout<<"new roots' next data : "<<root->next->data <<"|root->next->distance: " << root->next->distance<<endl;
	if( root && root->next && root->next->child && root->child && root->child->child) cout<< "new root's next->next->data: " <<root->next->child->data<<"|root->child->child->distance: " << root->child->child->distance<<endl;
	if(root && root->child && root->child->next) cout<< "new root's child->next->data: " <<root->child->next->data<<"|root->child->next->distance: " << root->child->next->distance<<endl;
	if(root && root->next && root->next->child) cout<< "new root's next->child->data: " <<root->next->child->data<<"|root->next->child->distance: " << root->next->child->distance<<endl;
	if(root && root->child && root->child->child) cout<<"new root->child->child data : "<<root->child->child->data<<"|root->child->child->distance: " << root->child->child->distance<<endl;
}


fibNode * pairwiseCombine(fibNode * &root1, fibNode * &root2)
{
	//do pairwise combine
	//fibNode * sibRoot = root->next;
	fibNode * root = root2;
	fibNode * sibRoot = root1;

	if(sibRoot->distance < root->distance)
	{
		fibNode *t = sibRoot->child;
		if(t == NULL) t = root;
		fibNode *sRoot = root->next;//save Root's next
		sibRoot->child = root;
		root->childCut = false;
		root->parent = sibRoot;
		root->next = t;

		//fix circular link list pointer
		fibNode *temp = t;
		if(temp)
		{
			int cc=0;
			while(temp->next != t)
			{
				temp = temp->next;//reach the last fibNode in cicular linked list
				++cc;
				if(cc>10) 
				{
					//cout<<endl<<endl<<"!!!!! INFINITE LOOP !!!!!" << __LINE__;
					break;
				}
			}
			temp->next = root;
			root->prev = temp;
		}
		//fix the case when sibRoot's next points to root
		sibRoot->next = sRoot;
		sRoot->prev = sibRoot;

		root = sibRoot;//set root to pointer to new root
		++(root->degree);
	}
	else
	{
		fibNode *t  = root->child ;
		fibNode *sSibRoot = sibRoot->next; //save sibRoot's next 
		if(t==NULL) //root2 has no child
		{
			t = sibRoot;
			root->child = sibRoot;
			sibRoot->parent = root;
			sibRoot->next = sibRoot;	
			sibRoot->prev = sibRoot;
			root->next = sSibRoot;
			sSibRoot->prev = root;
		}
		else //root2 has children
		{
			root->child = sibRoot;
			sibRoot->parent = root;

			//fix circular link list pointer
			fibNode *temp = t;
			int cc=0;
			if(temp)
			{
				while(temp->next != t)
				{
					temp = temp->next;//reach the last fibNode in cicular linked list
					++cc;
					if(cc>10) 
					{
						//cout<<endl<<endl<<"!!!!! INFINITE LOOP !!!!!" << __LINE__;
						break;
					}
				}
				temp->next = sibRoot;
				sibRoot->prev = temp;
			}
			sibRoot->next = t;
			t->prev = sibRoot;

		}
		sibRoot->childCut = false;
		//fix the case when sibRoot's next points to root
		root->next = sSibRoot;
		sSibRoot->prev = root;
		++(root->degree);
	}
	//cout<<"bheap inside pairwise Combine"<<endl;
	//debug(root);
	//printFibonacciHeap(root,nVertices);
	//cout<<"root->degree:  " << root->degree<<endl;
	return root;
}
fibNode * removeMin(fibNode * &root,int k) //delete the min fibNode and return it
{
	if(root == NULL) return NULL;
	//cout<<endl<<endl<<"removeMin root:" << root->data <<"| removeMin distance:  " <<root->distance <<endl <<" root->next->data: " << root->next->data<<endl ;
	fibNode * treeTable[MAXDEGREE];
	for(int i=0;i<MAXDEGREE;i++) treeTable[i] = NULL;

	fibNode * toReturn = new fibNode; toReturn->data =root->data; toReturn->distance = root->distance;
	fibNode *rootChild = root->child;
	if(rootChild)
	{ 
		//cout<<endl<<"rootChild->data: "<< root->child->data<<endl;
	}
	//cout<<"before return"<< toReturn->data<<endl;

	//cout<< " xx: ";
	fibNode * toCopy = root->next;

	//cout<<"before copying"<<endl;
	//debug(root);
	//printFibonacciHeap(root,nVertices);

	if(root->next == root)
	{
		delete(root);
		root = rootChild;
		return toReturn;
	}
	else
	{
		fibNode  * sibRoot = root->next , *tt = root;
		int cc=0;
		while(tt->next != root)
		{
			tt = tt->next;
			++cc;
			if(cc>10) 
			{
				//cout<<endl<<endl<<"!!!!! INFINITE LOOP !!!!!" << __LINE__;
				break;
			}
		}

		tt->next = sibRoot;
		sibRoot->prev = tt;
		//	delete(root);
		root = sibRoot;
	}

	//cout<<"before melding"<<endl;
	//debug(root); //printFibonacciHeap(root,nVertices);

/*
	for(int i = 0;i<6;i++) if( fibHeapArray[i] )
	{
		cout<<"i= "<<i <<" " <<fibHeapArray[i]->data << " "<< fibHeapArray[i]->distance<< "| ";
	}
*/

	if(rootChild !=NULL && root != rootChild)	root = meldWithOriginalRoot(rootChild,root); //meld the new root and old root's child

	//cout<<"after melding"<<endl;
	//debug(root); //printFibonacciHeap(root,nVertices);

	//find new root
	fibNode *tempRoot = root->next;
	fibNode *storedRoot = root; //store root pointer temporarily.
	while(tempRoot != storedRoot)
	{
		if(tempRoot->distance < root->distance)
		{
			root = tempRoot;
		}
		tempRoot = tempRoot->next;
	} 	
	
	//do pairwise combine
	storedRoot = root; //store root pointer temporarily.
	//cout<<"before doing pairwise combine " <<endl;
	//debug(root);
	int cnt = 0;
	while(1)
	{
		//cout<<endl<<endl<<"count: " << cnt<<endl<<endl;
		if(root == NULL) break;
		if(root->degree > MAXDEGREE || root->degree < 0) 
		{
			//cout<<endl<<endl<<"!!!!!         INVALID DEGREE !!!!!!!!"<<endl<<endl;
			break;
		}
		if( treeTable[root->degree] != NULL)
		{
			if(treeTable[root->degree] == root) 
				break;
			else
			{
				int tempDegree = root->degree;
				//cout<<"pairwise combining: "<<root->data<<" " <<treeTable[root->degree]->data<<endl;
				fibNode * x= root->next;
				int cc=0;
				while(x->next != root)
				{
					x=  x->next;
					++cc; 
					if(cc>10)
					{
						//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
						break;
					}
				}
				if(x->child)
				{
					//cout<<"root->prev->child:  "<< x->child->data<<endl;
				}
				else
				{
					//cout<<"root->prev->child is NULL"<<endl;
				}
				root = pairwiseCombine(root, treeTable[root->degree]);
				//debug(root);
				treeTable[tempDegree] = NULL;
			}
		}
		else
		{
			treeTable[root->degree] = root;
			root = root->next;
			fibNode * x= root->next;
			int cc = 0;
			while(x->next != root)
			{
				x=  x->next;
				++cc; 
				if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					break;
				}
			}
			if(x->child)
			{
				//cout<<"root->prev->child:  "<< x->child->data<<endl;
			}
			else
			{
				//cout<<"root->prev->child is NULL"<<endl;
			}

		}
		//cout<<endl;
		//printFibonacciHeap(root,nVertices);
		//cout<<endl;
		//debug(root);
		++cnt;
		//	if(cnt > 5) break;
	}

	//cout<<"Bheap after pairwise combine " <<endl;
	//printFibonacciHeap(root,nVertices);
	//cout<<"after doing pariwise combine " <<endl;
	//debug(root);
	//printFibonacciHeap(root,nVertices);
//	for(int i = 0;i<4;i++) if( fibHeapArray[i] ) cout<<"i= "<<i <<" " <<fibHeapArray[i]->data << " "<< fibHeapArray[i]->distance<< "| ";
//	cout<<endl;


	//now fix the root pointer to point to fibNode with min data
	storedRoot = root; //store root pointer temporarily.
	fibNode * potentialRoot=root;
	int counter = 0 ;
	while(root && root->next != storedRoot)
	{
//		cout<<"inside while loop" ;
		root=root->next;
		if(root->distance < potentialRoot->distance) potentialRoot = root;
		++counter; 
		if(counter>10)
		{
			//cout<<endl<<endl<<"!!!!!!        infinite loop       !!!!!!!!!";
			break;
		}
	}
	root=potentialRoot;
	if(root)  root->parent = NULL; //TODO: hack remove it later
//	cout<<"new root's data"<< root->data<<endl<< "new root's child "<<root->child->data<<endl<<" new roots' next"<<root->next->data <<endl;
//	for(int i = 0;i<4;i++) if( fibHeapArray[i] ) cout<<"i= "<<i <<" " <<fibHeapArray[i]->data << " "<< fibHeapArray[i]->distance<< "| ";
//	cout<<endl;

	return toReturn;
}

void decreaseKey(fibNode * &root, fibNode * &t, int newValue )  // t being pointer to the the fibNode whose decrease key is to be performed.
{
	//error check
	if(t == NULL || root == NULL) 
	{
		//cout<<"!!! ERROR !!! NULL Value in decreaseKey";
		return;
	}

	// decrease t->distance 
	t->distance = newValue;

	if(t->parent != NULL)
	{
		fibNode *tParent = t->parent;
		//  check if t's dist is still less than t->parent->dist, if no, return 
		if(t->distance >= t->parent->distance)
		{
			return;
		}

		//  else take t out of the heap and meld with top level list
		else
		{
			tParent->childCut = true;
			tParent->degree = 0;
			tParent->child = t->next;			
	
			t->parent = NULL;
			root = meldWithOriginalRoot(t,root);

			//check if t's old parent's childCut is T, if T then trigger cascadeCut else return
			fibNode *tgParent = tParent->parent;
			while(tParent && tgParent &&  tParent->childCut == true)
			{
				tgParent->childCut = true;
				tgParent->degree -= 1;
				tgParent->child = tParent->next;			
			
				tParent->parent = NULL;
				root = meldWithOriginalRoot(tParent,root);
				
				tParent = tgParent;
				tgParent = tParent->parent;
			}

		}
	}
	else 
	{
		//t->parent is NULL, update root pointer if needed
		if(root->distance > t->distance)
		{
			root = t;
			return;
		}
	}



//update root pointer if needed

}

void dijkstraFibonacciHeap(graph *g,int sourceVertex)
{
	nVertices = g->nVertices;
	intializeSingleSource(g,sourceVertex);
	bool visited[g->nVertices];

	for(int i =0;i<g->nVertices;i++) { d[i] = INF; visited[i] = false;} //intial estimate of source to all other vertices and no vertices have been visited yet.

	d[sourceVertex] = 0; //distance of source vertex to source vertex is zero
	
	fibNode * root = NULL;
//	fibNode * fibHeapArray[g->nVertices];
	//fibHeapArray[0]  = fibonacciHeapInsert(root,sourceVertex,d[sourceVertex]);
	nVertices = g->nVertices; //TODO: REMOVE THIS LINE LATER
	for(int i = 0;i<g->nVertices;i++) fibHeapArray[i] = fibonacciHeapInsert(root,i,d[i]);
//	cout<< "nVertices is " << g->nVertices<<" "<<endl;
//	cout<<"1st root->data : "<<root->data<<endl;
	//printFibonacciHeap(root,nVertices) ;
	//debug(root);
//	cout<<endl;
	
	for(int k =0;k<g->nVertices;k++)
	{
//		cout<<endl<<endl<<endl<< "fibonacciHeap when k = " << k<< endl ; 
		for(int i = 0;i<g->nVertices;i++) 
			if( fibHeapArray[i] ) 
			{
				//cout<<"i= "<<i <<" " <<fibHeapArray[i]->data << " "<< fibHeapArray[i]->distance<< "| ";
			}
		//		cout<<endl;
		//printFibonacciHeap(root,nVertices) ;
		fibNode * min = removeMin(root,k);
		if(min == NULL) break; //nothing in heap, break out .
//		cout<<"for loop  "<<endl; 
		int minIndex = min->data;		
//		cout<<"minIndex is "<<minIndex<<endl    ; //<<"| min->distance is "<<min->distance<< " "<< "min->child->data "<< min->child->data<< endl ;
		fibHeapArray[minIndex] = NULL; //set the pointer to NULL as it has been removed from heap
		struct edgeNode *e = g->edges[minIndex] , *temp;temp = e;
		//cout<<endl<<"before decrease key: bheap after k=" <<k <<endl;
//		if(e) cout<<"e->endPoint: "<< e->endPoint << "e->weight: "<< e->weight<< endl;
		//debug(root); 
		//printFibonacciHeap(root,nVertices);
		while(e != NULL)
		{
//			cout<< "here " ;
			int u = minIndex, v = e->endPoint, w = e->weight;
	
			//			cout << "u is: " << u<< " v is : "<<v<<"w is : " << w;
			if(fibHeapArray[v])
			{
//				cout<<"| fibHeapArray[v]:data:" << fibHeapArray[v]->data<<endl;	
			}
			if(d[v] > d[u] + w) 
			{
				d[v] = d[u] + w;
				decreaseKey(root, fibHeapArray[v], d[v]);
			}
			e=e->next;
		}
		visited[minIndex] = true;

		//		cout<<endl<<endl<<"distance array is: ";
		for(int j =0;j<g->nVertices;j++) 
		{
//			cout<<  d[j]<<" ";
		}

//		cout<<endl<<"bheap after k=" <<k <<endl;
		//debug(root);

//		//printFibonacciHeap(root,g->nVertices);
	}
	//cout<<endl;
	//cout<<endl<<"final d array: ";
//	for(int j =0;j<g->nVertices;j++) cout<<  d[j]<<" ";
	//printFibonacciHeap(root,g->nVertices);
}
