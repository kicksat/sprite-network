/* 
* Exchanges packets between Sprites
* and downlinks count of recieved packets
*
* Author: Tane Tatum
*
* Last Updated: 09/12/2018
*/


// Global variables
unsigned char address = 0x01;        // Address of this Sprite
#define SpriteNetTXProb 200          // Probablility Spritenet will transmit
#define DownlinkProb 25              // Probability of downlink
#define numSats 4                    // Total number of Sprites
unsigned char sentIDs[3];            // Stores IDs for previously repeated packets
unsigned int packetCounter[numSats]; // Records number of packets recieved from each Sprite


// Included Libraries
#include "hardwareSerial.h"
#include "SpriteNet.h"
#include "SpriteRadio.h"

STORAGE nvMem;   // Create object for nonvolatile storage

unsigned char prn0[64] = {
  0b00000001, 0b01011110, 0b11010100, 0b01100001, 0b00001011, 0b11110011, 0b00110001, 0b01011100,
  0b01100110, 0b10010010, 0b01011011, 0b00101010, 0b11100000, 0b10100011, 0b00000000, 0b11100001,
  0b10111011, 0b10011111, 0b00110001, 0b11001111, 0b11110111, 0b11000000, 0b10110010, 0b01110101,
  0b10101010, 0b10100111, 0b10100101, 0b00010010, 0b00001111, 0b01011011, 0b00000010, 0b00111101,
  0b01001110, 0b01100000, 0b10001110, 0b00010111, 0b00110100, 0b10000101, 0b01100001, 0b01000101,
  0b00000110, 0b10100010, 0b00110110, 0b00101111, 0b10101001, 0b00011111, 0b11010111, 0b11111101,
  0b10011101, 0b01001000, 0b00011001, 0b00011000, 0b10101111, 0b00110110, 0b10010011, 0b00000000,
  0b00010000, 0b10000101, 0b00101000, 0b00011101, 0b01011100, 0b10101111, 0b01100100, 0b11011010
};

unsigned char prn1[64] = {
  0b11111101, 0b00111110, 0b01110111, 0b11010101, 0b00100101, 0b11101111, 0b00101100, 0b01101001,
  0b00101010, 0b11101001, 0b00111100, 0b11000100, 0b00000111, 0b10010011, 0b11000101, 0b00000111,
  0b00110111, 0b00011111, 0b01111011, 0b11010001, 0b10111010, 0b00000111, 0b10010000, 0b00110111,
  0b11011111, 0b01011010, 0b11101101, 0b11001000, 0b10001100, 0b01101001, 0b10010111, 0b00101001,
  0b10101100, 0b11011001, 0b11010110, 0b00011010, 0b11010110, 0b10101000, 0b00000101, 0b11010011,
  0b01101010, 0b11001011, 0b11010110, 0b01010010, 0b00111111, 0b11100111, 0b10000010, 0b10000110,
  0b01101110, 0b10011010, 0b01100101, 0b10100110, 0b00101110, 0b01010100, 0b11110100, 0b01111010,
  0b11001011, 0b00101110, 0b01100011, 0b10111111, 0b01010100, 0b11000100, 0b11010100, 0b01010100
};
SpriteRadio spriteRadio(prn0, prn1);
SpriteNet spriteNet(address);         // Initialize SpriteNet


/////////// SETUP FUNCTION ///////////
void setup() {
  spriteNet.txInit();          // Set SpriteNet radio settings
  
  pinMode(LED, OUTPUT);        // Set LED pin to output
  pinMode(A0, INPUT);          // Set floating pin to input
  randomSeed(analogRead(A0));  // Seed random number generator
  Serial.begin(4800);          // Start Serial
  
  // Retrieve number of packets recieved from nonvolatile memory
  unsigned char byte_packetCounter[sizeof(packetCounter)];
  nvMem.read(byte_packetCounter, INFOMEM_SECTION_D, 0x00, sizeof(packetCounter));  // Read Old value
  byte2int(byte_packetCounter, packetCounter);  
  nvMem.write(0, INFOMEM_SECTION_D, 0x00, sizeof(packetCounter));                  // Reset persitent value to 0
  delay(500);    // Allow time to reset and erase nonvolatile memory
  nvMem.write(byte_packetCounter, INFOMEM_SECTION_D, 0x00, sizeof(packetCounter));                  // Reset persitent value to 0
  // Print Packet Counter
  Serial.print("Packet Counter: ");
      for(int i = 0; i < numSats; i++){
        Serial.print(packetCounter[i]);
        Serial.print(" ");
      }
      Serial.println(" ");
}


