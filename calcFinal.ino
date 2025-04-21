#include <LiquidCrystal.h>
#include <kmap.h>
#include <FastLED.h>
#include <vector>
#include<string>
using namespace std;



// defining pins

const int resetButton=13;
// pins for display
const int rs = 23, en = 22, d4 = 5, d5 = 18, d6 = 19, d7 =21;
const int nextButton=27;

//pins for multiplexer 
const int controlPins[] = { 26, 25, 33, 32 };
const int exeButton=14;
const int op=35;

//pins for led strips
const int NUM_LEDS= 16;
const int DATA_PIN1 =16;
const int DATA_PIN2 =4;
const int DATA_PIN3= 2;
const int DATA_PIN4 =15;



// defining lcd and variables
int resetButtonNew;
int resetButtonOld=1;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int nextButtonNew;
int nextButtonOld=1;


CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];


int colour[] = {
  CRGB::Blue, CRGB :: Red, CRGB:: Green, CRGB::Orange, 
  CRGB :: Purple, CRGB :: Yellow, CRGB ::Brown, CRGB::ForestGreen,
  CRGB ::Pink, CRGB::MediumSpringGreen,CRGB ::PaleGreen, CRGB::Chocolate,
  CRGB ::Coral, CRGB::Azure, CRGB::Indigo, CRGB::Aqua
};
kmap x=kmap();
vector<int> ipmt={}; // this vector will record i/p minterms
vector<int> ipdc={};  // this vector will record i/p donctcare
vector<vector<vector<int>>> opmt={};
vector<string> opst={};
int currSolIndex=-1;
int totalSols=0;
int exeButtonNew;
int exeButtonOld=1;

// functions

// ---for lcd at end after the intial setup
// ---for i/p
int readMux(int channel){
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPins[i], channel >> i & 1);
  }
  delay(10);
  float vol=analogRead(op);
  int x=map(vol, 0, 4095, 0, 3); // state 3 will be when
  return x;
}

void takeInput(vector<int> &mt ,vector<int> &dc){  // 
  for(int i=0;i<16;i++){
    int x =readMux(i);
    // x=0 means lever at mid so logic 0 dont do anything
    if(x==0){
      // dont care , lever down 
      dc.push_back(i);
    }
    else if(x==2 || x==3){
      // logic 1 , lever up
      mt.push_back(i);
    }
  }
  Serial.println("minterms");
  printVector(mt);
  Serial.println("dont cares");
  printVector(dc);
}
void initiateLEDStripBlack( CRGB leds[]){
    for(int i=0; i<16;i++){
      leds[i]= CRGB :: Black;
      delay(5);
    }
    FastLED.show();
}
void initiateLEDBlack(){
  initiateLEDStripBlack(leds1);
  initiateLEDStripBlack(leds2);
  initiateLEDStripBlack(leds3);
  initiateLEDStripBlack(leds4);
}
void initiateLEDPattern(){
  for(int i=0; i<16; i++){
    leds1[i]=CRGB :: Blue;
    delay(30);
    FastLED.show();
  }
  delay(20);
  for(int i=15; i>=0; i--){
    leds2[i]=CRGB :: Red;
    delay(30);
    FastLED.show();
  }
  delay(20);
  for(int i=0; i<16; i++){
    leds3[i]=CRGB :: Green;
    delay(30);
    FastLED.show();
  }
  delay(20);
  for(int i=15; i>=0; i--){
    leds4[i]=CRGB :: Yellow;
    delay(30);
    FastLED.show();
  }
  delay(20);
}

void setup() {
  Serial.begin(115200);
  pinMode(nextButton,INPUT_PULLUP);
  lcd.begin(16, 2);

  for (int i = 0; i < 4; ++i){
    pinMode(controlPins[i], OUTPUT);
  }
  pinMode(exeButton,INPUT_PULLUP);
  FastLED.addLeds<WS2812,DATA_PIN1,RGB>(leds1,NUM_LEDS);
  FastLED.addLeds<WS2812,DATA_PIN2,RGB>(leds2,NUM_LEDS);
  FastLED.addLeds<WS2812,DATA_PIN3,RGB>(leds3,NUM_LEDS);
  FastLED.addLeds<WS2812,DATA_PIN4,RGB>(leds4,NUM_LEDS);
  pinMode(resetButton,INPUT_PULLUP);
  
  initiateLEDPattern();
  initiateLEDBlack();
  startupScreen();
  delay(1000);
}
// n=4// for 4 variable
void loop() {

  // exe button logic
  lcd.scrollDisplayLeft();
  delay(200);
  exeButtonNew=digitalRead(exeButton);
  
  if(exeButtonOld== 0 && exeButtonNew==1){
    Serial.println("exe button pressed");
    ipmt.clear();
    ipdc.clear();
    opmt.clear();
    opst.clear();
    clearOutput();
    // delay(20);
    currSolIndex=-1;
    totalSols=0;
    takeInput(ipmt,ipdc);

   // call kmap function to solve km
    opmt=x.getMinTerms(ipmt,ipdc);
    opst=x.finalString(opmt);
    currSolIndex=0;
    totalSols=opst.size();
  // diplay 0 index solution in lcd & leds
    showOutput();

  }
  exeButtonOld=exeButtonNew;
  delay(10); // to make handle debounce 


     // next sol button logic
  nextButtonNew=digitalRead(nextButton); 
  if(nextButtonOld==0 && nextButtonNew==1){
    Serial.println("next button pressed");
    if(totalSols==0){
      lcd.clear();
      lcd.print("No Solution");
      delay(1000);
      lcd.clear();
    }
    else{
    currSolIndex++;
    currSolIndex=currSolIndex%totalSols;
    showOutput();
    }
  }
  nextButtonOld=nextButtonNew;
  delay(10); 


     //reset button logic
  resetButtonNew=digitalRead(resetButton);  
  if(resetButtonOld==0 && resetButtonNew==1){
    Serial.println("reset button pressed");
    ipmt.clear();
    ipdc.clear();
    opmt.clear();
    opst.clear();
    clearOutput();
    currSolIndex=-1;
    totalSols=0;
  }
  resetButtonOld=resetButtonNew;
  delay(10);


}


