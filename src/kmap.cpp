#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<set>
#include"kmap.h"
using namespace std;

/*
*	abbreviations
*	PI = Prime Implicants
*	EPI = Essential Prime Implicants
*	NPI = Non Essential Prime Implicants
*	PI = EPI Union  NPI
*/

/*
* Purpose : Returns Complete solution (Multiple Solutions also) of 4 variable Kmap represented by ipMinterm and ipdontCare
* Main Operations :
*	- Gets PI, (Tabulation)
*	- Finds EPI from them
*	- Checks EPI are enough to represent all
*	- If Yes returns
*	- Otherwise Proceeds to do Patrick method
*/
vector<vector<vector<int>>> kmap::getMinTerms(vector<int> ipMinterm,vector<int> ipdontCare){    
    vector<int> mintermDc= {};
	for(int i=0; i<ipMinterm.size(); i++) {
		mintermDc.push_back(ipMinterm[i]);
	}
	for(int i=0; i<ipdontCare.size(); i++) {
		mintermDc.push_back(ipdontCare[i]);
	}
	
	sort(mintermDc.begin(),mintermDc.end());
	vector<vector<vector<int>>> finalPi; //final prime implicantt

	//edge case 1
	if(ipMinterm.size()==0) return finalPi; //
	
	if(mintermDc.size()==16 && ipMinterm.size()!=0){
		// ans is "1" : always true
		finalPi={{{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}}};
		return finalPi;
	}
	vector<vector<int>> pi=primeImplicants(mintermDc);
	vector<string> chart=makeChart(pi,ipMinterm);
	vector<vector<int>> epi=findEpi(ipMinterm,chart,pi);
	set<int> epiMap; 
	for(int i=0;i<epi.size();i++) {
		for(int j=0; j<epi[i].size(); j++) {
			epiMap.insert(epi[i][j]);
		}
	}

	if(epiMap.size()==ipMinterm.size()) {
		//eg case that will return from here is minterms={0,1,3,7,8,9,11,15}
		finalPi.push_back(epi); // means final solution is EPI's only now exit main program ,3d vector
        return finalPi; //
	}

	// now if program reaches here it means epi do not cover all minterms so need to include other pi's also
	// now use patrick method
	// use brace expansion code from another project;
	// eg case reaching here ipminterm= {0,1,2,5,6,7}

	// removal of epis from pis giving us npi and chart;
	vector<vector<int>> npi=giveNpi(pi,epi);
	updateChart(chart,pi,epi);

	/*
	for(int i=0; i<step1.size(); i++) {
		if(search(epi,step1[i])) {
			step1.erase(step1.begin()+i);
			chart.erase(chart.begin()+i);
			i=-1;
		}
	}
	*/
	
	//removal of minterms (columns) covered by epi from
	vector<int> ipMintermUpdated=ipMinterm;
	for(int i=0; i<chart[0].size(); i++) {
		int currMinterm =ipMintermUpdated[i];
		auto it = epiMap.find(currMinterm);  
		if (it != epiMap.end()) {
			for(int j=0; j<chart.size(); j++) { 
				chart[j].erase(chart[j].begin()+i);
			}
		 ipMintermUpdated.erase(ipMintermUpdated.begin()+i);
			i=-1;
		}
	}

	unordered_map <int,char> piToAlpha; 
	unordered_map <char,int> alphaToPi;
	for(int i=0; i<npi.size(); i++) {
		int x=65+i;
		char a=char(x);
		piToAlpha[i]=a;
		alphaToPi[a]=i;

	}
	// cout<<piToAlpha[0];
    string toExpand=stringForExp(chart,piToAlpha); // this string will go for brace expansion in next step
	// cout<<toExpand<<endl;
	vector<string> expandedV=expand(toExpand);
	for(int i=0; i<expandedV.size(); i++) {
		removeDuplicateS(expandedV[i]); // removing duplicates from same string
	}

	// remove duplicates of same strings within the vector
	vector<string> temp;
	for(int i=0; i<expandedV.size(); i++) {
		string curr=expandedV[i];
		bool flag=true;
		for(int j=i+1; j<expandedV.size(); j++) {
			if(expandedV[i]==expandedV[j]){
				flag=false;break;
			}
		}
		if(flag)temp.push_back(expandedV[i]);
	}
	expandedV=temp;

	vector<string> minliterals= {};
	int mincount=1; // check for string size for 1 if found then ok otherwise find for 2;
	while (minliterals.empty()) {
    	for (int i=0;i<expandedV.size() ;i++) {
			string s=expandedV[i];
        	if (s.length() == mincount) {
            	minliterals.push_back(s);
        	}
    	}

    	if (minliterals.empty()) {
        	mincount++;
    	}
	}

	// now
	//checking of alphaToPi map
	// cout<<alphaToPi['A']<<endl;
	// cout<<alphaToPi['B']<<endl;
	vector<vector<vector<int>>> nEpi= {} ; // just like epi for non essential final
	for(int i=0; i<minliterals.size(); i++) {
		vector<vector<int>> current;
		for(int j=0; j<mincount; j++){
			char a=minliterals[i][j];
			int mintermIndex=alphaToPi[a];
			vector<int> pi=npi[mintermIndex];
			current.push_back(pi);
		}
		nEpi.push_back(current);
		if(epi.size()!=0) nEpi[i].insert(nEpi[i].begin(),epi.begin(),epi.end());
	}
	finalPi=nEpi;
    return finalPi;
} 


