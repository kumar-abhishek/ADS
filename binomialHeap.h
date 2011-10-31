#include<iostream>
#include<sstream>
#include<queue>
using namespace std;

#define MAXDEGREE 10 //max degree for a 500 vertex binomail heap cant be more than lg(500) = 9 <=10 
#define MAXV 1000       /* maximum number of vertices */
#define INF 99999

int nVertices = 0 ;//TODO:remove this global variable 

struct node
{
	int data; //used to denote vertex endPoint for dijkstra
	int distance; //as an estimate of shortest distance : for dijkstra
	int degree;
	node *child;
	node *sibling;
	node *parent; //needed for doing decrease-key operation
};

node * heapArray[500];

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

void printBinomialHeap(node *root,int nVertices)
{
	static int nver=0,level =0 ;
	if(nver>nVertices) return;
	if(root == NULL) return;
	node *storedRoot = root;
	cout<<"| level: "<<level <<", data: " <<(root->data)<< ", distance: "<<root->distance ; 
	do
	{
		cout<<"| level: "<<level <<", data: " <<(root->data)<< ", distance: "<<root->distance ; 
		++nver;
		++level;
		//printBinnoomialHeap(storedRoot->child,nVertices);
		--level;
		root = root->sibling;
	} while(root!=storedRoot);

	cout<<endl;
}



