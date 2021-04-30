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

int main(){
	lli n;
	cin>>n;
	lli q;
	cin>>q;
	lli a,b,c,d,e,f;
	vector<vector<lli>>v;
	vectorr<lli>u;
	vector<pair<lli,lli>>du;
	vector<lli>::iterator it;
	ask(i,0,q){
		cin>>a;
		v[i].pb(a);
		if(a==1){
			cin>>b>>c;
			u.pb(b);
			v[i].pb(b);
			v[i].pb(c);
		}


	}
	sort(u.begin(),u.end());
	du.pb(u[0]);
	ask(i,1,u.size()){
		if(u[i]!=u[(i-1)]){
			du.pb(mp(u[i],0));
		}
	}
	lli max=0,ans=0;
	ask(j,0,q){
		if(v[j][0]==1){
			
		}
	}

}