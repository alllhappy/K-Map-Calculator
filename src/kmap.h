
#ifndef kmap_h
#define kmap_h
#include<vector>
#include<string>
using namespace std;
class kmap{
  public :
    vector<vector<vector<int>>> getMinTerms(vector<int> ipMinterm , vector<int> ipdontCare);
    vector<string> finalString(vector<vector<vector<int>>> finalPi);

  private :
    /*tabulation*/
    vector<vector<int>> primeImplicants(vector<int> inputMinTerms);
    vector<string> makeChart(vector<vector<int>> pi,vector<int> ipMinterm);

    /*essential PI from chart*/
    vector<vector<int>> findEpi(vector<int> ipMinterm , vector<string> chart, vector<vector<int>> step1);

    /*non essential PI from char*/
    vector<vector<int>> giveNpi(vector<vector<int>> pi,vector<vector<int>> npi);
    void updateChart(vector<string> &chart,vector<vector<int>> pi,vector<vector<int>> npi);

    /*petrick method*/
    string stringForExp(vector<string> chart,unordered_map <int,char> piToAlpha);
    vector<string> expand(string s) ;//external
    void solve(int idx, vector <string> list, string curr,int n,vector<string> &ret); //brace expansion,exteranl

    /*small helper functions and utilities*/
    string intToBinary(int n);
    bool isOneBitDiff(string s1, string s2);
    bool dashPos(string s1,string s2);
    string insertDash(vector<int> group);
    string insertDash2(vector<int> v);
    string insertDash3(vector<int> v);
    bool search(vector<vector<int>> v1, vector<int> v2);
    bool subsetCheck(vector<int> v1, vector<int> v2);
    int findV(vector<int> v,int val);
    int findt(string x);
    string piToString(vector<int> v);
    void removeDuplicateS(string &a);
};

class debug{
  public:
  void printV(vector<int> v);
  void checkTable(vector<vector<vector<int>>> table);
  void printRows(vector<vector<int>> v);
  void printS(vector<string> a);
};
#endif