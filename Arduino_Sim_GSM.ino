//ئامادە کراو لەلایەن: موحەمەد ڕەفیق
//ئیمەیل: mahanrafiq@gmail.com
//فەیسبووک: fb.com/muhammadhalabja7
//مۆبایل: 07501064007
//شێوازی بەکارهێنان:
//سەرەتا لە کۆدەکاندا 11 جار ئەم ژمارەیە نوسراوە. 07501064007 هەمووی بگۆڕە بۆ ژمارەی موبایلەکەی خۆت.
//پاشان لەڕێی نامەی موبایلەکەتەوە دەتوانیت ئەم نامانە بنێریت بۆ ژمارەکە:
// Relay Num:           On code:        Off code:
// Relay 1(Pin D3)      Relay1on        Relay1off
// Relay 2(Pin D4)      Relay2on        Relay2off
// Relay 3(Pin D5)      Relay3on        Relay3off
// Relay 4(Pin D6)      Relay4on        Relay4off
// All Relays           All On          All Off  


#include <gprs.h>
#include <softwareserial.h>
 
#define TIMEOUT    5000
#define LED_PIN    13
#define ON 0
#define OFF 1

const int RLY1 = 3;
const int RLY2 = 4;
const int RLY3 = 5;
const int RLY4 = 6;
 
int StatusRLY1;
int StatusRLY2;
int StatusRLY3;
int StatusRLY4;
GPRS gprs;
 
void setup() {
  pinMode (RLY1 , OUTPUT);
  pinMode (RLY2 , OUTPUT);
  pinMode (RLY3 , OUTPUT);
  pinMode (RLY4 , OUTPUT);
  Serial.begin(9600);
  while(!Serial);
 
  Serial.println("Starting SIM800 Auto Read SMS");
  gprs.preInit();
  delay(1000);
 
  while(0 != gprs.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  } 
 
  //Set SMS mode to ASCII
  if(0 != gprs.sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", TIMEOUT)) {
    ERROR("ERROR:CNGF");
    return;
  }
   
  //Start listening to New SMS Message Indications
  if(0 != gprs.sendCmdAndWaitForResp("AT+CNMI=1,2,0,0,0\r\n", "OK", TIMEOUT)) {
    ERROR("ERROR:CNMI");
    return;
  }
 
  Serial.println("Init success");

  //bo agadar krdnawa ka karaba hatawa
  gprs.sendSMS("07501064007","Esta karaba hatawa");
}
 
//Variable to hold last line of serial output from SIM800
char currentLine[500] = "";
int currentLineIndex = 0;
 
//Boolean to be set to true if message notificaion was found and next
//line of serial output is the actual SMS message content
bool nextLineIsMessage = false;
 
void loop() {
  //Write current status to LED pin
  digitalWrite(RLY1, StatusRLY1);
  digitalWrite(RLY2, StatusRLY2);
  digitalWrite(RLY3, StatusRLY3);
  digitalWrite(RLY4, StatusRLY4);
   
  //If there is serial output from SIM800
  if(gprs.serialSIM800.available()){
    char lastCharRead = gprs.serialSIM800.read();
    //Read each character from serial output until \r or \n is reached (which denotes end of line)
    if(lastCharRead == '\r' || lastCharRead == '\n'){
        String lastLine = String(currentLine);
         
        //If last line read +CMT, New SMS Message Indications was received.
        //Hence, next line is the message content.
        if(lastLine.startsWith("+CMT:")){
           
          Serial.println(lastLine);
          nextLineIsMessage = true;
           
        } else if (lastLine.length() > 0) {
           
          if(nextLineIsMessage) {
            Serial.println(lastLine);
             

// Membaca dan mengartikan perintah SMS yang diterima
            //Kendali Relay 1
            if(lastLine.indexOf("Relay1on") >= 0){
              StatusRLY1 = 1;
              gprs.sendSMS("07501064007","Motor is On");
              Serial.println("Relay1/Saklar1 DIMATIKAN");} 
              else if(lastLine.indexOf("Relay1off") >= 0) {
              StatusRLY1 = 0;
              gprs.sendSMS("07501064007","Motor is Off");
              Serial.println("Relay1/Saklar1 DINYALAKAN");}
            //Kendali Relay 2
            if(lastLine.indexOf("Relay2on") >= 0){
              StatusRLY2 = 1;
              gprs.sendSMS("07501064007","Motor 2 is On");
              Serial.println("Relay2/Saklar2 DIMATIKAN");} 
              else if(lastLine.indexOf("Relay2off") >= 0) {
              StatusRLY2 = 0;
              gprs.sendSMS("07501064007","Relay 2 is Off");
              Serial.println("Relay2/Saklar2 DINYALAKAN");}
            //Kendali Relay 3
            if(lastLine.indexOf("Relay3on") >= 0){
              StatusRLY3 = 1;
              gprs.sendSMS("07501064007","Relay 3 is on");
              Serial.println("Relay3/Saklar3 DIMATIKAN");} 
              else if(lastLine.indexOf("Relay3off") >= 0) {
              StatusRLY3 = 0;
              gprs.sendSMS("07501064007","Relay 3 is off");
              Serial.println("Relay3/Saklar3 DINYALAKAN");}
            //Kendali Relay 4
            if(lastLine.indexOf("Relay4on") >= 0){
              StatusRLY4 = 1;
              gprs.sendSMS("07501064007","Relay 4 is on");
              Serial.println("Relay4/Saklar4 DIMATIKAN");} 
              else if(lastLine.indexOf("Relay4off") >= 0) {
              StatusRLY4 = 0;
              gprs.sendSMS("07501064007","Relay 4 is off");
              Serial.println("Relay4/Saklar4 DINYALAKAN");}
            //Kendali Semua LED
            if(lastLine.indexOf("All On") >= 0){
              StatusRLY1 = 1;
              StatusRLY2 = 1;
              StatusRLY3 = 1;
              StatusRLY4 = 1;
              gprs.sendSMS("07501064007","All is On");
              Serial.println("Semua Relay/Saklar DIMATIKAN");} 
              else if(lastLine.indexOf("All Off") >= 0) {
              StatusRLY1 = 0;
              StatusRLY2 = 0;
              StatusRLY3 = 0;
              StatusRLY4 = 0;
              gprs.sendSMS("07501064007","All Relays is Off");
              Serial.println("Semua Relay/Saklar DINYALAKAN");}
             
            nextLineIsMessage = false;
          }
           
        }
         
        //Clear char array for next line of read
        for( int i = 0; i < sizeof(currentLine);  ++i ) {
         currentLine[i] = (char)0;
        }
        currentLineIndex = 0;
    } else {
      currentLine[currentLineIndex++] = lastCharRead;
    }
  }
}
