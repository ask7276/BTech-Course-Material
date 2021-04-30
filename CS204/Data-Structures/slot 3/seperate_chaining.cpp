 
//Question: Take input no. of lists you want in hash table. Then take input number of insertions you want to make, and hence the numbers
//you want to insert. Then, take input number of deletions you want to do and hence the numbers you want to delete. Print the whole hash
//table after insertion and after deletion.

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
#define mod 1000000007

const lli N= 4e6;

struct bucket{
    list <lli> a;
};

struct bucket table[N];
lli buckets;

lli hashfunction(lli x){
    return x%buckets;
}

void insert(lli x){
    lli index= hashfunction(x);
    table[index].a.pb(x);
}

void deleteitem(lli x){
    lli index= hashfunction(x);
    lli f=0;
    for(auto i: table[index].a){
        if(i==x){
            f=1;
            break;
        }
    }
    if(f){
        table[index].a.remove(x);
    }
}

void display_hashtable(){
    for(lli i=0; i<buckets; i++){
        cout<<"Bucket " <<i <<" :";
        for(auto sa: table[i].a){
            cout<< sa <<"| ";
        }
        cout<<endl;
    }
}


int main(){
    cin>>buckets;
    lli n;
    cin>>n;
    for(lli i=0; i<n; i++){
        lli x;
        cin>> x;
        insert(x);
    }
    cout<<"After insertion: " <<endl;
    display_hashtable();
    lli h;
    cin>> h;
    while(h--){
        lli x;
        cin>>x;
        deleteitem(x);
    }
    cout<< "After deletion: " <<endl;
    display_hashtable();
}