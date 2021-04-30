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

#include <bits/stdc++.h>
using namespace std;

struct Node{
    int key;
    Node* left;
    Node* right;
};

Node *newNode(int key){
        Node *a = ( Node*)malloc(sizeof( Node));
        a->key=key;
        a->left=NULL;
        a->right=NULL;
        return a;
    }

Node *InsertR( Node *root, int key){
    	if(root == NULL) return newNode(key);
    	root->right =  InsertR(root->right, key);
    	return root;
    }

Node *InsertL( Node *root, int key){
    	if(root == NULL) return newNode(key);
    	root->left =  InsertL(root->left, key);
    	return root;
    }

void inorder( Node* root)
    {
        if (!root) return;
        inorder(root->left);
        cout<<root->key<<" ";
        inorder(root->right);
    }
    
void preorder( Node* root) {
        if(root == NULL) return;
        cout << root->key << " ";
        preorder(root->left);
        preorder(root->right);
    }

void postorder( Node* root) {
        if(root == NULL) return;
        postorder(root->left);
        postorder(root->right);
        cout << root->key << " ";
    }

bool Search( Node* root, int key)
    {
        if (root == NULL)
            return false;

        if (root->key == key)
            return true;

        bool l = Search(root->left, key);

        bool r = Search(root->right, key);

        return l || r;
    }


int main(){
  Node *root = NULL;
	root = InsertL(root, 80);
	root = InsertR(root, 54);
	root = InsertL(root, 23);
	root = InsertL(root, 43);
	root = InsertR(root, 5);
	inorder(root);
	cout<<endl;
	preorder(root);
	cout<<endl;
    postorder(root);
	cout<<endl;
	cout<<Search(root, 80)<<"\n";
	cout<<Search(root,70)<<"\n";
}