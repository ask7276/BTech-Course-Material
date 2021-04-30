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
set<lli> f;
void as(lli s, vector<lli> a[], bool visited[],lli flag,lli b[]){
	if(visited[s] == true && b[s] == flag){
		cout<<"NO";
		//cout<<s<<flag;
		exit(0);
	}
	if(visited[s] == true){
		return;
	}
	visited[s] = true;
	f.erase(s);
	b[s] = 3-flag;

	for(auto u : a[s]){
		as(u,a,visited,3-flag,b);

	}
	return ;
}


int main(){
	lli n,m;
	cin>>n>>m;
	vector<lli> a[n];
	lli c,d;
	
	bool visited[n]= {false};
	
	ask(i,0,n){
		f.insert(i);
	}
	ask(i,0,m){
		cin>>c>>d;
		a[c-1].pb(d-1);
		a[d-1].pb(c-1);
	}
	lli flag=1;
	lli b[n] = {0};
	//visited[0];
	//b[0] = 0;
	while(!f.empty()){
		lli g= *f.begin();
		//o1(g);
		if(a[g].size()==0){
			f.erase(g);
			continue;
		}
		as(g,a,visited,flag,b);
	}
	

	cout<<"YES";
	return 0;
}