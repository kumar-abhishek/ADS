#include<iostream>
#include<cstring>
#include<fstream>
#include "graph.h"
#include "dijkstraSimpleScheme.h"
//#include "fibonacciHeap.h"
#include "binomialHeap.h"
#include<ctime>
#include<cstdlib>
using namespace std;

int randGenerator(int range)
{
	return (rand() % range) ;
}

int main(int argc, char *argv[])
{
	graph *g = new graph; //define graph

	if(strcmp(argv[1], "-r") == 0) 
	{
		cout<<"random mode"<<endl;
		int v1,v2,cost;
		int vnum = 0, density = 0, nedges = 0;

		if(argv[2]) vnum = atoi(argv[2]);//get number of vertices
		if(vnum == 0 ) vnum = 5; //user didnt specify the number of vertices
		
		if(argv[3]) density = atoi(argv[3]);//get density of graph
		if(density == 0 ) density = 50; //user didnt specify density, so assume 50%
		
		nedges = (density*(vnum)*(vnum-1))/100 + 1;   // calculate number of edges needed to generate for the specified density		

		initializeGraph(g);

		//generate random graph 	
		srand(time(NULL));//initialize random seed
		for(int i=0;i<nedges;i++)
		{
			v1 = randGenerator(vnum);
			v2 = randGenerator(vnum);
			cost = randGenerator(1000) + 1;

			++(g->nEdges); //increment number of edges in graph
			g->nVertices = max(g->nVertices,max(v1,v2)); //highest vertex index entered user defines the number of vertices in the graph - 1 
			insertEdge(g,v1,v2,cost);
		}

		if(g->nVertices !=0 ) ++(g->nVertices) ; //update to be equal to say that this is number of vertices in graph. 

		printGraph(g);
//		cout<<endl<<vnum<< " " << density<< " " << nedges<<endl;

		//create graph using above input and call dijkstra.
		//for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraSimpleScheme(g,sourceVertex);
		for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraBinomialHeap(g,sourceVertex);

	}

	if(strcmp(argv[1], "-is") == 0) 
	{
		cout<<"simple scheme"<<endl;
		if(argc == 3)
		{
			//read input from the file 
			cout<<"reading input from file";
			readGraphFromFile(g,argv[2]);
		}
		else
		{
			readGraphFromUser(g);
			//get input from user 
		}
		//printGraph(g);
		//cout<<endl;
		//create graph using above input and call dijkstra.
		for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraSimpleScheme(g,sourceVertex);
		cout<<endl;
	}

	if(strcmp(argv[1], "-if") == 0) 
	{
		cout<<"F-heap scheme"<<endl;
		if(argc == 3)
		{
			//read input from the file 
			readGraphFromFile(g,argv[2]);
		}
		else
		{
			readGraphFromUser(g);
			//get input from user 
		}
		printGraph(g);
		cout<<endl<<endl;
		//create graph using above input and call dijkstra.
//		for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraFibonacciHeap(g,sourceVertex);

	}

	if(strcmp(argv[1], "-ib") == 0) 
	{
		cout<<"B-heap scheme"<<endl;
		if(argc == 3)
		{
			//read input from the file 
			readGraphFromFile(g,argv[2]);
		}
		else
		{
			readGraphFromUser(g);
			//get input from user 
		}
		//printGraph(g);
		//cout<<endl<<endl;
		//create graph using above input and call dijkstra.
		for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraBinomialHeap(g,sourceVertex);
		cout<<endl;
	}



	return 0;
}
