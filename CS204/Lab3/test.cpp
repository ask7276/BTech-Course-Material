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
	#define z 1000
	char a[z];
	cin>>t;
	lli e[t];
	ask(i,0,t){
		string s;
		cin>>s;
		lli c=-1,flag=0;
		lli b = s.size();
		ask(j,0,b){
			if(s[j] == '('){
				c++;
				a[c] = '(';
			}
			else if(s[j] == '{'){
				c++;
				a[c] = '{';
			}
			else if(s[j] == '['){
				c++;
				a[c] = '[';
			}
			else if(s[j] == '<'){
				c++;
				a[c] = '<';
			}
			else if(s[j] == ')'){
				if(a[c]== '('){
					c--;
				}
				else{
					flag=1;
					break;
				}
			}
			else if(s[j] == ']'){
				if(a[c]== '['){
					c--;
				}
				else{
					flag=1;
					break;
				}
			}
			else if(s[j] == '}'){
				if(a[c]== '{'){
					c--;
				}
				else{
					flag=1;
					break;
				}
			}
			else if(s[j] == '>'){
				if(a[c]== '<'){
					c--;
				}
				else{
					flag=1;
					break;
				}
			}
			else if(s[j]=='|'){
				if(a[c]=='|'){
					c--;
				}
				else{
					c++;
					a[c]='|';
				};
			}
			else{
				flag=1;
				break;
			}
			
		}
		if((flag==1) || (c!=-1)){
			e[i]= 0;
			flag=0;
			continue;
		}
		else{
			e[i]=1;
		};
		
	}
	ask(i,0,t){
		if(e[i]==0){
			cout<<"NO\n";
		}
		else{
			cout<<"YES\n";
		};
	};
	return 0;
}
