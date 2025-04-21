#include <iostream>
#include <unordered_map>
#include<vector>
#include<algorithm>
#include <set>
#include<Arduino.h>
#include"kmap.h"
using namespace std;

string intToBinary(int n) {
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

bool isOneBitDiff(string s1, string s2) {
	// lenght of both strings will be 4
	// will igonre dash
	int changes=0;
	for(int i=0; i<4; i++) {
		if(s1[i]!=s2[i] && (s1[i]=='1' || s1[i]=='0') && (s2[i]=='1' || s2[i]=='0')) {
			changes++;
		}
	}
	if(changes==1) {
		return true;
	}
	else {
		return false;
	}
}

string insertDash(pair<int,int> group) { // for groups of size =2
	// we know if they are in 1 group then they have 1 bit difference
	string st1=intToBinary(group.first);
	string st2=intToBinary(group.second);
	for(int i=0; i<st1.length(); i++) {
		if(st1[i]!=st2[i]) {
			st1[i]='-';
		}
	}
	return st1;
}


bool dashPos(string s1,string s2) {
	if(s1.size()==0 || s2.size()==0) {
		return false;
	}

	for(int i=0; i<s1.size(); i++) {
		if(s1[i]=='-') {
			if(s2[i]!='-') {
				return false;
			}
		}
	}
	return true;

}


void printV(vector<int> v) {
	for(int i=0; i<v.size(); i++) {
		cout<<v[i]<<" ";
	}
}


string insertDash2(vector<int> v) { // for groups of size==4
	// we know if they are in 1 group then they have 1 bit difference 1 dash pos is same
	// 0,1,2,3 first 2 group1 then last 2 group2
	if(!v.empty()) {
		pair<int,int> p1= {v[0],v[1]};
		string st1=insertDash(p1);
		pair<int,int> p2= {v[2],v[3]};
		string st2=insertDash(p2);
		for(int i=0; i<st1.length(); i++) {
			if(st1[i]!=st2[i]) {
				st1[i]='-';
			}
		}
		return st1;
	}
	else {
		return "";
	}

}

string insertDash3(vector<int> v) {  // for groups of size==8
	// we know if they are in 1 group then they have 1 bit difference 2 dash pos is same
	// 0,1,2,3 first 2 group1 then last 2 group2
	if(!v.empty()) {
		vector<int> v1= {v[0],v[1],v[2],v[3]};
		string st1=insertDash2(v1);
		vector<int> v2= {v[4],v[5],v[6],v[7]};
		string st2=insertDash2(v2);
		for(int i=0; i<st1.length(); i++) {
			if(st1[i]!=st2[i]) {
				st1[i]='-';
			}
		}
		return st1;
	}
	else {
		return "";
	}

}

void checkTable(vector<vector<vector<int>>> table) {
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

bool search(vector<vector<int>> v1, vector<int> v2) {
	for(int i=0; i<v1.size(); i++) {
		if(v1[i]==v2) {
			return true;
		}
	}
	return false;
}

// will check if all elements of v1 is present in v2 or not
bool subsetCheck(vector<int> v1, vector<int> v2) {

	int i=0;
	int j=0;
	int count=0;
	while(i<v1.size()) {
		int element=v1[i];
		int reference=v2[j];
		if(element==reference) {
			count++;
			i++;
		}
		j++;
		if(j==v2.size()) {
			i++;
			j=0;
		}
	}
	// cout<<"count"<<count<<endl;
	if(count==v1.size()) {
		return true;
	}
	else {
		return false;
	}

}

vector<vector<int>> primeImplicants(vector<int> inputMinTerms) {
	vector<vector<int>> table1;
	vector<int> empty= {};
	vector<vector<int>> implicants= {};

	//Table 1
	table1= {empty,empty,empty,empty,empty};
	for(int i=0; i<inputMinTerms.size(); i++) {
		int t=inputMinTerms[i];
		if(t==0) {
			table1[0].push_back(t);
		}

		if(t==1 || t==2 || t==4 || t==8) {
			table1[1].push_back(t);
		}

		if(t==3 || t==5 || t==6 || t==9 ||t==10 || t==12) {
			table1[2].push_back(t);
		}

		if(t==7 || t==11 || t==13 || t==14) {
			table1[3].push_back(t);
		}

		if(t==15) {
			table1[4].push_back(t);
		}
	}

	// //checking Table1

// 	  for(int i=0;i<table1.size();i++){
// 	     for(int j=0;j<table1[i].size();j++){
// 	      cout<< table1[i][j]<<endl;
// 	     }
// 	     cout<<"/////////////";
// 	     cout<<endl;
// 	}


	// Table 2

	vector<pair<int,int>> empty2= {};

	vector<vector<pair<int,int>>> table2= {empty2,empty2,empty2,empty2};
	for(int i=0; i<4; i++) {
		vector<int> currRow=table1[i];
		vector<int> nextRow=table1[i+1];
		for(int j=0; j<currRow.size(); j++) {
			int flag=0;
			for(int k=0; k<nextRow.size(); k++) {

				if(isOneBitDiff(intToBinary(currRow[j]),intToBinary(nextRow[k]))) {
					//merge & group
					flag=1;
					pair<int,int> group= {currRow[j],nextRow[k]};
					table2[i].push_back(group);
				}

			}
			// means if no group is formed for that number
			if(flag==0) {
				vector<int> impl= {currRow[j]}; // vector of single elment
				implicants.push_back(impl);
			}
		}
	}

	// last group of table 1is not  pushed in implicants by above push method
	for(int i=0; i<table1[4].size(); i++) {
		implicants.push_back(table1[4]);
	}



// 		// checking Table2

// 	  for(int i=0;i<table2.size();i++){
// 	     for(int j=0;j<table2[i].size();j++){
// 	      cout<< table2[i][j].first<<" "<<table2[i][j].second<<endl;
// 	     }
// 	     cout<<"/////////////";
// 	     cout<<endl;
// 	}


// // Table3
	vector<vector<int>> empty3= {{}};
	vector<vector<vector<int>>> table3= {empty3,empty3,empty3};

// will have 3 groups 0,1,2 so 3 times run
	for(int i=0; i<3; i++) {
		vector<pair<int,int>> currRow=table2[i];
		vector<pair<int,int>> nextRow=table2[i+1];

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
					int n1=  currRow[k].first;
					int n2=  currRow[k].second;
					int n3=  nextRow[j].first;
					int n4=  nextRow[j].second;
					vector<int> temp= {n1,n2,n3,n4};
					sort(temp.begin(),temp.end());
					// to not push duplicate values in the table row
					if(search(visited,temp)==0) {
						visited.push_back(temp);
						table3[i].push_back(temp);
					}
				}
			}

			if(flag==0) {
				//means no grouping is formed for that group; vector of 2 elements;
				vector<int> impl= {currRow[k].first,currRow[k].second};
				implicants.push_back(impl);
			}
		}
	}

	// last group of table2 is not pushed in implicants by above methods

	for(int i=0; i<table2[3].size(); i++) {
		{
			vector<int> impl= {table2[3][i].first,table2[3][i].second};
			implicants.push_back(impl);
		}
	}




