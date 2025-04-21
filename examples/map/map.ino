#include<string>
#include <vector>
#include <kmap.h>
using namespace std;
kmap x=kmap();
vector<int> ipmt={0,2,4,10};
vector<int> ipdc={5};
vector<vector<vector<int>>> mt=x.getMinTerms(ipmt,ipdc);
vector<string> st=x.finalString(mt);
void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("hey");
  printSt(st);
  // Serial.println(mt[0][0][0]);

}

void loop() {
  // put your main code here, to run repeatedly:

}


void printSt(vector<string> v){
  for(int i=0; i<v.size();i++){
    Serial.println(v[i].c_str());
    delay(10);
  }
}