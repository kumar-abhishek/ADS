
void dijkstraSimpleScheme(graph *g,int sourceVertex)
{
	intializeSingleSource(g,sourceVertex);
	bool visited[g->nVertices];
	for(int i =0;i<g->nVertices;i++) { d[i] = INF; visited[i] = false;} //intial estimate of source to all other vertices and no vertices have been visited yet.
	d[sourceVertex] = 0; //distance of source vertex to source vertex is zero
	int x = sourceVertex;
	for(int k =0;k<g->nVertices;k++)
	{
		int minIndex= g->nVertices, minWeight=INF;
		for(int i=0;i<g->nVertices ;i++) {if(visited[i]==true) continue;if(minWeight > d[i]) {minWeight=d[i]; minIndex=i;};} //doing extractMin }
		struct edgeNode *v = g->edges[minIndex] , *temp;temp = v;
		while(v)
		{
			relax(minIndex, v->endPoint, v->weight);
			v=v->next;
		}
		visited[minIndex] = true;
	}
		//cout<<endl<< "d array is ";
		cout<<endl;
		for(int j =0;j<g->nVertices;j++) cout<<  d[j]<<" ";
}

