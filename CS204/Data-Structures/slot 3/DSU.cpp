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

int main(){
	anket; 
	int V,E;
	cin>>V>>E;
	pr *subs = new pr[( V * sizeof(pr) )]; 
	 
	ask(v,0,V){
		subs[v].parent = v; 
		subs[v].rank = 0; 
	}
	vector<pair<lli,lli>> e;
	lli a,b;
	ask(i,0,E){
		cin>>a>>b;
		e.pb(mp(a,b));
	};
	ask(i,0,E){
		a = e[i].F;
		b = e[i].S;
		if(find(subs,a-1) == find(subs,b-1)){
			cout<<"graph is cyclic\n";
			return 0;
		}
		else{
			Union(subs,a-1,b-1);
		};
	};
	cout<<"graph is acyclic\n";
	return 0; 
} 
