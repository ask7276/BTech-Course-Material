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

lli division(lli a[], lli x, lli y, lli z) { 
    lli i; 
    for (i=x; i<y; i++){
        if (a[i] == z){
            break; 
        }
    }
    swap(a[i], a[y]); 
    i = x; 
    ask(j,x,y){
        if(a[j]<=z){
            swap(a[i],a[j]);
            i++;
        }
    }
    swap(a[i], a[y]); 
    return i; 
} 

lli findMedian(lli a[], lli n) 
{ 
    sort(a, a+n); 
    return a[n/2]; 
} 

lli median(lli a[], lli l, lli r, lli k) 
{ 
    
    if (k > 0 && k <= r - l + 1) 
    { 
        lli n = r-l+1; 
        lli i;
        lli med[(n+4)/5];
        for (i=0; i<n/5; i++) 
            med[i] = findMedian(a+l+i*5, 5); 
        if (i*5 < n){ 
            med[i] = findMedian(a+l+i*5, n%5); 
            i++; 
        }    
        lli mom = (i == 1)? med[i-1]: median(med, 0, i-1, i/2); 

        lli pos = division(a, l, r, mom); 

        if (pos-l == k-1) 
            return a[pos]; 
        if (pos-l > k-1) 
            return median(a, l, pos-1, k); 
        return median(a, pos+1, r, k-pos+l-1); 
    } 
    return 0; 
} 

int main() { 
    lli q;
    cin>>q;
    ask(i,0,q){
        lli n;
        cin>>n;
        lli a[n];
        lli b,c,d;
        ask(j,0,n){
            cin>>b>>c;
            d  = b*b + c*c;
            a[j]=d;
        };
        lli k;
        
        if(n%2==0){
            k = n/2 ;
        }
        else{
            k = (n+1)/2;
        }
        cout<<sqrt(median(a,0,n-1,k))<<"\n";
    }
    return 0; 
}