node * meld(node * &root1,node * &root2)
{
//	cout<<endl<<"Inside meld()" <<endl;
	if(root2 == NULL) return root1;
	if(root1 == NULL) return root2;
	node *temp1 = root1->sibling;
	root1->sibling = root2->sibling;
	root1->sibling->sibling = root2;
	root2->sibling = temp1;
	temp1->sibling = root1;
	if(root1->distance < root2->distance) return root1;
	else return root2;
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
			int cc=0;
			while(temp->sibling != t)
			{
				temp = temp->sibling;//reach the last node in cicular linked list
				++cc;
				if(cc>10) 
				{
					//cout<<endl<<endl<<"!!!!! INFINITE LOOP !!!!!" << __LINE__;
					break;
				}
			}
			temp->sibling = root;
		}
		//fix the case when sibRoot's sibling points to root
		sibRoot->sibling = sRoot;

		root = sibRoot;//set root to pointer to new root
		++(root->degree);
	}
	else
	{
		node *t  = root->child ;
		node *sSibRoot = sibRoot->sibling; //save sibRoot's sibling 
		if(t==NULL) //root2 has no child
		{
			t = sibRoot;
			root->child = sibRoot;
			sibRoot->parent = root;
			sibRoot->sibling = sibRoot;	
			root->sibling = sSibRoot;
		}
		else //root2 has children
		{
			root->child = sibRoot;
			sibRoot->parent = root;

			//fix circular link list pointer
			node *temp = t;
			int cc=0;
			if(temp)
			{
				while(temp->sibling != t)
				{
					temp = temp->sibling;//reach the last node in cicular linked list
					++cc;
					if(cc>10) 
					{
						//cout<<endl<<endl<<"!!!!! INFINITE LOOP !!!!!" << __LINE__;
						break;
					}
				}
				temp->sibling = sibRoot;
			}
			sibRoot->sibling = t;

		}
		//fix the case when sibRoot's sibling points to root
		root->sibling = sSibRoot;
		++(root->degree);
	}
	//cout<<"bheap inside pairwise Combine"<<endl;
	//debug(root);
	////printBinnoomialHeap(root,nVertices);
	//cout<<"root->degree:  " << root->degree<<endl;
	return root;
}
node * removeMin(node * &root,int k) //delete the min node and return it
{
	if(root ==NULL) return NULL;
	//cout<<endl<<endl<<"removeMin root:" << root->data <<"| removeMin distance:  " <<root->distance <<endl <<" root->sibling->data: " << root->sibling->data<<endl ;
	node * treeTable[MAXDEGREE];
	for(int i=0;i<MAXDEGREE;i++) treeTable[i] = NULL;

	node * toReturn = new node; toReturn->data =root->data; toReturn->distance = root->distance;
	node *rootChild = root->child;
	if(rootChild)
	{ 
		//cout<<endl<<"rootChild->data: "<< root->child->data<<endl;
	}
	//cout<<"before return"<< toReturn<<endl;

	//cout<< " xx: ";
	node * toCopy = root->sibling;

	//cout<<"before copying"<<endl;
	//debug(root);
	////printBinnoomialHeap(root,nVertices);

	if(root->sibling == root)
	{
		delete(root);
		root = rootChild;
	}
	else
	{
		node  * sibRoot = root->sibling , *tt = root;
		int cc=0;
		while(tt->sibling != root)
		{
			tt = tt->sibling;
			++cc;
			if(cc>10) 
			{
				//cout<<endl<<endl<<"!!!!! INFINITE LOOP !!!!!" << __LINE__;
				break;
			}
		}





		tt->sibling = sibRoot;
		//	delete(root);
		root = sibRoot;
	}

	//cout<<"before melding"<<endl;
	//debug(root); ////printBinnoomialHeap(root,nVertices);

	for(int i = 0;i<6;i++) if( heapArray[i] )
	{
		//cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
	}

	if(rootChild !=NULL && root != rootChild)
	{
		//root->child = meld(rootChild,root->child); //meld the new root and old root's child
	}
	if(rootChild !=NULL && root != rootChild)	root = meld(rootChild,root); //meld the new root and old root's child

	//cout<<"after melding"<<endl;
	//debug(root);
	////printBinnoomialHeap(root,nVertices);


	//do pairwise combine
	node *storedRoot = root; //store root pointer temporarily.
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
				node * x= root->sibling;
				int cc=0;
				while(x->sibling != root)
				{
					x=  x->sibling;
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
			root = root->sibling;
			node * x= root->sibling;
			int cc = 0;
			while(x->sibling != root)
			{
				x=  x->sibling;
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
		//printBinnoomialHeap(root,nVertices);
		//cout<<endl;
		//debug(root);
		++cnt;
		//	if(cnt > 5) break;
	}

	//cout<<"Bheap after pairwise combine " <<endl;
	////printBinnoomialHeap(root,nVertices);
	//cout<<"after doing pariwise combine " <<endl;
	//debug(root);
	////printBinnoomialHeap(root,nVertices);
//	for(int i = 0;i<4;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
//	cout<<endl;


	//now fix the root pointer to point to node with min data
	storedRoot = root; //store root pointer temporarily.
	node * potentialRoot=root;
	int counter = 0 ;
	while(root && root->sibling != storedRoot)
	{
//		cout<<"inside while loop" ;
		root=root->sibling;
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
//	cout<<"new root's data"<< root->data<<endl<< "new root's child "<<root->child->data<<endl<<" new roots' sibling"<<root->sibling->data <<endl;
//	for(int i = 0;i<4;i++) if( heapArray[i] ) cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
//	cout<<endl;

	return toReturn;
}

void decreaseKey(node * &root, node * &t, int newValue )  // t being pointer to the the node whose decrease key is to be performed.
{
	//if(t->data == 0 ) return;
	if(t == NULL) return; 
//	if(root ) cout<<endl<<"root->data "<<root->data; if(t) cout<< "|t->data " << t->data<<endl;
	t->distance = newValue;
//	if(t->parent !=NULL) cout<<"deckey n"<<endl;

	int count = 0 ;

	//swap node with its parent if it violates  min heap property
	while( (t->parent != NULL) && (t->distance < t->parent->distance))
	{
//		cout<<"decreaseKey count: "<<count<<endl;
		++count;
		if(count > nVertices)
		{
			//cout<<"!!! ERROR !!!" <<endl;
			return;
		}
		/*
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
		cntx = 0;
		while(px->sibling !=p) {px = px->sibling;if(cntx >10){cout<<"fell into infinite loop" ;  break;} }
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
*/
	
		node *p = t->parent;
		node *t1 = p->child;
		node *c = t->child;
		node *tx = t->sibling;
		node *txEnd = t->sibling;
		node *t2 = t1;
		node *px = p->sibling;
		node *pxEnd = p->sibling;
		node *gp = p->parent;
		node *cx = NULL;
		if(c) cx = c->sibling;

		//swap degrees of parent and child
		int tDegree = p->degree;
		p->degree = t->degree;
		t->degree = tDegree;



		int cc=0;//TODO:remove this variable

		//case 1 & 2 
		if((p->sibling ==p) && (t1==t))
		{
//			cout<<endl<<"case 1 & 2" ;
			while(txEnd->sibling !=t)
			{
				txEnd = txEnd->sibling;
				++cc;
				if(cc>10)

				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
			p->parent = t;
			t->child = p;
			t->sibling = t;
			t->parent = gp;
			if(gp && (gp->child==p)) gp->child = t;
		
			p->child =c; 
			if(c) c->parent = p;
			cc=0;
			while(c && cx!=c)
			{
				cx->parent = p;
				cx = cx->sibling;
				++cc;
				if(cc>10)

				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
			if(tx != t) 
			{
				p->sibling = tx;
				txEnd->parent =t;
				txEnd->sibling = p;
			}
			else 
				p->sibling = p;
			cc=0;
			while(tx!=txEnd)
			{
				tx->parent = t;
				tx=tx->sibling;
				++cc;
				if(cc>10)

				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
		}

		//case 3 & 4:
		else if((p->sibling !=p) && (t==t1) )
		{
//			cout<<endl<<"case 3 & 4" ;
			cc=0;
			while(pxEnd->sibling !=p)
			{ 
				pxEnd = pxEnd->sibling;
				++cc;
				if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}

			}
			cc=0;
			while(txEnd->sibling !=t)
			{
				txEnd = txEnd->sibling;
				++cc; 
				if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
			p->parent = t;
			t->child = p;
			t->parent = gp;

			if(gp && (gp->child == p)) gp->child = t;

			
			p->child =c;
			if(c) c->parent =p;
			cc= 0 ;
			while(c && cx!=c)
			{
				cx->parent = p;
				cx=cx->sibling;
				++cc; 
				if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
			pxEnd->sibling =t;
			if(tx != t) 
			{
				p->sibling = tx;
				txEnd->parent =t;
				txEnd->sibling = p;
			}
			else 
				p->sibling = p;
			int cc= 0;
			while(tx!=txEnd) 
			{
				tx->parent = t;
				tx= tx->sibling;
				++cc;
				if(cc>10){
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
			t->sibling = px;

		}


		//case 5:
		else if(t1 && (t!=t1) && (p->sibling==p))
		{
//			cout<<endl<<"case 5" ;
			p->parent =t ;
			t->child = t1;
			t1->parent =t;
			//t1->sibling = p;
			cc=0;
			while(t2->sibling !=t)
			{
				t2 = t2->sibling;
				++cc;
				if(cc>10)

				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					return;
					//break;
				}
			}
			t2->sibling = p;	

			p->sibling = tx;
			t->parent = gp;
			if(gp) gp->child = t;

			if(t1 !=tx)
			{
				cc = 0 ;
				while(tx->sibling !=t1)
				{
					tx->parent = t;
					tx= tx->sibling;
					++cc;
					if(cc>10)
					{
						//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl;
						return;
					}
				}
				tx->parent =t ;
			}
			else
				p->sibling = t1;

			p->child =c;
			if(c) c->parent =p;
			t->sibling =t;
			cc=0;
			while(c && cx!=c)
			{
				cx->parent = p;
				cx=cx->sibling;
				++cc; if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl;
					return;
					// break;
				}
			}
		}

		//case 6:
		else if(t1 && (t!=t1) && (p->sibling!=p))
		{
//			cout<<endl<<"case 6" ;
			cc=0;
			while(pxEnd->sibling !=p){
				pxEnd = pxEnd->sibling;
				++cc;
				if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl;
					return; 
					//break;
				}
			}
			p->parent =t ;
			t->child = t1;
			t1->parent =t;

			//			t1->sibling = p;
			cc=0;
			while(t2->sibling !=t)
			{
				t2 = t2->sibling;
				++cc; 
				if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
			t2->sibling = p;	
		
			p->sibling = tx;
			t->parent = gp;
			if(gp) gp->child = t;


			if(t1 !=tx)
			{
				cc=0;
				while(tx->sibling !=t1)
				{
					tx->parent = t;
					tx= tx->sibling;
					++cc; 
					if(cc>10)
					{
						//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
						//break;
						return;
					}
				}
				tx->parent =t ;
			}
			else
				p->sibling = t1;



			p->child =c;
			if(c) c->parent =p;
			t->sibling =px;
			pxEnd->sibling =t;
			cc = 0;
			while(c && cx!=c)
			{
				cx->parent = p;
				cx=cx->sibling;
				++cc; 
				if(cc>10)
				{
					//cout<<endl<<endl<<"!!!!!!! INF LOOP!!!!!!!" <<endl<<endl; 
					//break;
					return;
				}
			}
		}
		else
		{
			//otherwise
			//			cout<<endl<<endl<<"No cases in Decrease Key matched :!!!!   ERROR !!!!! " <<endl<<endl;
			//break;
			return;
		}
		//debug(root);


	}
	//fix the root pointer if needed
	if(root && t->parent == NULL && t->distance < root->distance)    
	{
//		cout<<"fixing root pointer";
		root = t;
	}
	else
	{
//		cout<<"didnt fix root pointer";
	}
//	cout<<"inside decrease key function: "<<endl;
	//debug(root);
	////print BinnoomialHeap(root,nVertices);
}

void dijkstraBinomialHeap(graph *g,int sourceVertex)
{
	nVertices = g->nVertices;
	intializeSingleSource(g,sourceVertex);
	bool visited[g->nVertices];

	for(int i =0;i<g->nVertices;i++) { d[i] = INF; visited[i] = false;} //intial estimate of source to all other vertices and no vertices have been visited yet.

	d[sourceVertex] = 0; //distance of source vertex to source vertex is zero
	
	node * root = NULL;
//	node * heapArray[g->nVertices];
	//heapArray[0]  = binomialHeapInsert(root,sourceVertex,d[sourceVertex]);
	nVertices = g->nVertices; //TODO: REMOVE THIS LINE LATER
	for(int i = 0;i<g->nVertices;i++) heapArray[i] = binomialHeapInsert(root,i,d[i]);
//	cout<< "nVertices is " << g->nVertices<<" "<<endl;
//	cout<<"1st root->data : "<<root->data<<endl;
	////printBinnoomialHeap(root,nVertices) ;
	//debug(root);
//	cout<<endl;
	
	for(int k =0;k<g->nVertices;k++)
	{
//		cout<<endl<<endl<<endl<< "binomialHeap when k = " << k<< endl ; 
		for(int i = 0;i<g->nVertices;i++) 
			if( heapArray[i] ) 
			{
				//cout<<"i= "<<i <<" " <<heapArray[i]->data << " "<< heapArray[i]->distance<< "| ";
			}
		//		cout<<endl;
		////printBinnoomialHeap(root,nVertices) ;
		node * min = removeMin(root,k);
		if(min == NULL) break; //nothing in heap, break out .
//		cout<<"for loop  "<<endl; 
		int minIndex = min->data;		
//		cout<<"minIndex is "<<minIndex<<endl    ; //<<"| min->distance is "<<min->distance<< " "<< "min->child->data "<< min->child->data<< endl ;
		heapArray[minIndex] = NULL; //set the pointer to NULL as it has been removed from heap
		struct edgeNode *e = g->edges[minIndex] , *temp;temp = e;
//		cout<<endl<<"before decrease key: bheap after k=" <<k <<endl;
		//cout<< e->endPoint << " "<< e->weight<< endl;
		//debug(root); 
		////printBinnoomialHeap(root,nVertices);
		while(e != NULL)
		{
//			cout<< "here " ;
			int u = minIndex, v = e->endPoint, w = e->weight;
	
			//			cout << "u is: " << u<< " v is : "<<v<<"w is : " << w;
			if(heapArray[v])
			{
//				cout<<"| heapArray[v]:data:" << heapArray[v]->data<<endl;	
			}
			if(d[v] > d[u] + w) 
			{
				d[v] = d[u] + w;
				decreaseKey(root, heapArray[v], d[v]);
			}
			e=e->next;
		}
		visited[minIndex] = true;

		//		cout<<endl<<endl<<"distance array is: ";
//		for(int j =0;j<g->nVertices;j++) 
//		{
//			cout<<  d[j]<<" ";
//		}

//		cout<<endl<<"bheap after k=" <<k <<endl;
		//debug(root);

//		printBinnoomialHeap(root,g->nVertices);
	}
	//cout<<endl<<"final d array" <<endl<<endl;
//	cout<<endl;
//	for(int j =0;j<g->nVertices;j++) cout<<  d[j]<<" ";
	//printBinnoomialHeap(root,g->nVertices);
}
