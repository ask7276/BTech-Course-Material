
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

lli a[1000000];
lli front=0;
lli rear=0;
void my_push(lli b, lli &front, lli &rear){
    if(front == (rear+1)%(sizeof(a)/sizeof(lli))){
        cout<<"full\n";
        return ;
    }
    a[rear] = b;
    rear = (rear+1)%(sizeof(a)/sizeof(lli));
    return ;
}

lli my_pop(lli &front, lli &rear){
    if(front == rear){
        cout<<"empty queue: error";
        exit(1);
    }
    lli s = a[front];
    front = (front + 1)%(sizeof(a)/sizeof(lli));
    return s;
}

lli my_front(lli &front,lli &rear){
    if(front == rear){
        exit(1);
    }
    lli s = a[front];
    return s;
}

lli my_back(lli &front,lli &rear){
    if(front == rear){
        exit(1);
    }
    lli s = a[(rear-1)%(sizeof(a)/sizeof(lli))];
    return s;
}

lli my_size(lli &front,lli &rear){
    return (rear-front)%(sizeof(a)/sizeof(lli));
}

bool my_empty(lli &front,lli &rear){
    if(front== rear){
        return true;
    }
    return false;
}

// void start(){
//     cout<<"1 push 2 pop 3 front 4 back 5 size 6 empty\n";
// }

int main(){
	anket;
    start();
	lli t;
    cin>>t;
    ask(i,0,t){
        lli a;
        cin>>a;
        if(a==1){
            lli b;
            cin>>b;
            my_push(b,front,rear);
            //o2(front,rear);
        }
        if(a==2){
            lli c;
            c=my_pop(front,rear);
            o1(c);
        }
        if(a==3){
            lli c = my_front(front,rear);o1(c);
        }
        if(a==4){
            lli c = my_back(front,rear);o1(c);
        }
        if(a==5){
            lli c = my_size(front,rear);o1(c);
        }
        if(a==6){
            bool c = my_empty(front,rear);o1(c);
        }

    }
	return 0;
}
