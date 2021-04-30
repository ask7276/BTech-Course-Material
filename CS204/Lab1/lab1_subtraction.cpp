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
	lli t;
	cin>>t;
	ask(i,0,t){
		vector <lli> first,second;
		string s1,s2;
		cin>>s1>>s2;
		for(lli i=0;i<s1.size();i++){
			
			first.pb(s1[i] -48);
			//o1(first[i]);
		}
		
		for(lli i=0;i<s2.size();i++){
			
			second.pb(s2[i]-48);
			//o1(second[i]);
		}


		lli a = first.size();
		lli b = second.size();
		lli big,small;
		if(a>b){
			big = a;
			small = b;
			for(lli i = small-1;i>=0;i--){
				if(first[(i+big-small)] < second[i]){
					for(lli j = i+big-small-1;j>=0;j--){
						if(first[j]!= 0){
							first[j]--;
							break;
						}
						else{
							first[j]=9;
						};
					};
					first[(i+big-small)] = 10 + first[(i+big-small)]-second[i];
				}
				else{
					first[(i+big-small)] -= second[i];
				};
			};

		}
		else if(b>a){
			big = b;
			small = a;
			first.swap(second);
			for(lli i = small-1;i>=0;i--){
				if(first[(i+big-small)] < second[i]){
					for(lli j = i+big-small-1;j>=0;j--){
						if(first[j]!= 0){
							first[j]--;
							break;
						}
						else{
						first[j]=9;
						};
					};
					first[(i+big-small)] = 10 + first[(i+big-small)]-second[i];
				}
				else{
					first[(i+big-small)] -= second[i];
				};
			};

		}
		else{
			big =a;
			ask(i,0,a){
				if(first[i]>second[i]){
					break;
				};
				if(second[i]>first[i]){
					first.swap(second);
					break;
				};
			};
			for(lli i = a-1;i>=0;i--){
				if(first[(i)] < second[i]){
					for(lli j = i-1;j>=0;j--){
						if(first[j]!= 0){
							first[j]--;
							break;
						}
						else{
						first[j]=9;
						};
					};
					first[i] = 10 + first[i]-second[i];
				}
				else{
					first[(i)] -= second[i];
				};
			};
		};
		ask(i,0,big){
			cout<<first[i];
		};
		
		cout<<"\n";
	}
	return 0;
	
}