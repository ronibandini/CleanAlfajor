// Clean Alfajor Dispenser 
// Roni Bandini
// Argentina, August 2020


#include <Wire.h> 
#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

char ssid[] = "SSIDHere";       
char pass[] = "PASSHere";        

int status = WL_IDLE_STATUS;
char server[] = "RemoteServer.com";     
WiFiClient client;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

// Icons by Artur Funk
 unsigned char wifi1_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000111, 0b11100000, //      ######     
  0b00011111, 0b11111000, //    ##########   
  0b00111111, 0b11111100, //   ############  
  0b01110000, 0b00001110, //  ###        ### 
  0b01100111, 0b11100110, //  ##  ######  ## 
  0b00001111, 0b11110000, //     ########    
  0b00011000, 0b00011000, //    ##      ##   
  0b00000011, 0b11000000, //       ####      
  0b00000111, 0b11100000, //      ######     
  0b00000100, 0b00100000, //      #    #     
  0b00000001, 0b10000000, //        ##       
  0b00000001, 0b10000000, //        ##       
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char clock_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000011, 0b11100000, //       #####     
  0b00000111, 0b11110000, //      #######    
  0b00001100, 0b00011000, //     ##     ##   
  0b00011000, 0b00001100, //    ##       ##  
  0b00110000, 0b00000110, //   ##         ## 
  0b00110000, 0b00000110, //   ##         ## 
  0b00110000, 0b11111110, //   ##    ####### 
  0b00110000, 0b10000110, //   ##    #    ## 
  0b00110000, 0b10000110, //   ##    #    ## 
  0b00011000, 0b10001100, //    ##   #   ##  
  0b00001100, 0b00011000, //     ##     ##   
  0b00000111, 0b11110000, //      #######    
  0b00000011, 0b11100000, //       #####     
  0b00000000, 0b00000000, //                 
};

 unsigned char lock_open_icon16x16[] =
{
  0b00111111, 0b11111100, //   ############  
  0b00111111, 0b11111100, //   ############  
  0b00111000, 0b00011100, //   ###      ###  
  0b00111000, 0b00000000, //   ###           
  0b00111000, 0b00000000, //   ###           
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b00000000, 0b00000000, //                 
};

 unsigned char check_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000111, //              ###
  0b00000000, 0b00001111, //             ####
  0b00000000, 0b00011111, //            #####
  0b01110000, 0b00111110, //  ###      ##### 
  0b01111000, 0b01111100, //  ####    #####  
  0b01111100, 0b11111000, //  #####  #####   
  0b00011111, 0b11110000, //    #########    
  0b00001111, 0b11100000, //     #######     
  0b00000111, 0b11000000, //      #####      
  0b00000011, 0b10000000, //       ###       
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char cancel_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00111000, 0b00001110, //   ###       ### 
  0b00111100, 0b00011110, //   ####     #### 
  0b00111110, 0b00111110, //   #####   ##### 
  0b00011111, 0b01111100, //    ##### #####  
  0b00001111, 0b11111000, //     #########   
  0b00000111, 0b11110000, //      #######    
  0b00000011, 0b11100000, //       #####     
  0b00000111, 0b11110000, //      #######    
  0b00001111, 0b11111000, //     #########   
  0b00011111, 0b01111100, //    ##### #####  
  0b00111110, 0b00111110, //   #####   ##### 
  0b00111100, 0b00011110, //   ####     #### 
  0b00111000, 0b00001110, //   ###       ### 
  0b00000000, 0b00000000, //                 
};

Servo servo;

#define RST_PIN  6    // RST RC522
#define SS_PIN  7    // SDA RC522
#define UV_PIN 2
#define BUZZ_PIN 3
#define SERVO_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);  

String s1="";
int randNumber=0;

int dispenserDelay=300;

// Argentine anthem
int length = 13;  
char notes[] = "cecgfdbbbbcdc";  
int beats[] = {8, 8, 2, 8,2, 2, 4, 4, 4, 4,1,1,8};
int tempo = 80;

IPAddress ip;


void setup() {
  Serial.begin(9600);
 
  Serial.println("Starting...");  

  servo.attach(SERVO_PIN);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(UV_PIN, OUTPUT);  

  SPI.begin();         
  mfrc522.PCD_Init();   

  display.clearDisplay();
  display.setTextSize(1);  
  display.setTextColor(WHITE);
  display.drawBitmap(90, 7,  wifi1_icon16x16, 16, 16, 1);  
  display.setCursor(0, 0);
  display.println("Clean Alfajor");    
  display.setCursor(0, 10);
  display.println("Connecting to"); 
  display.setCursor(0, 20);
  display.println("WiFi..."); 
  display.display();

  randomSeed(analogRead(0));  


   // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi link failed");
    // don't continue
    while (true);
  }


  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to SSID: ");
    Serial.println(ssid);    
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection
    delay(10000);
  }
  Serial.println("Connected");
  

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
   
  display.clearDisplay();    
  display.drawBitmap(90, 7,  wifi1_icon16x16, 16, 16, 1);  
  display.setCursor(0, 0);
  display.println("Clean Alfajor");    
  display.setCursor(0, 10);
  display.println("Local IP");    
  display.setCursor(0, 20);
  display.println(ip); 
  display.display();
 

   // plays anthem
    for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2); 
  }

  delay(1000); 
  
  
  
  tone(BUZZ_PIN, 700); 
  delay(200);
  noTone(BUZZ_PIN);
                    
  servo.write(90);
  delay(1000);        

  display.clearDisplay();  
  display.setCursor(0, 0);
  display.println("Clean Alfajor");   
  display.drawBitmap(100, 7,  clock_icon16x16, 16, 16, 1);   
  display.setCursor(0, 10);
  display.println(ip); 
  display.setCursor(0, 20);
  display.println("Waiting for card");    
  display.display();

}

