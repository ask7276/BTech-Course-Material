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

string subtract(string s1,string s2){
	vector <lli> first,second;
		for(lli i=0;i<s1.size();i++){
			
			first.pb(s1[i] -48);
			//o1(first[i]);
		}
		
		for(lli i=0;i<s2.size();i++){
			
			second.pb(s2[i]-48);
			//o1(second[i]);
		}


		lli a = first.size();
		lli b = second.size();
		lli big,small;
		if(a>b){
			big = a;
			small = b;
			for(lli i = small-1;i>=0;i--){
				if(first[(i+big-small)] < second[i]){
					for(lli j = i+big-small-1;j>=0;j--){
						if(first[j]!= 0){
							first[j]--;
							break;
						}
						else{
							first[j]=9;
						};
					};
					first[(i+big-small)] = 10 + first[(i+big-small)]-second[i];
				}
				else{
					first[(i+big-small)] -= second[i];
				};
			};

		}
		else if(b>a){
			big = b;
			small = a;
			first.swap(second);
			for(lli i = small-1;i>=0;i--){
				if(first[(i+big-small)] < second[i]){
					for(lli j = i+big-small-1;j>=0;j--){
						if(first[j]!= 0){
							first[j]--;
							break;
						}
						else{
						first[j]=9;
						};
					};
					first[(i+big-small)] = 10 + first[(i+big-small)]-second[i];
				}
				else{
					first[(i+big-small)] -= second[i];
				};
			};

		}
		else{
			big =a;
			ask(i,0,a){
				if(first[i]>second[i]){
					break;
				};
				if(second[i]>first[i]){
					first.swap(second);
					break;
				};
			};
			for(lli i = a-1;i>=0;i--){
				if(first[(i)] < second[i]){
					for(lli j = i-1;j>=0;j--){
						if(first[j]!= 0){
							first[j]--;
							break;
						}
						else{
						first[j]=9;
						};
					};
					first[i] = 10 + first[i]-second[i];
				}
				else{
					first[(i)] -= second[i];
				};
			};
		};
		string ans= "";
		ask(i,0,big){
			ans.pb(first[i]+'0');
		};
		
		return ans;
}


string multiply(string s1,string s2){
	vector <lli> first,second,ans;
		for(lli i=0;i<s1.size();i++){
			
			first.pb(s1[i] -48);
			//o1(first[i]);
		}
		
		for(lli i=0;i<s2.size();i++){
			
			second.pb(s2[i]-48);
			//o1(second[i]);
		}
		lli a = first.size();
		lli b = second.size();
		
		lli count=0,temp;
		for(lli i=0;i<a+b;i++){
			ans.pb(0);
		};
		for(lli i= a-1;i>=0;i--){
			for(lli j= b-1;j>=0;j--){
				ans[(j+a-count)] += second[j]*first[i];

			};
			count++;
		};
		lli carry =0;
		reverse(ans.begin(),ans.end());
		for(lli i=0;i<a+b;i++){
			if(ans[i]  >=10){
				ans[(i+1)] +=ans[i]/10;
				ans[i] %= 10;
			};
		};
		reverse(ans.begin(),ans.end());
		string result= "";
		ask(i,0,ans.size()){
			result.pb(ans[i]+'0');
		};
		return result;
}


bool com(string s1,string s2){
	if(s1.size()<s2.size()){
		return false;
	}
	if(s1.size()>s2.size()){
		return true;
	}

	ask(i,0,s1.size()){
		if(s1[i] < s2[i]){
			return false;
		}
		if(s1[i] > s2[i]){
			return true;
		}
	}
	return true;
}

string deletefirst(string s1){
	string s2 = s1;
	for(lli i=0;i<s1.size();i++){
		if(s2[0]=='0'){
			s2.erase(s2.begin());
		}
		else{
			break;
		}
	};
	return s2;
}

int main(){
	lli t;
	cin>>t;
	ask(i,0,t){
		string s1,s2,s3,s5;
		cin>>s1>>s2;
		int a = s1.size();
		int b= s2.size();
		int h;
		if(!com(s1,s2)){
			cout<<i+1<<".quotient is 0\nremainder is "<<s1<<"\n";
			continue;
		}

		if(a>b){
			ask(f,0,a-b){
				s2.pb(0+'0');
			};
		}

		vector <int> ans;
		ask(j,0,a-b+1){
			h=0;
			ask(k,1,10){
				string s4 = "";
				s4.pb(k+'0');
				s3= multiply(s2,s4);
				s3 = deletefirst(s3);
				if(com(s1,s3)){
					h=k;
					continue;
				}
				else{
					break;
				};
			};
			ans.pb(h);
			s5 = "";
			s5.pb(h+'0');
			s3 = multiply(s2,s5);
			s3 = deletefirst(s3);
			s1 = subtract(s1,s3);
			s1= deletefirst(s1);
			s2.popb();
		}
		cout<<i+1<<".";
		cout<<"quotient is ";
		ask(i,0,ans.size()){
		    cout<<ans[i];
		};
		cout<<"\nremainder is ";
		if(s1==""){
		    cout<<0<<"\n";
		}
		else{
		    s1 = deletefirst(s1);	
		    cout<<s1<<"\n";
		}

	}
	return 0;
}
