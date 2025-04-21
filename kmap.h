
#ifndef kmap_h
#define kmap_h
#include "Arduino.h"
#include<vector>
#include<string>
using namespace std;
class kmap{
  public :
  kmap();
  vector<vector<vector<int>>> getMinTerms(vector<int> ipMinterm , vector<int> ipdontCare);
  vector<string> finalString(vector<vector<vector<int>>> finalPi);
};
#endif