// TODO Remove Duplicates & handle case of no groupings
// handle cases of non groupings in table 1,2,3. --->edit : handled in implicants vector pushing


// 	checkTable(table3);

// // checking insertDash2 function

// 	  for(int i=0;i<3;i++){
// 	     for(int j=0;j<table3[i].size();j++){
// 	        cout<<insertDash2(table3[i][j]);
// 	         cout<<endl;
// 	      }

// 	     cout<<"/////////////";
// 	     cout<<endl;

// 	}

	// Table4(Last table)

	vector<vector<vector<int>>> table4= {empty3,empty3};

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
					vector<int> temp= {
						v1[0],
						v1[1],
						v1[2],
						v1[3],
						v2[0],
						v2[1],
						v2[2],
						v2[3]
					};
					sort(temp.begin(),temp.end());
					if(search(visited,temp)==0) {
						visited.push_back(temp);
						table4[i].push_back(temp);
					}
				}
			}
			if(flag==0) {
				if(currRow[j].size()!=0) {
					vector<int> impl= {currRow[j][0],currRow[j][1],currRow[j][2],currRow[j][3]};
					implicants.push_back(impl);
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

// 	checkTable(table4);

	// all elements of table 4 are implicants

	for(int i=0; i<table4.size(); i++) {
		for(int j=0; j<table4[i].size(); j++) {
			vector<int> v=table4[i][j];
			if(v.size()!=0) {
				implicants.push_back(v);
			}
		}
	}

// 	cout<<"test"<<isOneBitDiff("1--0","1--1");


// remove subset of smaller groups from larger groups;

// removing subsets from larger sets;
	for(int i=0; i<implicants.size(); i++) {
		int flag=0;
		vector<int> currImpl=implicants[i];
		for(int j=implicants.size()-1; j>i; j--) {
			vector<int> toBeChecked=implicants[j];
			if(subsetCheck(currImpl,toBeChecked)) {
				flag=1;
			}
		}

		if(flag) {
			implicants.erase(implicants.begin()+i);
			i=-1;
		}
	}

	return implicants;

}


void printRows(vector<vector<int>> v) {
	for(int i=0; i<v.size(); i++) {
		printV(v[i]);
		cout<<endl;
	}
}

int findV(vector<int> v,int val) {
	for(int i=0; i<v.size(); i++) {
		if(v[i]==val) {
			return i;
		}
	}
	return -1;
}
// returns  first index of t from given string
int findt(string x) {
	for(int i=0; i<x.length(); i++) {
		if(x[i]=='t') {
			return i;
		}
	}
	return -1;
}


// function for brace expansion(exteranl code)
void solve(int idx, vector <string> list, string curr,int n,vector<string> &ret) {

	if(idx == n) {
		ret.push_back(curr);
		return;
	}
	for(int i = 0; i < list[idx].size(); i++) {
		solve(idx + 1, list, curr + list[idx][i],n,ret);
	}
}

// function for brace expansion(external code)
vector<string> expand(string s) {
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

void removeDuplicateS(string &a) {
	sort(a.begin(),a.end());
	for(int i=0; i<a.length()-1; i++) {
		if(a[i]==a[i+1]) {
			a.erase(a.begin()+i+1);
			i=-1;
		}
	}
}

string piToString(vector<int> v) {
	string ans="";
	if(v.size()==8) {
		//use insertDash3
		string x=insertDash3(v);
		for(int i=0; i<x.length(); i++) {
			char curr=x[i];
			if(curr!='-') {
				char a=char(65+i);
				if(curr=='1') {
					ans.push_back(a);
				}
				else if(curr='0') {
					ans.push_back(a);
					string comp="'"; // to represent its complement
					ans+=comp;
				}
			}
		}
	}
	else if(v.size()==4) {
		// use insertDash2
		string x=insertDash2(v);
		for(int i=0; i<x.length(); i++) {
			char curr=x[i];
			if(curr!='-') {
				char a=char(65+i);
				if(curr=='1') {
					ans.push_back(a);
				}
				else if(curr='0') {
					ans.push_back(a);
					string comp="'"; // to represent its complement
					ans+=comp;
				}
			}
		}

	}

	else if(v.size()==2) {
		// use insertDash
		pair<int,int> p= {v[0],v[1]};
		string x=insertDash(p);
		for(int i=0; i<x.length(); i++) {
			char curr=x[i];
			if(curr!='-') {
				char a=char(65+i);
				if(curr=='1') {
					ans.push_back(a);
				}
				else if(curr='0') {
					ans.push_back(a);
					string comp="'"; // to represent its complement
					ans+=comp;
				}
			}
		}
	}
	else if(v.size()==1) {
		// dont use insert dashing
		int n=v[0];
		string x=intToBinary(n);
		for(int i=0; i<x.length(); i++) {
			char curr=x[i];
			if(curr!='-') {
				char a=char(65+i);
				if(curr=='1') {
					ans.push_back(a);
				}
				else if(curr='0') {
					ans.push_back(a);
					string comp="'"; // to represent its complement
					ans+=comp;
				}
			}
		}
	}
	return ans;
}


void printS(vector<string> a){
    
    for(int i=0; i<a.size(); i++) {
		cout<<a[i]<<endl;
		cout<<"/////////////"<<endl;
	}
}

//  to find Epi from chart
void findEpi(vector<vector<int>> &epi , vector<int> ipMinterm , vector<string> chart, vector<vector<int>> step1){
// traverse columnwisse strings to check if count of t=1;
// if count of t=1 then also find its corresponding implicant
    for(int j=0; j<ipMinterm.size(); j++) {
		int count=0;
		string temp;
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
}

string stringForExp(vector<string> chart,unordered_map <int,char> piToAlpha){
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
kmap :: kmap(){

}
vector<vector<vector<int>>> kmap::getMinTerms(vector<int> ipMinterm,vector<int> ipdontCare){
    
    vector<int> mintermDc= {};
	for(int i=0; i<ipMinterm.size(); i++) {
		mintermDc.push_back(ipMinterm[i]);
	}
	for(int i=0; i<ipdontCare.size(); i++) {
		mintermDc.push_back(ipdontCare[i]);
	}
	
	sort(mintermDc.begin(),mintermDc.end());
	vector<vector<vector<int>>> finalPi= {};
	vector<int> check={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	if(ipMinterm.size()==0){
	    cout<<"final groups"<<endl;
	       // stringAns={"0"};
	       // cout<<"String solutions -"<<endl;
	       // printS(stringAns);
	        return finalPi;
	}
	
	
	if(mintermDc== check){
	    if(ipMinterm.size()!=0){
	    finalPi={{check}};
	   // stringAns={"1"};
	   // printS(stringAns);
	    return finalPi;
	    }
	}
	vector<vector<int>> step1=primeImplicants(mintermDc);
     
	vector<string> chart= {};


	string x(ipMinterm.size(),'x') ;  
	string reset=x;


	for(int i=0; i<step1.size(); i++) {
		vector<int> currRow=step1[i];
		for(int j=0; j<currRow.size(); j++) {
			int current=currRow[j];
			// to check the current is in minterm vector or not
			int index=findV(ipMinterm,current);  // returns index of element  from vector
			if(index!=-1) {
				// the current is not a dont care condn & is  present in ipMinterm so change string
				x[index]='t';
			}

		}
		chart.push_back(x);
		x=reset; //resetting for nextRow processing
	}



// find essential PI
	vector<vector<int>> epi= {};
    findEpi(epi,ipMinterm,chart,step1);



	
	unordered_map<int,bool> epiMap= {}; 

	for(int i=0; i<epi.size(); i++) {
		for(int j=0; j<epi[i].size(); j++) {
			int curr= epi[i][j];
			epiMap[curr]=1;
		}
	}

	bool epiIsSol=true;
	for(int i=0; i<ipMinterm.size(); i++) {
		int curr=ipMinterm[i];
		auto it = epiMap.find(curr);
		if (it == epiMap.end()) {
			epiIsSol=false;

		}
	}

	if(epiIsSol) {
		finalPi.push_back(epi); // means final solution is EPI's only now exit main program
// 		cout<<endl<<"final answer"<<endl;
// 		checkTable(finalPi); // to print final pi
// 		stringAns=finalString(finalPi);
// 		printS(stringAns);
// 		return 1;
        return finalPi;
	}


	// now if program reaches here it means epi do not cover all minterms so need to include other pi's also
	// now use patrick method
	// use brace expansion code from another project;


	// removal from step1;
	for(int i=0; i<step1.size(); i++) {
		vector<int> curr=step1[i];
		if(search(epi,curr)) {
			step1.erase(step1.begin()+i);
			chart.erase(chart.begin()+i);
			i=-1;
		}

	}




	vector<int> ipMintermCopy=ipMinterm;
	for(int i=0; i<chart[0].size(); i++) {
		int currMinterm=ipMintermCopy[i];
		auto it = epiMap.find(currMinterm);  
		if (it != epiMap.end()) {
			for(int j=0; j<chart.size(); j++) { 
				chart[j].erase(chart[j].begin()+i);
			}
			ipMintermCopy.erase(ipMintermCopy.begin()+i);
			i=-1;
		}
	}



	unordered_map <int,char> piToAlpha= {}; 
	unordered_map <char,int> alphaToPi= {};
	for(int i=0; i<step1.size(); i++) {
		int x=65+i;
		char a=char(x);
		piToAlpha[i]=a;
		alphaToPi[a]=i;

	}

// 	cout<<piToAlpha[0];

    string toExpand=stringForExp(chart,piToAlpha); // this string will go for brace expansion

// 	cout<<toExpand<<endl;

	vector<string> expandedV=expand(toExpand);

	for(int i=0; i<expandedV.size(); i++) {

		removeDuplicateS(expandedV[i]); // removing duplicates from same string
	}

	// remove duplicates of same strings within the vector
	for(int i=0; i<expandedV.size(); i++) {
		string curr=expandedV[i];
		for(int j=0; j<expandedV.size(); j++) {
			// pass the case i=j
			if(i!=j) {
				string comp=expandedV[j];
				if(curr==comp) {
					expandedV.erase(expandedV.begin()+j);
					j=0;
				}
			}
		}
	}



	vector<string> minliterals= {};
	int mincount=1; // check for string size for 1 if found then ok otherwise find for 2;
	for(int i=0; i<expandedV.size(); i++) {
		if(expandedV[i].length()==mincount) {
			minliterals.push_back(expandedV[i]);
		}
		else {

			if(i==expandedV.size()-1) {
				if(minliterals.size()==0) {
					mincount++;
					i=-1; //restart the loop;
				}

			}
		}
	}



	// now
	//checking of alphaToPi map
// 	cout<<alphaToPi['A']<<endl;
// 	cout<<alphaToPi['B']<<endl;
	vector<vector<vector<int>>> nEpi= {} ; // just like epi for non essential final
	for(int i=0; i<minliterals.size(); i++) {
		vector<vector<int>> current= {};
		for(int j=0; j<mincount; j++) {
			char a=minliterals[i][j];
			int mintermIndex=alphaToPi[a];
			vector<int> pi=step1[mintermIndex];
			current.push_back(pi);
		}


		nEpi.push_back(current);
		if(epi.size()!=0) {
			nEpi[i].insert(nEpi[i].begin(),epi.begin(),epi.end());
		}


	}
	finalPi=nEpi;
    return finalPi;
} // export

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
			if(j!=finalPi[i].size()-1) {
				currTerm+=" + " ;
			}
			currExpr+=currTerm;

		}
		stringAns[i]+=currExpr;
	}
	return stringAns;
} // export

// int test() {	
// 	vector<int> ipMinterm= {0,1,3,14};
// 	vector<int> ipdontCare= {6};
// 	vector<vector<vector<int>>> ans=getMinTerms(ipMinterm,ipdontCare);
// 	checkTable(ans);
// 	vector<string> stringAns=finalString(ans);
//     printS(stringAns);


// 	return 0;
// }




