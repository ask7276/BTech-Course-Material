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
#define mod 1000000007

int minKey(int key[], bool mstSet[],int &V){ 
	int min = INT_MAX, min_index; 
	for (int v = 0; v < V; v++) {
		if (mstSet[v] == false && key[v] < min){
			min = key[v], min_index = v; 
		}
	}
	return min_index; 
} 

void printMST(int parent[], int **graph, int &V){ 
	cout<<"Edge \tWeight\n"; 
	for (int i = 1; i < V; i++) 
		cout<<parent[i]+1<<" - "<<i+1<<" \t"<<graph[i][parent[i]]<<" \n"; 
} 

void primMST(int **graph, int &V){ 
	int parent[V]; 
	int key[V]; 
	bool mstSet[V]; 
	for (int i = 0; i < V; i++) 
		key[i] = INT_MAX, mstSet[i] = false; 
	key[0] = 0; 
	parent[0] = -1; 
	for (int count = 0; count < V - 1; count++){
		int u = minKey(key, mstSet,V);
		mstSet[u] = true;
		for (int v = 0; v < V; v++)
			if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]) 
				parent[v] = u, key[v] = graph[u][v]; 
	}
	printMST(parent, graph,V); 
} 

int main(){
	int V;
	cin>>V;
    int **g=new int*[V];
    for(int i=0;i<V;i++)
    g[i]=new int[V]; 
	int E;
	cin>>E;
	lli a,b;
	ask(i,0,V){
		ask(j,0,V){
			g[i][j]= 0;
		}
	}
	lli w;
	ask(i,0,E){
		cin>>a>>b>>w;
		g[a-1][b-1] = w;
		g[b-1][a-1] = w;
	}

	primMST(g,V); 

	return 0; 
} 


