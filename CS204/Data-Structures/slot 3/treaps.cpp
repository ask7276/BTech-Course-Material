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

struct cnode{
    lli value;
    lli p;
    cnode* leftp;
    cnode* rightp;
};

cnode* leftprotate(cnode* y){
    cnode* x= y->rightp;
    cnode* t2= x->leftp;
    x->leftp= y;
    y->rightp= t2;
    return x; 
}

cnode* rightprotate(cnode* x){
    cnode* y= x->leftp;
    cnode* t2= y->rightp;
    y->rightp= x;
    x->leftp= t2;
    return y;
}

cnode* newnode(lli x){
    cnode* temp= new cnode;
    temp->value= x;
    temp->p= rand()%100;
    temp->leftp=NULL;
    temp->rightp= NULL;
    return temp;
}

cnode* search(cnode* root, lli value){
    if(root==NULL || root->value==value){
        return root;
    }

    if(root->value>value){
        return search(root->leftp, value);
    }
    if(root->value<value){
        return search(root->rightp, value);
    }
}

cnode* insert(cnode* root, lli value){
    if(root==NULL){
        return newnode(value);
    }

    if(root->value>=value){
        root->leftp= insert(root->leftp, value);
        if(root->leftp->p>root->p){
            root= rightprotate(root);
        }
    }
    else if(root->value<value){
        root->rightp= insert(root->rightp, value);
        if(root->rightp->p>root->p){
            root= leftprotate(root);
        }
    }
    return root;
}

cnode* deletenode(cnode* root, lli value){
    if(root==NULL){
        return root;
    }
    //If value is not equal to root's value, we check in its rightp or leftp subtree//
    if(root->value>value){
        root->leftp= deletenode(root->leftp, value);
    }
    else if(root->value<value){
        root->rightp= deletenode(root->rightp, value);
    }
    else if(root->value==value){
        if(root->leftp==NULL){
            cnode* temp= root->rightp;
            delete(root);
            root= temp;
        }
        else if(root->rightp==NULL){
            cnode* temp= root->leftp;
            delete(root);
            root= temp;
        }
        else if(root->leftp!=NULL && root->rightp!=NULL){
            if(root->leftp->p>root->rightp->p){
                root= rightprotate(root);
                root->rightp= deletenode(root->rightp, value);
            }
            else if(root->rightp->p>root->leftp->p){
                root= leftprotate(root);
                root->leftp= deletenode(root->leftp, value);
            }
        }
    }
    return root;
}

void inorder(cnode* root) { 
    if (root!=NULL){ 
        inorder(root->leftp); 
        cout << "value: "<< root->value << " | Priority: "
            << root->p; 
        if (root->leftp) 
            cout << " | leftp child: " << root->leftp->value; 
        if (root->rightp) 
            cout << " | rightp child: " << root->rightp->value; 
        cout << endl; 
        inorder(root->rightp); 
    } 
} 
int main(){
    struct cnode *root = NULL; 
    lli n;
    cin>>n;
    string s1 = "not Found!!!",s2= "found!!!";
    ask(i,0,n){
        string s;
        lli t;
        cin>>s;
        if(s=="insert"){
            cin>>t;
            root = insert(root,t);
        }
        else if(s == "inorder"){
            inorder(root);
        }
        else if(s=="delete"){
            cin>>t;
            deletenode(root,t);
        }
        else if(s=="search"){
            cin>>t;
            cnode *res = search(root, t); 
            if(res==NULL){
            cout<< s1<<endl;
            }
            else{
            cout<< s2 <<endl;
            }
        }
    }
    inorder(root);
    return 0; 
}