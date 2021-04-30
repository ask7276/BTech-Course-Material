#include<bits/stdc++.h>
 
typedef long long int lli;
typedef unsigned long long int ulli;
typedef long double ldb;
 
#define pb push_back
#define popb pop_back
#define si size()
#define sof sizeof()
#define be begin()
#define en end()
#define all(v) v.be,v.en
#define le length()
#define mp make_pair 
#define mt make_tuple
#define F first
#define S second
#define ask(i,m,n) for(int i=m;i<n;i++)
 
#define deci(n)  fixed<<setprecision(n)
#define high(n) __builtin_popcount(n)
#define parity(n) __builtin_parity(n)
#define ctz(n)  __builtin_ctz(n)
#define maxe *max_element
#define mine *min_element
#define anket ios::sync_with_stdio(0), cin.tie(0),cout.tie(0)
 
#define endl "\n"
#define o0(a) cout<<a<<" "
#define o1(a) cout<<a<<"\n"
#define o2(a,b) cout<<a<<" "<<b<<"\n"
#define o3(a,b,c) cout<<a<<" "<<b<<" "<<c<<"\n"
#define o4(a,b,c,d) cout<<a<<" "<<b<<" "<<c<<" "<<d<<"\n"
using namespace std;
//stores the endpoints of edge and w
class Edge { 
	public: 
	int a, b, w; 
	
}; 
//contain number of vertices,edges and edge list
class Graph { 
	public: 
	int V,E; 
	Edge* edge; 
}; 
Graph* createGraph(int V, int E){ 
	Graph* graph = new Graph; 
	graph->V = V; 
	graph->E = E; 
	graph->edge = new Edge[E]; 
	return graph; 
} 
//following code is used to find the cycle present if any in the graph in log time
class pr{ 
	public: 
	int parent; 
	int rank; 
}; 
int find(pr subs[], int i){ 
	if (subs[i].parent != i) 
		subs[i].parent = find(subs, subs[i].parent); 
	return subs[i].parent; 
} 
void Union(pr subs[], int x, int y){ 
	int xroot = find(subs, x); 
	int yroot = find(subs, y); 
	if (subs[xroot].rank < subs[yroot].rank){
		subs[xroot].parent = yroot; 
	}
	else if (subs[xroot].rank > subs[yroot].rank){
		subs[yroot].parent = xroot;
	}
	else{ 
		subs[yroot].parent = xroot; 
		subs[xroot].rank++; 
	} 
} 
//used as a comparison function for the sorting of the graph
int myComp(const void* a, const void* b){ 
	Edge* a1 = (Edge*)a; 
	Edge* b1 = (Edge*)b; 
	return a1->w > b1->w; 
} 
//main running algo
void KruskalMST(Graph* graph){ 
	int V = graph->V; 
	Edge result[V]; 
	int e = 0; 
	int i = 0; 

	qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp); 
	pr *subs = new pr[( V * sizeof(pr) )]; 
	 
	ask(v,0,V){
		subs[v].parent = v; 
		subs[v].rank = 0; 
	}
	while (e < V - 1 && i < graph->E){ 
		Edge next_edge = graph->edge[i++]; 
		int x = find(subs, next_edge.a); 
		int y = find(subs, next_edge.b); 
		if (x != y){ 
			result[e++] = next_edge; 
			Union(subs, x, y); 
		} 
	} 
	cout<<"Following are the edges in the constructed MST\n"; 
	for (i = 0; i < e; ++i) {
		cout<<result[i].a<<" - "<<result[i].b<<"\n"; 
	}
	return; 
} 

int main(){
	anket; 
	int V,E;
	cin>>V>>E;
	Graph* graph = createGraph(V, E); 
	ask(i,0,E){
		cin>>graph->edge[i].a>>graph->edge[i].b>>graph->edge[i].w;
	}
	KruskalMST(graph); 
	return 0; 
} 


