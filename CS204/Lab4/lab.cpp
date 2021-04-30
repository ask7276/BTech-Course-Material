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



struct treenode 
{ 
    string value; 
    treenode* left, *right; 
}; 

bool isOp(string c) 
{ 
    if (c == "+" || c == "-" || 
            c == "*" || c == "/" || 
            c == "^") 
        return true; 
    return false; 
} 

treenode* newNode(string v) 
{ 
    treenode *temp = new treenode; 
    temp->left = temp->right = NULL; 
    temp->value = v; 
    return temp; 
}; 

treenode* create(vector<string >c) 
{ 
    stack<treenode *> st; 
    treenode *t, *t1, *t2; 
  	
    for (int i=0; i<c.size(); i++) 
    { 
        
        if (!isOp(c[i])) 
        { 
            t = newNode(c[i]); 
            st.push(t); 
        } 
        else 
        { 
            t = newNode(c[i]); 
  
            
            t1 = st.top();
            st.pop();       
            t2 = st.top(); 
            st.pop(); 
            t->right = t1; 
            t->left = t2; 
            st.push(t); 
        } 
    } 
    t = st.top(); 
    st.pop(); 
  
    return t; 
} 

lli solve(treenode *r){
	if((r->value!= "+") && (r->value != "-")  && (r->value != "*") && (r->value != "/") && (r->value != "^")){
		string t= r->value;
		lli c = (lli)stoi(t);
		return c;
	}
	string t = r->value;
	lli a= solve(r->left);
	lli b = solve(r->right);
	if(t=="+"){
		return a+b;
	}
	if(t=="-"){
		return a-b;
	}
	if(t=="*"){
		return a*b;
	}
	if(t=="/"){
		return a/b;
	}
	if(t=="^"){
		return (lli)pow(a,b);
	}
}



int main(){
    lli t;
    vector<lli> ak;
    cin>>t;
    ask(i,0,t){
        lli n;
        cin>>n;
        ask(j,0,n){
            
            stack <char >a;
            vector <string >b;
            string s;
            cin>>s;
            lli c=0,flag=0;
            char d;
            
            ask(k,0,s.size()){
            	 c=0;
            	 flag=0;
            	
                if((s[k]<58) && (s[k]>47)){
                	ask(l,k,s.size()){
                		if(s[l]<58 && s[l]>47){
                			c = 10*c + s[l]-48;
                			k=l;
                		}
                		else break;
                	}
                	
                	
  					ostringstream str1;
  					str1<<c;
  					string st = str1.str();
  					b.pb(st);
  					continue;
                }
                else{
                	if(s[k] == '+' || s[k]=='-'){
                		
                        if(s[(k-1)]=='(' && s[k]=='-'){
                            b.pb("0");
                        }
                        if(s[(k-1)]=='(' && s[k]=='+'){
                            b.pb("0");
                        }

                		for(;!a.empty();){
                		    d = a.top();
                			if(d == '*' || d == '/' || d== '^' ||d == '+' || d == '-'){
                				//cout<<d<<" ";
                				string st2(1,d);
                				b.pb(st2);

                				a.pop();
                				
                			}
                			else{
                			    break;
                			}
                		}
                		a.push(s[k]);
                		continue;
                	}
                	if(s[k]=='*' || s[k]=='/'){
                		
                		for(;!a.empty();){
                		    d = a.top();
                		    if( d== '^'|| d=='*' || d== '/'){
               				//cout<<d<<" ";
                			    string st3(1,d);
                    			b.pb(st3);

                    			a.pop();
                    			
                    		}
                    		else{
                    			break;
                    		}
                		}
                    	a.push(s[k]);
                	   	continue;
                	}
                	if(s[k]=='^'){
                		a.push(s[k]);
                		continue;
                	}
                    if(s[k]=='('){
                        a.push(s[k]);
                    }
                    if(s[k]==')'){
                        for(;a.top()!='(';){
                            string st5(1,a.top());
                            b.pb(st5);
                            a.pop();
                        }
                        a.pop();
                    }
                }

            }
            for(;!a.empty();){
            	//cout<<a.top()<<" ";
            	d = a.top();
            	string st4(1,d);
            	b.pb(st4);
            	a.pop();
            };
            treenode *r = create(b);
            lli y = solve(r);
            ak.pb(y);
        }
    } 
    ask(i,0,ak.size()){
    	cout<<ak[i]<<"\n";
    }  
    return 0;
}

