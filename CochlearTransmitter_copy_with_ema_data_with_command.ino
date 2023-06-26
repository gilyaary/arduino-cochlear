#include <SPI.h>
#include "CommandParser.h"

SPISettings settingsA(1000000, MSBFIRST, SPI_MODE1);
int bytes_size = 64;

uint8_t bytes [] = {
  0b10101011,
  0b01101011,
  0b11111011,
  0b11110110,
  0b10101111,
  0b01111101,
  0b11111011,
  0b10110110,
  0b00000001,
  0b01010110,
  0b11010110,
  0b11110111,
  0b01101101,
  0b01011110,
  0b11110110,
  0b11101101,
  0b11101100,
  0b00000010,
  0b10101101,
  0b10110110,
  0b11101110,
  0b11011101,
  0b01011110,
  0b11111111,
  0b01111101,
  0b11101110,
  0b00000101,
  0b01010111,
  0b11011011,
  0b11011111,
  0b10110101,
  0b01111011,
  0b11011111,
  0b11110111,
  0b10110000,
  0b00001010,
  0b10101111,
  0b11110111,
  0b10111011,
  0b01101010,
  0b11110111,
  0b11011111,
  0b01101011,
  0b01100000,
  0b00010101,
  0b01101111,
  0b01111111,
  0b01110110,
  0b11010101,
  0b11101111,
  0b01111110,
  0b11111110,
  0b11000000
};

uint8_t *data_array = (uint8_t*)malloc(sizeof(bytes));
int data_array_size = sizeof(bytes);

int CTRL_TX_PIN = 10;

void setup() {

  pinMode(CTRL_TX_PIN, OUTPUT);
  digitalWrite(CTRL_TX_PIN, 1);  
  
  for (int i=0; i<bytes_size; i++) {
    data_array[i] = bytes[i];
  }

  Serial.begin(115200);
  SPI.begin();
}

// the loop routine runs over and over again forever:
void loop() {
    int FRAME_SIZE = 10 * TOKEN_SIZE + 2 * EXT_SIZE + FRAME_PADS + PHASE_PADS;  
                
    
    if (Serial.available() > 0) {
        String lastCommand = Serial.readStringUntil('\n');
        //String lastCommand = "^5,28,140,64;6,28,145,64;7,28,150,64;8,28,160,64;31,31,155,64;";
        Serial.println(lastCommand);
        if (lastCommand.c_str()[0] != '^') {
            return;
        }

        int size = lastCommand.length();
        int currentStartIndex = 0;
        int frameCount = 0;
        for(int i=1; i<size; i++) {
            if (lastCommand.c_str()[i] == ';' || i == size-1){
                frameCount += 1;
            } 
        }
        int bytesPerFrame = FRAME_SIZE/8;
        free (data_array);
        data_array_size = bytesPerFrame*frameCount;
        data_array = (uint8_t*)malloc(data_array_size);   
        int frameIndex = 0;

        Serial.println(size);
        Serial.println(data_array_size);
        Serial.println(frameCount);
        Serial.println("#####################");

        for(int i=1; i<size; i++) {
            if (lastCommand.c_str()[i] == ';' || i == size-1) {
                String frameString = lastCommand.substring(currentStartIndex, i-currentStartIndex);
                //printf("%s\n", frameString.c_str());
                currentStartIndex = i+1;
                uint8_t bits [FRAME_SIZE];
                string str = frameString.c_str();
                parseEma(str, bits);
                int byteIndex = frameIndex * bytesPerFrame;
                for(int i=FRAME_SIZE-1; i>=7; i-=8) {
                    data_array[byteIndex] = 0;
                    for(int j=0; j<8; j++) {
                        Serial.print(bits[i-j]);
                        if (bits[i-j]) {
                          data_array[byteIndex] |= 128 >>j;
                        }
                    }
                    Serial.println();
                    Serial.print(data_array[byteIndex]);
                    Serial.println();
                    byteIndex += 1;
                }
            }
            frameIndex += 1;
        }
        
        Serial.println(data_array_size);
        Serial.flush();
    }    
    //end serial read

    uint8_t data [data_array_size] ;  
    for (int i=0; i<data_array_size; i++) {
      data[i] = data_array[i];
    }
    
    SPI.beginTransaction(settingsA);
    digitalWrite(CTRL_TX_PIN, 0);
    SPI.writeBytes(data, data_array_size);
    SPI.endTransaction();
    digitalWrite(CTRL_TX_PIN, 1);
    //delayMicroseconds(10);
    
}

//string str = "^5,28,140,64;6,28,145,64;7,28,150,64;8,28,160,64;31,31,155,64;";
//string str = "5,28,140,64;";

    
/*
10101011111010111111011111110110101011111011101101101111101101100000000000
10101010111111011111011111110110101011111011110110101111101101100000000000
10101011101111011111011110110110101011111011111101101111101101100000000000
10101010110110110111011110110110101011111011101111110111111101100000000000
10101011110111110111110111110110101011111101111101111101111101100000000000
*/
//61,45,5
//171,235,247
