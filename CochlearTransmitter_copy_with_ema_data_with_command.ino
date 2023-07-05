#include <SPI.h>
#include "CommandParser.h"

SPISettings settingsA(1000000, MSBFIRST, SPI_MODE1);
int bytes_size = 71;
uint8_t* xxx = (uint8_t*)malloc(1024); 

//^20,28,150,64;20,28,150,64;20,28,150,64;20,28,150,64;20,28,150,64;20,28,150,64;20,28,150,64;20,28,150,64;20,28,150,64;20,28,150,64;

//HIGER TONE
// uint8_t bytes [] = {
//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000,
  
//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000,

//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000,

//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000,

//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000,

//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000,

//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000,

//   0b10101011,
//   0b01101011,
//   0b11111011,
//   0b11110110,
//   0b10101111,
//   0b01111101,
//   0b11111011,
//   0b10110110,
//   0b00000000
// };

//LOWER TONE
uint8_t bytes [] = {
  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000,

  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000,

  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000,

  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000,

  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000,

  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000,

  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000,

  0b10101011,
  0b01101011,
  0b01111011,
  0b10110110,
  0b10101111,
  0b01111011,
  0b01110110,
  0b11110110,
  0b00000000

};


int data_array_size = 71;// sizeof(bytes);

int CTRL_TX_PIN = 10;

void setup() {

  pinMode(CTRL_TX_PIN, OUTPUT);
  digitalWrite(CTRL_TX_PIN, 1);  
  Serial.begin(115200);
  
  for (int i=0; i<bytes_size; i++) {
    xxx[i] = bytes[i];
    Serial.print(xxx[i]);
    Serial.println();
  }

  
  SPI.begin();
}

bool newData = false;

// the loop routine runs over and over again forever:
void loop() {
    int FRAME_SIZE = 10 * TOKEN_SIZE + 2 * EXT_SIZE + FRAME_PADS + PHASE_PADS;  
    
                
    
    if (Serial.available() > 0) {
        free(xxx);
        xxx = (uint8_t*)malloc(1024);
        xxx[0] = 1;
        xxx[1] = 2;
        xxx[2] = 3;
        xxx[3] = 4;
        xxx[4] = 5;
        xxx[5] = 6;
        xxx[6] = 7;
        xxx[7] = 8;
        newData = true;
        String lastCommand = Serial.readStringUntil('\n');
        //String lastCommand = "^5,28,140,64;6,28,145,64;7,28,150,64;8,28,160,64;31,31,155,64;";
        Serial.println(lastCommand);
        if (lastCommand.c_str()[0] != '^') {
            return;
        }

        int size = lastCommand.length();
        int currentStartIndex = 1;
        int frameCount = 0;
        for(int i=1; i<size; i++) {
            if (lastCommand.c_str()[i] == ';' || i == size-1){
                frameCount += 1;
            } 
        }
        int bytesPerFrame = FRAME_SIZE/8;
        data_array_size = bytesPerFrame*frameCount;
        int frameIndex = 0;

        Serial.println(size);
        Serial.println(data_array_size);
        Serial.println(frameCount);
        Serial.println("#####################");

        for(int i=1; i<size; i++) {
            //Serial.print(i);
            //Serial.println();

            if (lastCommand.c_str()[i] == ';' || i == size-1) {
                
                //Serial.print("FOUND NEW FRAME END TOKEN");
                //Serial.println();
                String frameString = lastCommand.substring(currentStartIndex, i);
                currentStartIndex = i+1;
                uint8_t bits [FRAME_SIZE];

                string str = frameString.c_str();
                //Serial.println(String(str.c_str()));
                
                parseEma(str, bits);
                int c =0;
                
                // for(int p=FRAME_SIZE-1; p>=0; p--) {
                //     if(c%6 == 0) {
                //         Serial.println();        
                //     }
                //     Serial.print(bits[p]);
                //     c+=1;
                // }
                
                for(int p=FRAME_SIZE-1; p>=0; p--) {
                    if(c%8 == 0) {
                        //Serial.println();        
                    }
                    Serial.print(bits[p]);
                    c+=1;
                }

                Serial.println();
                
                int byteIndex = 0;//frameIndex * bytesPerFrame;
                for(int ii=FRAME_SIZE-1; ii>=7; ii-=8) {
                    //Serial.print("FRAME_");
                    //Serial.print(ii);
                    //Serial.println();
                    xxx[byteIndex] = 0;
                    for(int j=0; j<8; j++) {
                        if (bits[ii-j]) {
                            xxx[byteIndex] |= 128 >>j;
                        }
                        //Serial.print(bits[ii-j]);
                    }
                    
                    Serial.println();
                    Serial.print(byteIndex);
                    Serial.print(": ");
                    Serial.print(xxx[byteIndex]);
                    Serial.println();

                    byteIndex += 1;
                }
                Serial.print("----FRAME_END");
                Serial.println();

            }
            frameIndex += 1;
        } //frames to bytes loop

        // for(int k=0; k<data_array_size; k++) {
        //     Serial.print(data_array[k]);
        //     Serial.print("-");
        // }
        
        //Serial.println(data_array_size);
        Serial.flush();
    }    
    //end serial read

    if(data_array_size > 0) {

      uint8_t data [data_array_size] ;  
      for (int i=0; i<data_array_size; i++) {
          data[i] = xxx[i];
          if (newData) {
            // Serial.print(i);
            // Serial.print(":");
            Serial.print(xxx[i]);
            // Serial.print(i);
            Serial.print(",");
          }
          
      }
      if (newData) {
        Serial.println();
        newData = false;
      }
      
      
      SPI.beginTransaction(settingsA);
      digitalWrite(CTRL_TX_PIN, 0);
      SPI.writeBytes(data, data_array_size);
      SPI.endTransaction();
      digitalWrite(CTRL_TX_PIN, 1);
      delayMicroseconds(2);
    }
    
}

//string str = "^5,28,140,64;6,28,145,64;7,28,150,64;8,28,160,64;31,31,155,64;";
//string str = "5,28,140,64;";
//try also str="^20,28,150,64;21,28,157,64;22,28,164,64;5,28,140,64;"
    
/*
10101011111010111111011111110110101011111011101101101111101101100000000000
10101010111111011111011111110110101011111011110110101111101101100000000000
10101011101111011111011110110110101011111011111101101111101101100000000000
10101010110110110111011110110110101011111011101111110111111101100000000000
10101011110111110111110111110110101011111101111101111101111101100000000000
*/
//61,45,5
//171,235,247
