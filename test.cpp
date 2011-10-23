#include<iostream>
#include<string>
#include<cstdio>
using namespace std;

#define MAXV 1000       /* maximum number of vertices */
struct edgeNode {
	int y; /* adjacency info */
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
	p->y = y;
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
                while(temp!=NULL) { cout<< temp->y << " " << temp->weight<< "\t";temp=temp->next;}
        }
}

int main()
{
	graph *g = new graph;
	initializeGraph(g);
	readGraph(g);
	printGraph(g);
}