vector<string> kmap::finalString(vector<vector<vector<int>>> finalPi) {
     if(finalPi.size()==0){
         vector<string> ans={"0"};
         return ans;
     }
     vector<int> check={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
     if(finalPi.size()==1 && finalPi[0][0]==check){
         vector<string> ans={"1"};
         return ans;
     }
    
	vector<string> stringAns(finalPi.size(),"");
	for(int i=0; i<finalPi.size(); i++) {
		string currExpr="";
		for(int j=0; j<finalPi[i].size(); j++) {
			vector<int> curr=finalPi[i][j];
			string currTerm=piToString(curr);
			if(j!=finalPi[i].size()-1) currTerm+=" + " ;
			currExpr+=currTerm;

		}
		stringAns[i]+=currExpr;
	}
	return stringAns;
} 


/*main tabulation code*/
vector<vector<int>> kmap::primeImplicants(vector<int> inputMinTerms) {

	vector<vector<int>> table1(5); //5 due to tabele1[i]--> i 1s in binary representation
	vector<vector<int>> implicants;

	//Table 1
	for(int i=0; i<inputMinTerms.size(); i++) {
		int t=inputMinTerms[i];
		int cnt=0;
		while(t){
			int lsb=t&1;
			if(lsb) cnt++;
			t=t>>1;
		}
		table1[cnt].push_back(inputMinTerms[i]);
	}

	// Table 2
	vector<vector<vector<int>>> table2(4);
	for(int i=0; i<4; i++) {
		vector<int> currRow=table1[i];
		vector<int> nextRow=table1[i+1];
		for(int j=0; j<currRow.size(); j++) {
			int flag=0;
			for(int k=0; k<nextRow.size(); k++) {
				if(isOneBitDiff(intToBinary(currRow[j]),intToBinary(nextRow[k]))) {
					//merge & group
					flag=1;
					table2[i].push_back({currRow[j],nextRow[k]});
				}
			}
			// means if no group is formed for that number
			if(flag==0) {
				implicants.push_back({currRow[j]});
			}
		}
	}

	// last group of table 1 is not  pushed in implicants by above push method
	for(int i=0; i<table1[4].size(); i++) {
		implicants.push_back(table1[4]);
	}

	// Table3

	vector<vector<vector<int>>> table3(3);

// will have 3 groups 0,1,2 so 3 times run
	for(int i=0; i<3; i++) {
		vector<vector<int>> currRow=table2[i];
		vector<vector<int>> nextRow=table2[i+1];

		vector<vector<int>> visited= {};
		for(int k=0; k<currRow.size(); k++) {
			int flag=0;
			for(int j=0; j<nextRow.size(); j++) {
				// if dash at same place && one bit diff
				string s1=insertDash(currRow[k]);
				string s2=insertDash(nextRow[j]);
				// cout<< s1<<" "<<s2<<endl;
				if( dashPos(s1,s2) && isOneBitDiff(s1,s2) ) {
					// merege the 2 pairs
					flag=1;
					vector<int> temp= {currRow[k][0],currRow[k][1],nextRow[j][0],nextRow[j][1]};
					sort(temp.begin(),temp.end());
					// to not push duplicate values in the table row
					if(search(visited,temp)==0) {
						visited.push_back(temp);
						table3[i].push_back(temp);
					}
				}
			}

			if(flag==0) {
				//means no grouping is formed for that group; vector of 2 elements
				implicants.push_back({currRow[k][0],currRow[k][1]});
			}
		}
	}

	// last group of table2 is not pushed in implicants by above methods
	for(int i=0; i<table2[3].size(); i++) {
		implicants.push_back(table2[3][i]);
	}

	// checkTable(table3);

	// Table4(Last table)

	vector<vector<vector<int>>> table4(2);

	for(int i=0; i<2; i++) {
		vector<vector<int>> currRow=table3[i];
		vector<vector<int>> nextRow=table3[i+1];
		vector<vector<int>> visited= {};
		for(int j=0; j<currRow.size(); j++) {
			int flag=0;
			for(int k=0; k<nextRow.size(); k++) {
				vector<int> v1=currRow[j];
				vector<int> v2=nextRow[k];
				string s1=insertDash2(v1);
				string s2=insertDash2(v2);
				if(dashPos(s1,s2) && isOneBitDiff(s1,s2)) {
					// combine the group , this will be of 8 integers
					flag=1;
					vector<int> temp= {v1[0],v1[1],v1[2],v1[3],v2[0],v2[1],v2[2],v2[3]};
					sort(temp.begin(),temp.end());
					if(search(visited,temp)==0) {
						visited.push_back(temp);
						table4[i].push_back(temp);
					}
				}
			}
			if(flag==0) {
				if(currRow[j].size()!=0) {
					implicants.push_back({currRow[j][0],currRow[j][1],currRow[j][2],currRow[j][3]});
				}
			}
		}
	}

// last group implicants are not pushed by this method;

	for(int i=0; i<table3[2].size(); i++) {
		if(table3[2][i].size()!=0) {
			implicants.push_back(table3[2][i]);
		}
	}

	// checkTable(table4);
	// all elements of table 4 are implicants

	for(int i=0; i<table4.size(); i++) {
		for(int j=0; j<table4[i].size(); j++) {
			vector<int> v=table4[i][j];
			implicants.push_back(v);
			
		}
	}
	// cout<<"test"<<isOneBitDiff("1--0","1--1");

	// remove subset of smaller groups from larger groups;
	// removing subsets from larger sets;
	vector<vector<int>> impl;
	for(int i=0; i<implicants.size(); i++) {
		bool flag=false;
		for(int j=i+1;j<implicants.size();j++){
			if(subsetCheck(implicants[i],implicants[j])){
				flag=true;
			}
		}
		if(!flag)  impl.push_back(implicants[i]);
	}
	return impl;
}
vector<string> kmap::makeChart(vector<vector<int>> pi,vector<int> ipMinterm){
	vector<string> chart;
	string x(ipMinterm.size(),'x') ;  
	for(int i=0; i<pi.size(); i++) {
		vector<int> currRow=pi[i];
		for(int j=0; j<currRow.size(); j++) {
			// to check the current is in minterm vector or not
			auto it=find(ipMinterm.begin(),ipMinterm.end(),currRow[j]);
			if(it!=ipMinterm.end()) {
				// the current is not a dont care condn & is  present in ipMinterm so change string
				x[it-ipMinterm.begin()]='t';
			}

		}
		chart.push_back(x);
		x=string(ipMinterm.size(),'x'); //resetting for nextRow processing
	}
	return chart;
}
//  to find Epi from chart
vector<vector<int>> kmap::findEpi(vector<int> ipMinterm , vector<string> chart, vector<vector<int>> step1){
// traverse columnwisse strings to check if count of t=1;
// if count of t=1 then also find its corresponding implicant
	vector<vector<int>> epi;
    for(int j=0; j<ipMinterm.size(); j++) {
		int count=0;
		string temp="";
		for(int i=0; i<chart.size(); i++) {
			char ch=chart[i][j];
			temp+=ch;
			if(ch=='t') {
				count++;
			}
		}
		if(count==1) {
			int index=findt(temp);
			vector<int> tempEpi= step1[index];
			// to avoid duplicate pushing of same epi
			if(search(epi,tempEpi)==0) {
				epi.push_back(tempEpi);
			}
		}
	}
	return epi;
}

vector<vector<int>> kmap::giveNpi(vector<vector<int>> pi,vector<vector<int>> epi){
	vector<vector<int>> npi;
	for(int i=0;i<pi.size();i++){
		if(search(epi,pi[i])) continue;
		npi.push_back(pi[i]);
	}
	return npi;
}

 void  kmap::updateChart(vector<string> &chart,vector<vector<int>> pi,vector<vector<int>> epi){
	vector<string> temp;
	//removal rows (PIs) from the chart
	for(int i=0;i<pi.size();i++){
		if(search(epi,pi[i])) continue;
		temp.push_back(chart[i]);
	}
	chart=temp;
	return; 
	//now removal of minterms(columns) covered by epis 

}
/*patrick method*/
string kmap::stringForExp(vector<string> chart,unordered_map <int,char> piToAlpha){
    string toExpand=""; 
	// columnwise traversal
	for(int i=0; i<chart[0].size(); i++) {
		string bracket="{";
		for(int j=0; j<chart.size(); j++) {
			if(chart[j][i]=='t') {
				//  currPiIndex will be j //row no.
				char a=piToAlpha[j];
				string s;
				s.push_back(a);
				s.push_back(',');
				bracket+=s;

			}
		}

		if(bracket!="{" || bracket!="}") {
			//  always last character is , then pop it append } to this
			bracket.pop_back();
			bracket.push_back('}');
			// now add this string to toExpand
			toExpand+=bracket;

		}
	}
	return toExpand;
}

// function for brace expansion(external code)
vector<string> kmap::expand(string s) {
	vector <string> ret;
	int n;
	vector <string> list(100);
	n = 0;
	int flag = false;
	for(int i = 0; i < s.size(); i++) {
		if(s[i] == ',') {
			continue;
		} else if(s[i] == '{') {
			flag = true;
		} else if(s[i] == '}') {
			flag = false;
			n++;
		} else {
			list[n] += s[i];
			if(!flag)n++;
		}
	}
	solve(0, list,"",n,ret);
	sort(ret.begin(), ret.end());
	return ret;
}

// function for brace expansion(exteranl code)
void kmap::solve(int idx, vector <string> list, string curr,int n,vector<string> &ret) {
	if(idx == n) {
		ret.push_back(curr);
		return;
	}
	for(int i = 0; i < list[idx].size(); i++) {
		solve(idx + 1, list, curr + list[idx][i],n,ret);
	}
}


/*
*
* Small helper functions and utilities
*
*/

// Give the binary string representation of input number
string kmap::intToBinary(int n) {
	unordered_map<int,string> mapping= {
		{0,"0000"},
		{1,"0001"},
		{2,"0010"},
		{3,"0011"},
		{4,"0100"},
		{5,"0101"},
		{6,"0110"},
		{7,"0111"},
		{8,"1000"},
		{9,"1001"},
		{10,"1010"},
		{11,"1011"},
		{12,"1100"},
		{13,"1101"},
		{14,"1110"},
		{15,"1111"}
	};
	return mapping[n];
}


// Compares s1 and s2 and returns true if only one bit differs else false
bool kmap::isOneBitDiff(string s1, string s2) {
	int changes=0;
	for(int i=0; i<4; i++) {
		if(s1[i]!=s2[i] && (s1[i]!='-') && (s2[i]!='-')) {
			changes++;
		}
	}
	if(changes==1) return true;
	return false;
}

// Inserts dash character in combined string representation of group
string kmap::insertDash(vector<int> group) { 
	/*
	* for groups of size =2
	* we know if they are in 1 group then they have 1 bit difference
	*/
	string st1=intToBinary(group[0]);
	string st2=intToBinary(group[1]);
	for(int i=0; i<st1.length(); i++) {
		if(st1[i]!=st2[i]) {
			st1[i]='-';
		}
	}
	return st1;
}

// Returns true if both strings have dash ('-') at same positions
bool kmap::dashPos(string s1,string s2) {
	if(s1.size()==0 || s2.size()==0) return false;
	for(int i=0; i<s1.size(); i++){
		if(s1[i]=='-' && s1[i]!=s2[i]) return false;
	}
	return true;
}

// Inserts dash at difference position. Used in combining 2 + 2 terms into 4
string kmap::insertDash2(vector<int> v) {
	/*
	* for groups of size==4  (v.size()=4)
	* we know if they are in 1 group then they have 1 bit difference 1 dash pos is same
	*/
	if(v.empty()) return "";
	
	vector<int> p1= {v[0],v[1]};
	string st1=insertDash(p1);
	vector<int> p2= {v[2],v[3]};
	string st2=insertDash(p2);
	for(int i=0; i<st1.length(); i++){
		if(st1[i]!=st2[i]) st1[i]='-';
	}
	return st1;
}

// Inserts dash at difference position. Used in combining 4 + 4 terms into 8
string kmap::insertDash3(vector<int> v) {
	/*
	* for groups of size==8
	* we know if they are in 1 group then they have 1 bit difference 2 dash pos is same
	*/  
	if(v.empty()) return "";
	vector<int> v1= {v[0],v[1],v[2],v[3]};
	string st1=insertDash2(v1);
	vector<int> v2= {v[4],v[5],v[6],v[7]};
	string st2=insertDash2(v2);
	for(int i=0; i<st1.length(); i++) {
		if(st1[i]!=st2[i]) st1[i]='-';
	}
	return st1;
}

// Checks whether v2(1d vector) is present in v1(2d vector) or not 
bool kmap::search(vector<vector<int>> v1, vector<int> v2) {
	for(int i=0; i<v1.size(); i++){
		if(v1[i]==v2) return true;
	}
	return false;
}

// Check if all elements of v1 is present in v2 or not
bool kmap::subsetCheck(vector<int> v1, vector<int> v2) {
	set<int> mp;
	for(int i=0;i<v2.size();i++){
		mp.insert(v2[i]);
	}
	int cnt=0;
	for(int i=0;i<v1.size();i++){
		if(mp.find(v1[i])!=mp.end())cnt++;
	}
	
	if(cnt==v1.size()) return true;
	return false;
}

// Returns index of val inside v , if not found returns -1
int kmap::findV(vector<int> v,int val) {
	for(int i=0; i<v.size(); i++) {
		if(v[i]==val) return i;
	}
	return -1;
}

// Returns first index of t from given string
int kmap::findt(string x) {
	for(int i=0; i<x.length(); i++) {
		if(x[i]=='t') return i;
	}
	return -1;
}

// Returns a string containing only unique characters in it in sorted order
void kmap::removeDuplicateS(string &a) {
	sort(a.begin(),a.end());
	string temp="";
	for(int i=0; i<a.length()-1; i++) {
		if(a[i]!=a[i+1]) {
			temp.push_back(a[i]);
		}
	}
	if(temp.size()>0 && temp[temp.size()-1]!=a[a.size()-1])temp.push_back(a[a.size()-1]);
	a=temp;
	return;
}

// Returns String form of combined minterms(v) . Eg v={0,1} --> "000-" --> A'B'C'
string kmap::piToString(vector<int> v) {
	string ans="";
	string x="";
	if(v.size()==8) x=insertDash3(v);
	else if(v.size()==4) x=insertDash2(v);
	else if(v.size()==2) x=insertDash(v);
	else if(v.size()==1)x=intToBinary(v[0]);
	
	for(int i=0; i<x.length(); i++) { 
		char curr=x[i];
		if(curr!='-'){
			char a=char(65+i);
			ans.push_back(a);
			if(curr=='0') ans.push_back('\'');
		}
	}
	return ans;
}


/*
*
* debug functions
*
*/

// Prints vector of strings
void debug::printS(vector<string> a){
    
    for(int i=0; i<a.size(); i++) {
		cout<<a[i]<<endl;
		cout<<"/////////////"<<endl;
	}
}

//Prints vector of int
void debug::printV(vector<int> v) {
	for(int i=0; i<v.size(); i++) {
		cout<<v[i]<<" ";
	}
}

//Prints the 3d vector
void debug::checkTable(vector<vector<vector<int>>> table) {
	for(int i=0; i<table.size(); i++) {
		for(int j=0; j<table[i].size(); j++) {
			vector<int> temp=table[i][j];
			printV(temp);
			cout<<endl;
		}
		cout<<"/////////////";
		cout<<endl;
	}
}

//prints 2d vector
void debug::printRows(vector<vector<int>> v) {
	for(int i=0; i<v.size(); i++) {
		printV(v[i]);
		cout<<"\n";
	}
}

/*
int main() {	
	// {0,1,3,14} , {6}
	vector<int> ipMinterm= {0,1,2,5,6,7};
	vector<int> ipdontCare= {};
	kmap calc=kmap();
	debug d1=debug();
	vector<vector<vector<int>>> ans=calc.getMinTerms(ipMinterm,ipdontCare);
	vector<string> stringAns=calc.finalString(ans);
	d1.checkTable(ans);
	d1.printS(stringAns);
	return 0;
}
*/