void startupScreen(){
    lcd.print("K-MAP CALCULATOR");
    blinkScreen();
    lcd.print("KARAN SHARMA");
    lcd.setCursor(0, 1);
    lcd.print("2023UEC2600");
    blinkScreen();
    lcd.print("AADARSH SAHNI");
    lcd.setCursor(0, 1);
    lcd.print("2023UEC2609");
    blinkScreen();
    lcd.print("SHIVAM");
    lcd.setCursor(0, 1);
    lcd.print("2023UEC2581");
    blinkScreen();
 }

 void blinkScreen(){
  delay(500);
  lcd.display();
  delay(500);
  lcd.clear();
  delay(500);
  lcd.display();
 }

 void lcdPrint(int i){
    string ans=opst[i];
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Solution : ") ;
    lcd.print(i+1);
    lcd.setCursor(0, 1);
    lcd.print(ans.c_str());
    delay(1500);
 }

 void ledShow(int i){
  if(opmt.empty()){
    initiateLEDBlack();
  }
  else{
     vector<vector<int>> ans=opmt[i];
  for(int j=0;j<ans.size();j++){
    int n= ans[j].size();
    for(int k=0;k<n;k++){
      int curr=ans[j][k];
      int startIndex;
      int currVacant;
      if(curr==0 || curr==1 || curr==2 || curr==3){
        // select strip 1
        setStartStrip1(curr,startIndex);
        currVacant=offLedIndex(startIndex,leds1);
        leds1[currVacant]=colour[j];
      }


      else if(curr==4 || curr==5 || curr==6 || curr==7){
        // select strip 2
        setStartStrip2(curr,startIndex);
        currVacant=offLedIndex(startIndex,leds2);
        leds2[currVacant]=colour[j];
        
      }
      else if(curr==12 || curr==13 || curr==14 || curr==15){
        // select strip 3
        setStartStrip3(curr,startIndex);
        currVacant=offLedIndex(startIndex,leds3);
        leds3[currVacant]=colour[j];
      }
      else if(curr==8 || curr==9 || curr==10 || curr==11){
        // select strip 4
        setStartStrip4(curr,startIndex);
        currVacant=offLedIndex(startIndex,leds4);
        leds4[currVacant]=colour[j];
      }
    }  
  }
  FastLED.show();
  // delay(1000);
  }
 
 }

 void showOutput(){
  if(currSolIndex>=0 && totalSols>0){
      clearOutput();
      lcdPrint(currSolIndex);
      ledShow(currSolIndex);
    }
    else{
      clearOutput();
      lcd.print("Invalid");
      delay(1000);
      lcd.clear();
    }
 }

 void clearOutput(){
    lcd.clear();
    lcd.setCursor(0,0);
    initiateLEDBlack();
 }

 int offLedIndex(int startIndex, CRGB leds[]){
  int n=startIndex+4;
  for(int i=startIndex ; i<n;i++){ // traverse startIndex,startIndex+1,startIndex+2,startIndex+3
    if(leds[i] == CRGB :: Black){
      return i;
    }
  }
  return -1; // means no led is off -->means all are on
}

void setStartStrip1(int curr, int &startIndex){
  if(curr==0){
        startIndex=0;
      }
      else if(curr==1){
        startIndex=4;
      }
      else if(curr==2){
        startIndex=12;
      }
      else if(curr==3){
        startIndex=8;
      }
}
void setStartStrip2(int curr, int &startIndex){
  if(curr==4){
        startIndex=0;
      }
      else if(curr==5){
        startIndex=4;
      }
      else if(curr==6){
        startIndex=12;
      }
      else if(curr==7){
        startIndex=8;
      }
}

void setStartStrip3(int curr, int &startIndex){
  if(curr==12){
        startIndex=0;
      }
      else if(curr==13){
        startIndex=4;
      }
      else if(curr==14){
        startIndex=12;
      }
      else if(curr==15){
        startIndex=8;
      }
}
void setStartStrip4(int curr, int &startIndex){
  if(curr==8){
        startIndex=0;
      }
      else if(curr==9){
        startIndex=4;
      }
      else if(curr==10){
        startIndex=12;
      }
      else if(curr==11){
        startIndex=8;
      }
}
void printVector(vector<int> a){
  for(int i=0;i<a.size();i++){
    Serial.println(a[i]);
  }
}