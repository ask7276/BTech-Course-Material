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

double slope(int x,int y,int z,int w){
    if(x ==z){
        return 1000000000000000000;
    }
    return ((double)(w-y)/(z-x));
}

int orientation(lli a,lli b,lli c,lli d,lli e,lli f) {
    int val = (d -b) * (e - c) - 
              (c - a) * (f - d); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 

int main(){
    lli t;
    cin>>t;
    lli a,b;
    vector<pair<lli,lli>>c;
    lli x,y;
    ask(i,0,t){
        cin>>a>>b;
        c.pb(mp(a,b));
    }
    sort(c.begin(),c.end());
    x = c[0].F;
    y = c[0].S;
    vector<tuple<double,lli,lli>> d;
    ask(i,1,t){
        d.pb(mt(slope(x,y,c[i].F,c[i].S),c[i].F,c[i].S));
    };
    sort(d.begin(),d.end());
    stack<pair<lli,lli>> e;
    e.push(mp(x,y));
    e.push(mp(get<1>(d[0]),get<2>(d[0])));
    lli q,w,r,z,p,s;
    q = x;
    w = y;
    r = get<1>(d[0]);
    z = get<2>(d[0]);
    ask(i,1,t-1){
        p = get<1>(d[i]);
        s = get<2>(d[i]);
        if(orientation(q,w,r,z,p,s) == 2 ){
            e.push(mp(p,s));
            q = r;
            w = z;
            r= p;
            z= s;
        }
        else{
            while(orientation(q,w,r,z,p,s) != 2 && e.size()!= 0){
                e.pop(); 
                r= q;
                z = w;
                q = e.top().F;
                w = e.top().S;
            }
            e.push(mp(r,z));
            e.push(mp(p,s));
            q = r;
            w = z;
            r=p;
            z=s;
        }
    }
    o1(e.size());
    while(!e.empty()){
        lli g,h;
        g = e.top().F;
        h= e.top().S;
        cout<<g<<" "<<h<<"\n";
        e.pop();
    }
    return 0;
}
