#include<iostream>
#include<cstring>
#include<fstream>
#include "graph.h"
#include "dijkstraSimpleScheme.h"
#include "fibonacciHeap.h"
#include "binomialHeap.h"
#include<ctime>
#include<cstdlib>
#include<iomanip>
#include<set> //TODO:REMOVE THIS STATEMENT BEFORE SUBMISSION
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
/*
		if(argv[2]) vnum = atoi(argv[2]);//get number of vertices
		if(vnum == 0 ) vnum = 500; //user didnt specify the number of vertices
		
		if(argv[3]) density = atoi(argv[3]);//get density of graph
		if(density == 0 ) density = 100; //user didnt specify density, so assume 50%
*/

//uncomment for the final submission:ucomment begins here

		cout<<setw(25)<<"Number of Vertices"<<setw(10)<<"Density" <<setw(25)<<"Simple Scheme(ms)"<<setw(25)<<"B-Heap Scheme(ms)"<<setw(25)<<"F-Heap Scheme(ms)"<<endl;
		for(vnum = 100; vnum<=500; vnum += 100)
		{
			for(density = 10; density <=100; density += 10)
			{

				nedges = (density*(vnum)*(vnum-1))/100 + 1;   // calculate number of edges needed to generate for the specified density		

				initializeGraph(g);

				//for writing graph to a file.
				//TODO:comment this later
				ofstream outfile("randOutput.txt",ofstream::binary);
				

				//generate random graph 	
				srand(time(NULL));//initialize random seed
				set<pair<pair<int, int >, int>  > s;
			//	for(int i=0;i<nedges;i++)
				while(s.size() < nedges)
				{
					v1 = randGenerator(vnum);
					v2 = randGenerator(vnum);
					cost = randGenerator(1000) + 1;
					if(v1 == v2) continue;	
					s.insert(make_pair(make_pair(v1,v2),cost));
				
					char buffer[10];
					sprintf(buffer,"%d %d %d\n",v1,v2,cost);
					outfile.write(buffer,10);
				}
				outfile.close();//TODO:delete this later

				set<pair<pair<int,int>,int> >::iterator it;
				for(it=s.begin();it != s.end(); it++)
				{
					//cout<<it->first.first<< "  " <<it->first.second<<" "<<it->second<<endl;
					++(g->nEdges); //increment number of edges in graph
					g->nVertices = max(g->nVertices,max(v1,v2)); //highest vertex index entered user defines the number of vertices in the graph - 1
					insertEdge(g,v1,v2,cost);
				}

				if(g->nVertices !=0 ) ++(g->nVertices) ; //update to be equal to say that this is number of vertices in graph. 
				
				g->nVertices = vnum;//TODO:check if this line is needed 


				//create graph using above input and call dijkstra.

				clock_t Start,Finish;double TimeSimple=0, TimeBHeap=0, TimeFHeap=0;

				for(int i=0;i<5;i++)
				{
					//simple Scheme
					Start  = clock();
					for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraSimpleScheme(g,sourceVertex);
					TimeSimple += ((double)(clock() - Start)) ;
				}	
				TimeSimple /= (10*1000); //avg time in ms 

				for(int i=0;i<5;i++)
				{
					//Binomial Heap Scheme
					Start  = clock();
					for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraBinomialHeap(g,sourceVertex);
					TimeBHeap += ((double)(clock() - Start));
				}	
				TimeBHeap /= (10*1000); //avg time in ms 



				for(int i=0;i<5;i++)
				{
					//Finonacci Heap Scheme
					Start  = clock();
					for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraFibonacciHeap(g,sourceVertex);
					TimeFHeap += ((double)(clock() - Start));
				}	
				TimeFHeap /= (10*1000); //avg time in ms 


				//				dijkstraFibonacciHeap(g,0);
				cout<<setw(25)<<vnum<<setw(10)<<density <<setw(25)<<double(TimeSimple) <<setw(25)<<TimeBHeap<<setw(25)<<TimeFHeap<<endl;


			}
		}

//uncomment:ends here
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
		//for(int sourceVertex = 0;sourceVertex < g->nVertices;sourceVertex++) 	dijkstraFibonacciHeap(g,sourceVertex);
		dijkstraFibonacciHeap(g,0);
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
