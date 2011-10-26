#include<iostream>
#include<string>
#include<cstdio>
#include<algorithm>
#include "binomialHeap.h"
using namespace std;

#define MAXV 1000       /* maximum number of vertices */
#define INF 99999

int d[MAXV];
struct edgeNode {
	int endPoint; /* adjacency info */
	int weight; /* edge weight, if any */
	struct edgeNode *next;     /* next edge in list */
} ;

struct graph {
	struct edgeNode *edges[MAXV+1];   /* adjacency info */
	int nVertices; /* number of vertices in graph */
	int nEdges ; /* number of edges in graph */
} ;

void initializeGraph(graph *g)
{
	g-> nVertices = 0;
	g-> nEdges = 0;
	for (int i=0; i<MAXV; i++) g->edges[i] = NULL;
}

void insertEdge(graph *g, int x, int y, int cost)
{
	edgeNode *p; /* temporary pointer */
	p = new edgeNode;
	p->weight = cost;
	p->endPoint = y;
	p->next = g->edges[x];
	g->edges[x] = p; /* insert at head of list */
}

void readGraph(graph *g)
{
	int v1,v2,cost;
	initializeGraph(g);
	string input;
	getline(cin,input);
	while(input.find('*')==string::npos)
	{
		sscanf(input.c_str(),"%d %d %d",&v1,&v2,&cost);
		++(g->nEdges); //increment number of edges in graph
		g->nVertices = max(g->nVertices,max(v1,v2)); //highest vertex index entered user defines the number of vertices in the graph - 1 
		insertEdge(g,v1,v2,cost);
		getline(cin,input);
	}
        if(g->nVertices !=0 ) ++(g->nVertices) ; //update to be equal to say that this is number of vertices in graph. 

}

void printGraph(graph *g)    //print graph:
{
        for(int i=0;i< g->nVertices; i++)
        {
                struct edgeNode *temp = g->edges[i];
                cout<<endl<<"edges from vertex  "<< i <<": ";
                while(temp!=NULL) { cout<< temp->endPoint << " " << temp->weight<< "\t";temp=temp->next;}
        }
}

void intializeSingleSource(graph *g,int sourceVertex )
{
	for(int i =0;i<g->nVertices;i++)
	{
		d[i] = INF;
	}
	d[sourceVertex] = 0;
}

void dijkstraBinomialHeap(graph *g,int sourceVertex)
{
	intializeSingleSource(g,sourceVertex);
	bool visited[g->nVertices];

	for(int i =0;i<g->nVertices;i++) { d[i] = INF; visited[i] = false;} //intial estimate of source to all other vertices and no vertices have been visited yet.

	d[sourceVertex] = 0; //distance of source vertex to source vertex is zero
	
	node * root = NULL;
	node * heapArray[g->nVertices];
	heapArray[0]  = binomialHeapInsert(root,sourceVertex,d[sourceVertex]);
	for(int i = 1;i<g->nVertices;i++) heapArray[i] = binomialHeapInsert(root,i,d[i]);
	cout<< "nVertices is " << g->nVertices<<" "<<endl;
	cout<<"1st root->data : "<<root->data<<endl;
	printBinomialHeap(root) ;
	cout<<endl;
	
	for(int k =0;k< 1;k++)
	{
		cout<< "binomialHeap when k = " << k<< endl ; printBinomialHeap(root) ;
		node * min = removeMin(root); 
//		if(min) cout<<"here " <<min->distance<< " "<<endl ;
//		int minIndex = min->data;		
//		cout<<"minIndex is "<<minIndex<<"| min->distance is "<<min->distance<< " "<<endl ;;
		printBinomialHeap(root);
//		struct edgeNode *e = g->edges[minIndex] , *temp;temp = e;
//		cout<< e->endPoint << " "<< e->weight<< endl;
/*
		
		while(e)
		{
			int u = minIndex, v = e->endPoint, w = e->weight;
			
			if(d[v] > d[u] + w) 
			{
				d[v] = d[u] + w;
				decreaseKey(root, heapArray[v], d[v]);
			}
			e=e->next;
		}
		visited[minIndex] = true;
*/
		cout<<endl<<endl<<"distance array is: ";
		for(int j =0;j<g->nVertices;j++) cout<<  d[j]<<" ";

	}
//	cout<<endl<<"distance array is:  ";
//	for(int j =0;j<g->nVertices;j++) cout<<  d[j]<<" ";
}





int main()
{
	graph *g = new graph;
	initializeGraph(g);
	readGraph(g);
	printGraph(g);
	cout<<endl;
	int sourceVertex = 0;
	dijkstraBinomialHeap(g,sourceVertex);
//	for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	DIJKSTRA(g,sourceVertex);
}

