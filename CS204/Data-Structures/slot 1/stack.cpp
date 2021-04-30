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

struct Node { 
   int data; 
   struct Node *next; 
}; 
struct Node* top = NULL; 
void push(int val) {
   struct Node* newnode = (struct Node*) malloc(sizeof(struct Node)); 
   newnode->data = val; 
   newnode->next = top; 
   top = newnode; 
}
void pop() {
   if(top==NULL)
      cout<<"Stack Underflow"<<endl;
   else {
      Node *t=top;
      cout<<"The popped element is "<< top->data <<endl;
      top = top->next;
      delete t;
   }
}
void display() {
   struct Node* ptr;
   if(top==NULL)
      cout<<"stack is empty";
   else {   
      ptr = top; 
      cout<<"Stack elements are: ";
      while (ptr != NULL) { 
         cout<< ptr->data <<" "; 
         ptr = ptr->next; 
      } 
   }
   cout<<endl;
}
int main() {
     lli n;
     cin>>n;
     ask(i,0,n){
      string s;
      cin>>s;
      if(s=="push"){
        lli t;
        cin>>t;
        push(t);
      }
      if(s=="pop"){
        pop();
      }
      if(s=="display"){
        display();
      }
     }
      return 0;
} 
