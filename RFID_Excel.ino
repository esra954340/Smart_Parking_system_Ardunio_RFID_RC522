#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10 //RX slave select
#define RST_PIN 9
Servo myservo;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

byte card_ID[4]; //card UID size 4byte
byte Name1[4]={0x53,0x98,0xCE,0x2E};//first UID card 53 98 CE 2E(card)

byte Name2[4]={0x90,0x26,0x57,0x73};//second UID card 90 26 57 73(keychain)


int NumbCard[2];//the number of cards. in my case i have just two cards.
int j=0;  
int statu[2];//the number of cards. in my case i have just two cards.
int s=0;        

int const RedLed=6;
int const GreenLed=5;
int const Buzzer=8;
int const Motor=3;

String Log;
String Name;//user name
long Number;//user number
int n ;//The number of card you want to detect (optional)  

void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,Date,Name,Number,Time IN,Time OUT");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  pinMode(RedLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(Motor,OUTPUT);
  delay(200);
   myservo.attach(3);
myservo.write(0); 
/*
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
*/  
   }
   
void loop() {
  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }
 
 for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Name1[i]){
       Name="ESRA";//user name
       Number=114;//user number
       j=0;
       s=0;
      }
      else if(card_ID[i]==Name2[i]){
       Name="VIMAL";//user name
       Number=102;//user number
       j=1;
       s=1;
     
     }
     
      else{
          digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,HIGH);
          digitalWrite(Buzzer,HIGH);
      delay(400);
       digitalWrite(Buzzer,LOW);
       
          goto cont;//go directly to line 71
     }
}
      if(NumbCard[j] == 1 && statu[s] == 0){
      statu[s]=1;
      Serial.print("DATA,DATE," + Name);//send the Name to excel
      Serial.print(",");
      Serial.print(Number); //send the Number to excel
      Serial.print(",");
      Serial.print("");
      Serial.print(",");
      Serial.println("TIME");
      
      }
      else if(NumbCard[j] == 0){
      NumbCard[j] = 1;
      n++;
      Serial.print("DATA,DATE," + Name);//send the Name to excel
      Serial.print(",");
      Serial.print(Number); //send the Number to excel
      Serial.print(",");
      Serial.print("TIME");
      Serial.print(",");
      Serial.println("");
      digitalWrite(GreenLed,HIGH);
      digitalWrite(RedLed,LOW);
      digitalWrite(Buzzer,HIGH);
      delay(30);
      digitalWrite(Buzzer,LOW);
     Serial.println("Gate Open");
      myservo.write(70);   
      delay(5000);
      myservo.write(0);
         Serial.println("Gate Close");
myservo.write(LOW);
      }
      else if(statu[s] == 1){
     //Turn Red LED when the employee Already Left
     digitalWrite(RedLed,HIGH);
      }
      delay(1000);
cont:
delay(2000);
digitalWrite(GreenLed,LOW);
digitalWrite(RedLed,LOW);

//if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 2 card (optional)
/*if(n==2){
    Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
    Serial.println("FORCEEXCELQUIT");
    }*/
}
    
