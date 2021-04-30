
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

lli n,m, s; 
vector<vector<lli>> adj;  
queue<lli> q;
vector<bool> visited;
vector<lli> d, p;
//dn for distance from s and pn for storing parent 

void my_bfs(lli s){
    q.push(s-1);
    visited[s-1] = true;
    p[s-1] = -1;
    while (!q.empty()) {
        lli v = q.front();
        q.pop();
        o0(v+1);
        for (lli u : adj[v]) {
            if (!visited[u]) {
                visited[u] = true;
                q.push(u);
                d[u] = d[v] + 1;
                p[u] = v;
            }
        }
    }
    
}

int main(){
	anket;
    cin>>n>>m;
    ask(i,0,n){
        vector<lli> ele;
        adj.pb(ele);
    };
    visited.resize(n);
    p.resize(n);
    d.resize(n);
    lli a,b;
    ask(i,0,m){
        cin>>a>>b;
        adj[a-1].pb(b-1);
        adj[b-1].pb(a-1);
    }
    lli e;
    cin>>e;
    my_bfs(e);
	return 0;
}