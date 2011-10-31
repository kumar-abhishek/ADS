#include<iostream>
#include<string>
#include<cstdio>
#include<algorithm>
using namespace std;

#define MAXV 1000       /* maximum number of vertices */
#define INF 99999

int d[MAXV];
struct edgeNode {
	int endPoint; /* endPoint of an edge */
	int weight; /* edge weight */
	struct edgeNode *next;     /* pointer to next edge in list */
} ;

struct graph {
	struct edgeNode *edges[MAXV+1];   /* adjacency info */
	int nVertices; /* number of vertices in the graph */
	int nEdges ; /* number of edges in the graph */
} ;

void initializeGraph(graph *g)
{
	g-> nVertices = 0;
	g-> nEdges = 0;
	for (int i=0; i<MAXV; i++) g->edges[i] = NULL;
}

void insertEdge(graph *g, int x, int y, int cost)
{
	edgeNode *p; /* temp pointer */
	p = new edgeNode;
	p->weight = cost;
	p->endPoint = y;
	p->next = g->edges[x];
	g->edges[x] = p; /* insert at head of list */
}

void readGraphFromUser(graph *g)
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

void readGraphFromFile(graph *g,char filename[])
{
	int v1,v2,cost;
	initializeGraph(g);
	string input;

	ifstream myfile(filename);
	if(!myfile.is_open())
	{
		cout<<"Not able to find the input file";
		return;
	}

	while(myfile.good())
	{
		getline(myfile,input);
		sscanf(input.c_str(),"%d %d %d",&v1,&v2,&cost);
		++(g->nEdges); //increment number of edges in graph
		g->nVertices = max(g->nVertices,max(v1,v2)); //highest vertex index entered user defines the number of vertices in the graph - 1 
		insertEdge(g,v1,v2,cost);
	}
	if(g->nVertices !=0 ) ++(g->nVertices) ; //update to be equal to say that this is number of vertices in graph. 
	myfile.close();
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

void relax(int u,int v,int w)
{
	if(d[v] > d[u] + w) 
		d[v] = d[u] + w;
}

