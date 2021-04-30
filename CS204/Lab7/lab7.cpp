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
	ask(i,0,n){
		lli q;
		lli max=0;
		cin>>q;
		string s;
		vector<string> t;
		vector<tuple<string,char,string>>a;
		ask(j,0,q){
			cin>>s;
			if(s.size()>max){
				max = s.size();
			};
			t.pb(s);
		};
		
		ask(w,0,q){
			s =t[w];
			string f = "";
			lli b = max/s.size();
			ask(j,0,b+1){
				f.append(s);
			};
			s = f.substr(0,max);
			a.pb(mt(s,t[w][(t[w].size()-1)],t[w]));
		}
		sort(a.begin(),a.end());
		for(lli w=q-1;w>=0;w--){
		    cout<<get<2>(a[w]);
		    
		}
		cout<<"\n";
	}
	return 0;
}
