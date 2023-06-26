#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

using namespace std;

#define SYNC_1_TOKEN 42 //0x00101010b;
#define SYNC_2_TOKEN 43 //0x00101011b;
#define TOKEN_SIZE 6
#define EXT_SIZE 2
#define FRAME_PADS 10
#define PHASE_PADS 0


int addToBits(uint8_t* bits, int to, int value, int size);
void parseEma(string str, uint8_t* bits);
void frameToCodes(uint8_t* emaArray, uint8_t* bits);
int getToken(int code);
