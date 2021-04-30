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
	anket;
	lli n;
	cin>>n;

	lli q;
	cin>>q;
	
	vector<pair<lli,lli>>a;
	vector<lli>f,g;
	vector<lli>::iterator it1;
	vector<pair<lli,lli>>::iterator it2;
	lli b,c,d,e;
	lli max=0,ans=0;
	ask(i,0,q){
		cin>>b;
		if(b==1){
			cin>>c>>d;
			if(f.size()==0){
				f.pb(c);
				a.pb(mp(c,d));
				continue;
			}
			
			lli flag=0;
			ask(j,0,f.size()){
				if(f[j]==c){
					a[j].S += d;
					if(a[j].S>max){
						ans = a[j].F;
						max = a[j].S;
						flag=1;
						break;
					}
				}
			}
			if(flag==0){
				f.pb(c);
				a.pb(mp(c,d));
				if(d>max){
					ans =c;
				}
			}
			
		}
		else{
			
			if(a.size()==0){
				g.pb(-1);
				continue;
			}
			
			g.pb(ans);
		}
	};
	ask(i,0,g.size()){
		if(g[i]==-1){
			cout<<"No data available.\n";
		}
		else{
			cout<<g[i]<<"\n";
		}
	}

	return 0;
}