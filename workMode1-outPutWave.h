#include <reg51.h>                              // special function register declarations
#include<absacc.h>
#include<intrins.h>
#include<math.h>

#define SIN_BASE_ADDRESS 0x1000 //0x10FF
#define TRI_BASE_ADDRESS 0x1100 //0x11FF
#define SQU_BASE_ADDRESS 0x1200 //0x12FF
#define TEE_BASE_ADDRESS 0x1300 //0x13FF

void waveInit();