/////////// LOOP FUNCTION ///////////
void loop() {
  // Check random number comparator for transmitting
  long randNumber = random(1000);
  if(randNumber < SpriteNetTXProb){                // compare random number against probability threshold
    // Transmit random data
    unsigned char txData[1];
    txData[0] = randomByte();  // Data to transmit
    Packet txPacket;
    Packet *p_txPacket = &txPacket;
    txPacket.destination = 0x00;
    txPacket.origin = address;
    txPacket.sender = address;
    txPacket.data = txData;
    txPacket.dataLength = 1;
    spriteNet.sendPacket(p_txPacket);
    Serial.print("Transmit ");
    spriteNet.printPacket(&txPacket);
    Serial.print("TX Data:  ");
    Serial.println(txPacket.data[0]);
    blink(2, 200);
  }
  
  // Listen for packet  
  Packet rxPacket;
  unsigned char rxData[64];
  rxPacket.data = rxData; 
  Packet *p_rxPacket = &rxPacket;
  spriteNet.listen(p_rxPacket);
  if(rxPacket.dataLength > 0){            // If packet recieved
    Serial.print("Recieved ");
    spriteNet.printPacket(p_rxPacket);
    Serial.print("RX Data: ");
    Serial.println(rxPacket.data[0]);
    blink(4,50);
    
    // Check if recieved packet is new
    bool duplicate = IDmatch(rxPacket.packetID, sentIDs);       // True if ID of current packet is in sentIDs array
    if(!duplicate){
      updateIDs(rxPacket.packetID, sentIDs);                    // Add current packet to list of sentIDs
      unsigned char satIndex = rxPacket.origin-1;               // Index of satellite recieved
      packetCounter[satIndex] = packetCounter[satIndex]++;      // Update packet counter
      unsigned char byte_packetCounter[sizeof(packetCounter)];
      int2byte(packetCounter, byte_packetCounter);              //Convert integer array to byte array
      //nvMem.write(byte_packetCounter, INFOMEM_SECTION_D, 0x00 , sizeof(packetCounter));  // Update nonvolatile memory
      nvMem.write(&byte_packetCounter[satIndex*2], INFOMEM_SECTION_D, satIndex*2 , 2);  // Update nonvolatile memory
      Serial.print("Packet Counter: ");
      for(int i = 0; i < numSats; i++){
        Serial.print(packetCounter[i]);
        Serial.print(" ");
      }
      Serial.println(" ");
    
      //Repeat packet if needed
      if((rxPacket.destination != address) && (rxPacket.origin != address)){
        spriteNet.repeatPacket(p_rxPacket);
        blink(1, 300);
      
        //Only necessary for accurate serial printout
        Packet repPacket = rxPacket;
        repPacket.sender = address;
        Packet *p_repPacket = &repPacket;
        Serial.print("Repeated ");
        spriteNet.printPacket(p_repPacket);
      }
    }  
  }
  
  // Downlink
  randNumber = random(1000);
  if(randNumber < DownlinkProb){                // compare random number against probability threshold
  
    // Construct char array from packet counter
    String downlinkString;
    downlinkString = String("A: ");
    downlinkString = String(downlinkString + address);        // Add sprite address to downlink message
    downlinkString = String(downlinkString + " Data: ");
    for(int i = 0; i < numSats; i++){
      downlinkString = String(downlinkString + " " + packetCounter[i]);
    }
    downlinkString = String(downlinkString + "\n");
    unsigned int downlinkLength = downlinkString.length();    // Get length of string
    char charArray[downlinkLength];                          
    downlinkString.toCharArray(charArray, downlinkLength+1);  // Convert string to char array
    Serial.print("Downlinking Data: \n   ");
    Serial.println(charArray);
    
    // Transmit char array
    spriteRadio.txInit();                                // Switch to SpriteRadio Settings
    digitalWrite(LED,HIGH);
    spriteRadio.transmit(charArray, downlinkLength);     // Downlink Data
    spriteNet.txInit();                                  // Return to spriteNet settings
    digitalWrite(LED,LOW);
  }
  Serial.println(" ");
  blink(1,5);          // Blink to indicate Sprite is alive
}




///////////////////////////////////


//Generate and return random byte 
unsigned char randomByte(){
  long randLong = random(255);
  unsigned char randByte;
  randByte = (int)((randLong & 0xFF));
  return randByte;
}


// Check if packet ID is in sentIDs
bool IDmatch(unsigned char ID, unsigned char *sentIDs){
  unsigned char matches = 0;
  for(int i = 0; i < 3; i++){
    if(ID == sentIDs[0]) matches++;
  }
  if(matches > 0) return true;
  else return false;
}

// Add new ID to the stored list
void updateIDs(unsigned char ID, unsigned char *sentIDs){
  for(int i = 0; i < 2; i++){
    sentIDs[i+1] = sentIDs[i];
  }
  sentIDs[0] = ID;
}


// Blinks LED a specified number of times
void blink(int num){
  for(int i = 0; i < num; i++){
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED,LOW);
    delay(100);
  }
}

// Blinks LED a specified number of times for given time duration
void blink(int num, int duration){
  for(int i = 0; i < num; i++){
    digitalWrite(LED, HIGH);
    delay(duration);
    digitalWrite(LED,LOW);
    delay(duration);
  }
}

// Convert int array to byte array
void int2byte(unsigned int integer[], unsigned char byteArray[]){
  for(int i = 0; i < numSats; i++){
    unsigned char j = i*2;
    byteArray[j] =  integer[i] & 0x00ff;
    byteArray[j+1] = (integer[i] & 0xff00) >> 8;
  }
}

// Convert int array to byte array
void int2byte(unsigned int integer[], char byteArray[]){
  for(int i = 0; i < numSats; i++){
    unsigned char j = i*2;
    byteArray[j] =  integer[i] & 0x00ff;
    byteArray[j+1] = (integer[i] & 0xff00) >> 8;
  }
}

// Convert byte array to int array
void byte2int(unsigned char byteArray[], unsigned int intArray[]){
  for(int i = 0; i < numSats; i++){
    unsigned char j = 2*i;
    intArray[i] = (byteArray[j]) | (byteArray[j+1] << 8);
  }
}