void loop() {
  


   if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
            // New card present
            if ( mfrc522.PICC_ReadCardSerial()) 
            {

                  tone(BUZZ_PIN, 700); 
                  delay(200);
                  noTone(BUZZ_PIN);
  
                  s1="";
                  // Send card UID
                  Serial.print(" Card:");
                  
                  
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);                                                       
                          s1 = s1 + String(mfrc522.uid.uidByte[i], HEX);
                                                                                
                  } 
                 
                  Serial.println();
                  Serial.println(s1.substring(0, 14));
                  // Ends reading
                  mfrc522.PICC_HaltA();                     
                  Serial.print("");
                  s1=String(s1); 

                display.clearDisplay();               
                display.setCursor(0, 0);
                display.println("Clean alfajor"); 
                display.drawBitmap(100, 7,  check_icon16x16, 16, 16, 1);                   
                display.setCursor(0, 10);
                display.println("Card read");    
                display.setCursor(0, 20);
                display.println(s1);    
                display.display();                    
                delay(1000);

                // Query remote server

                Serial.println("\nStarting connection to server...");  
                if (client.connect(server, 80)) {
                  Serial.println("connected to server");
                  // Make a HTTP request:
                  client.println("GET /server/alfajor.php?uid=" +s1+ " HTTP/1.1");
                  client.println("Host: YourServerHere.com");
                  client.println("Connection: close");
                  client.println();
                }

                waitServer();

                String serverAnswer="";   
                           
                while (client.available()) {
                  char c = client.read();
                  serverAnswer=serverAnswer+c;
                  Serial.write(c);
                }

              Serial.println("Answer is:");
              const String yourName=getValue(serverAnswer,'#',1);
              Serial.println(yourName);
                  
                if (yourName!="N/A" and yourName!=""){
                    
                    tone(BUZZ_PIN, 700); 
                    delay(100);
                    noTone(BUZZ_PIN); 

                    // UV Lamp

                    display.clearDisplay();              
                    display.setCursor(0, 0);
                    display.println("Clean alfajor");    
                    display.drawBitmap(100, 8,  lock_open_icon16x16, 16, 16, 1);  
                    display.setCursor(0, 10);
                    display.println(yourName);    
                    display.setCursor(0, 20);
                    display.println("UV sterilizing");    
                    display.display();
    
                    Serial.println("Capacitor on");      
                    digitalWrite(UV_PIN , HIGH);   
                    delay(2500);
                    digitalWrite(UV_PIN, LOW);    
                    delay(2500);                                                     

                    tone(BUZZ_PIN, 900); 
                    delay(100);
                    noTone(BUZZ_PIN);
                    tone(BUZZ_PIN, 900); 
                    delay(50);
                    noTone(BUZZ_PIN);
                  
                    Serial.println("Turn on");      
                    digitalWrite(UV_PIN , HIGH);   
                    delay(1000);     
                    digitalWrite(UV_PIN, LOW);   
                    delay(5000);   
                
                    Serial.println("Turn off");
                    digitalWrite(UV_PIN, HIGH);   
                    delay(200);     
                    digitalWrite(UV_PIN, LOW);   
                    delay(200);        

                    tone(BUZZ_PIN, 900); 
                    delay(100);
                    noTone(BUZZ_PIN);
                    tone(BUZZ_PIN, 900); 
                    delay(50);
                    noTone(BUZZ_PIN);
                  
                    display.clearDisplay();    
                    display.setCursor(0, 0);
                    display.println("Clean alfajor");    
                    display.drawBitmap(100, 8,  lock_open_icon16x16, 16, 16, 1);               
                    display.setCursor(0, 10);
                    display.println("Alfajor");    
                    display.setCursor(0, 20);
                    display.println("clean and ready");    
                    display.display();
                     
                        
                    servo.write(0);
                    delay(dispenserDelay);
                    servo.write(90);     

                    delay(5000);
                    }
                    else

                    {

                            tone(BUZZ_PIN, 200); 
                            delay(200);
                            tone(BUZZ_PIN, 100); 
                            delay(200);
                            noTone(BUZZ_PIN); 
        
                            display.clearDisplay();
                            display.setCursor(0, 0);
                            display.println("Clean alfajor");  
                            display.drawBitmap(100, 7,  cancel_icon16x16, 16, 16, 1);                                                           
                            display.setCursor(0, 10);
                            display.println("Your card is");    
                            display.setCursor(0, 20);
                            display.println("not authorized");    
                            display.display();

  
                            delay(4000);               
                    }
                                                                                             

                 
                  
                display.clearDisplay();                
                display.setCursor(0, 0);
                display.println("Clean alfajor");  
                display.drawBitmap(100, 7,  clock_icon16x16, 16, 16, 1);  
                display.setCursor(0, 10);
                display.println(ip);  
                display.setCursor(0, 20);
                display.println("Waiting for card");    
                display.display();                    
  
            }  
                
  }// card present 
  delay(1000);

}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(BUZZ_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZ_PIN, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void waitServer() {
  int timeout = 0;
  while (!client.available() && timeout < 5000) {
    timeout++;
    delay(1);
    if (timeout >= 5000) {
      Serial.println(F("Error, max timeout reached"));
      break;
    }
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
