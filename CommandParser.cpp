#include "CommandParser.h"

int getToken(int code) {
    //000 -> 101110 
    //001 -> 101111
    //010 -> 110111
    //011 -> 101101
    //100 -> 111110
    //101 -> 110110
    //110 -> 111011
    //111 -> 111101
    switch (code){
        case 0: return 46;
        case 1: return 47;
        case 2: return 55;
        case 3: return 45;
        case 4: return 62;
        case 5: return 54;
        case 6: return 59;
        case 7: return 61;
    }
    return 0; //code | (code<<3);
}

void frameToCodes(uint8_t* emaArray, uint8_t* bits) {
    //printf("E: %d\n", emaArray[0]); //E
    //printf("M: %d\n", emaArray[1]); //M
    //printf("A: %d\n", emaArray[2]); //A
    //printf("%d\n", emaArray[3]); //PADS

    //18 bit code
    //5bit electrode MSB, 5bit mode, 8 bit amplitude LSB
    int _18_bit_codes = (emaArray[0]<<13) | (emaArray[1]<<8) | emaArray[2];


    //printf("%d\n\n", _18_bit_codes);
    for (int i=17; i>=0; i--) {
         //_18_bit_codes >> i)
         //printf("%d", (_18_bit_codes>>i & 1));   
    }
    //printf("\n");

    //long tokensValue = 0;
    int FRAME_SIZE = 10 * TOKEN_SIZE + 2 * EXT_SIZE + FRAME_PADS + PHASE_PADS;  
    for (int i=0; i<FRAME_SIZE; i++){
        bits[i] = 0;
    }
    int to = FRAME_SIZE-1;
    
    //Phase 1
    to = addToBits(bits, to, SYNC_1_TOKEN, TOKEN_SIZE);
    int phase1CodeTotal = 0;
    // for(int i=0; i<9; i+=3) {
    //     //replace a 3 bitcode for 6 bit toen
    //     int code = (_18_bit_codes>>i) & 7;
    //     phase1CodeTotal += code;
    //     //printf("Code: %d\n", code);
    //     int token = getToken(code);
    //     to = addToBits(bits, to, token, TOKEN_SIZE);
    // }
    for(int i=0; i<9; i+=3) {
        //replace a 3 bitcode for 6 bit toen
        int code = (_18_bit_codes>>(18-i-3)) & 7;
        phase1CodeTotal += code;
        //printf("Code: %d\n", code);
        int token = getToken(code);
        to = addToBits(bits, to, token, TOKEN_SIZE);
    }
    int phase1ErrorCode = phase1CodeTotal%2 == 0 ? 45 : 61;
    
    to = addToBits(bits, to, phase1ErrorCode, TOKEN_SIZE);
    to = addToBits(bits, to, 2, EXT_SIZE); //phase ext 1,0
    to = addToBits(bits, to, 0, PHASE_PADS); //4 pads
    
    //Phase 2
    to = addToBits(bits, to, SYNC_2_TOKEN, TOKEN_SIZE);
    int phase2CodeTotal = 0;
    for(int i=9; i<18; i+=3) {
        //replace a 3 bitcode for 6 bit toen
        int code = (_18_bit_codes>>(18-i-3)) & 7;
        phase2CodeTotal += code;
        //printf("Code: %d\n", code);
        int token = getToken(code);
        to = addToBits(bits, to, token, TOKEN_SIZE); //LSB
    }
    int phase2ErrorCode = phase2CodeTotal%2 == 0 ? 45 : 61;
    to = addToBits(bits, to, phase2ErrorCode, 6);
    to = addToBits(bits, to, 2, EXT_SIZE); //phase ext 1,0
    to = addToBits(bits, to, 0, FRAME_PADS); //4 pads
    int totalBitCount = to;
}

int addToBits(uint8_t* bits, int to, int value, int size) {
    for (int i=0; i<size; i++) {
        int bit = (value >> i) & 1;
        //printf("Bit: %d\n", bit);
        int index = to - size + i + 1;
        //printf("Index: %d\n", index);
        bits[index] = bit;
    }
    return to - size;
}


void parseEma(string frameString, uint8_t* bits) {
    int fieldStartIndex = 0;
    uint8_t emaArray [4];
    int fieldIndex = 0; 
    for (int j=0; j<frameString.length(); j++) {
        if (frameString.c_str()[j] == ',' || j == frameString.length()-1) {
            string field = frameString.substr(fieldStartIndex, j-fieldStartIndex+1);
            //printf("%s\n", field.c_str());
            fieldStartIndex = j+1;
            emaArray[fieldIndex++] = atoi(field.c_str());
        }
    }
    //TODO: take emaArray and convert to bits. 
    //These bits will be seperated into 3 bit codes that will later be translated to tokens
    frameToCodes(emaArray, bits);
}

void printBits(uint8_t* bits, int len) {
    for (int i=len-1; i>=0; i--) {
        if(len-i == 7||len-i == 13||len-i == 19||len-i == 25||len-i == 31||len-i == 33||len-i == 39||len-i == 45||len-i == 51||len-i == 57||len-i == 63||len-i == 65) {
            printf(" ");    
        }
        printf("%d", bits[i]);
    }
    /*
    to create bytes we need the sync token, the 3 data tokens, the error token and the phase extender + padding
    SYNC DT1 DT2 DT3 ERR EXT PAD
    */
    printf("\n");
}

int main()
{
    string str = "5,28,140,64;6,28,145,64;7,28,150,64;8,28,155,64;31,31,255,64;";
    int size = str.length();
    int currentStartIndex = 0;
    for(int i=0; i<size; i++) {
        if (str.c_str()[i] == ';' || i == size-1) {
            string frameString = str.substr(currentStartIndex, i-currentStartIndex);
            //printf("%s\n", frameString.c_str());
            currentStartIndex = i+1;
            int FRAME_SIZE = 10 * TOKEN_SIZE + 2 * EXT_SIZE + FRAME_PADS + PHASE_PADS;  
            uint8_t bits [FRAME_SIZE];
            parseEma(frameString, bits);
            printBits(bits, FRAME_SIZE);
            
        }
    }
    return 0;
}
/*



10101011111010111111011111110110101011111011101101101111101101100000000000
10101010111111011111011111110110101011111011110110101111101101100000000000
10101011101111011111011110110110101011111011111101101111101101100000000000
10101010110110110111011110110110101011111011101111110111111101100000000000
10101011110111110111110111110110101011111101111101111101111101100000000000
*/