#include<workMode1-outputWave.h>

unsigned char waveMode=0;//工作模式1下输出波形默认无效0
//1正弦2三角波3方波4锯齿波
unsigned char WAVE_VALUE=0;//要输出波形的当前值

//初始化4种波形，每种1周期，256个采样点
void waveInit()
{
    unsigned int address=0;	
    unsigned int i=0;
    //sin
    i=0;
    address=SIN_BASE_ADDRESS;
    for(;address<=0x10FF;address++,i++){
        XBYTE[address]=floor(64*(sin(3.14*i/128)+1))+40;
    }
    //triangular
    i=0;
    address=TRI_BASE_ADDRESS;
    for(;address<=0x1179;address++,i++){
        XBYTE[address]=20+i;
    }
    i=0;
    address=0x117A;
    for(;address<=0x11F3;address++,i++){
        XBYTE[address]=141-i;
    }
    //square
    address=SQU_BASE_ADDRESS;
    for(;address<=0x1279;address++){
        XBYTE[address]=150;
    }
    address=0x127A;
    for(;address<=0x12F3;address++){
        XBYTE[address]=30;
    }
    //teeth
    i=0;
    address=TEE_BASE_ADDRESS;
    for(;address<=0x1379;address++,i++){
        XBYTE[address]=i+20;
    }
}
