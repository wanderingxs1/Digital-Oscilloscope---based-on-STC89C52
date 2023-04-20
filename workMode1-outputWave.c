#include<workMode1-outputWave.h>

unsigned char waveMode=0;//����ģʽ1���������Ĭ����Ч0
//1����2���ǲ�3����4��ݲ�
unsigned char WAVE_VALUE=0;//Ҫ������εĵ�ǰֵ

//��ʼ��4�ֲ��Σ�ÿ��1���ڣ�256��������
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
