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

void dfs(lli z ,vector<lli> v[],bool s[],lli u[],lli t) {
	if(s[z]==true){
		return;
	}
	u[z] = t;
    s[z] = true;
    for (lli r : v[z]) {
        if (!s[r])
            dfs(r,v,s,u,t);
    }
}

int main(){
	lli n,m,k;
	cin>>n>>m>>k;
	set<pair<lli,lli>> a;
	set<pair<lli,lli>>::iterator it3;
	map<pair<lli,lli>,lli> b;
	map<pair<lli,lli>,lli>::iterator it1,it2;
	lli c,d;
	if(k==0){
		cout<<0;
		return 0;
	}
	ask(i,0,k){
		cin>>c>>d;
		b[{c-1,d-1}] = i;
		a.insert({c-1,d-1});
	};
	vector<lli > v[k];
	lli x,y;
	//cout<<"success\n";
	while(!a.empty()){
		//{c,d} = *(a.begin());
		it3 = a.begin();
		c = (*it3).first;
		d = (*it3).second;

		if(a.find({c+1,d})!=a.end()){
			it1 = b.find({c,d});
			it2 = b.find({c+1,d});
			x = it1->second;
			y = it2->second;
			v[x].pb(y);
			v[y].pb(x);
		}
		if(a.find({c-1,d})!=a.end()){
			it1 = b.find({c,d});
			it2 = b.find({c-1,d});
			x = it1->second;
			y = it2->second;
			v[x].pb(y);
			v[y].pb(x);
		}
		if(a.find({c,d+1})!=a.end()){
			it1 = b.find({c,d});
			it2 = b.find({c,d+1});
			x = it1->second;
			y = it2->second;
			v[x].pb(y);
			v[y].pb(x);
		}
		if(a.find({c,d-1})!=a.end()){
			it1 = b.find({c,d});
			it2 = b.find({c,d-1});
			x = it1->second;
			y = it2->second;
			v[x].pb(y);
			v[y].pb(x);
		}
		a.erase({c,d});
	}
	bool s[k] ;
	lli u[k];
	ask(i,0,k){
		s[i]= false;
	}
	lli t=0;
	ask(i,0,k){
		if(s[i]==false){
			dfs(i,v,s,u,t);
			t++;
		}
	}
	lli o[k]={0};
	ask(i,0,k){
		o[u[i]]++;
	}
	sort(o,o+k);
	cout<<o[k-1];
	return 0;
}