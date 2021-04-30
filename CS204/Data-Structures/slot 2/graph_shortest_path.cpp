
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

#define inf 1e9

lli n,m;

int main(){
	anket;
    cin>>n>>m;
    lli graph[n][n];
    ask(i,0,n){
        ask(j,0,m){
            if(i!= j){
                graph[i][j] = inf;
            }
            else{
                graph[i][j]=0;
            }
        }
    }
    lli a,b,c;
    ask(i,0,m){
        cin>>a>>b>>c;
        graph[a-1][b-1] = c;
        graph[b-1][a-1]=c;
    };
    
    lli dist[n][n]; 
    ask(i,0,n){
        ask(j,0,n){
            dist[i][j] = graph[i][j];
        }
    }
    ask(k,0,n){
        ask(i,0,n){
            ask(j,0,n){
                dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
            }
        }
    }
    cout<<"The following matrix shows the shortest distances between every pair of vertices \n"; 
    ask(i,0,n){
        ask(j,0,n){
            if(dist[i][j]== inf){
                cout<<"INF ";
                continue;
            }
            cout<<dist[i][j]<<"   ";
        }
        cout<<"\n";
    } 
	return 0;
}