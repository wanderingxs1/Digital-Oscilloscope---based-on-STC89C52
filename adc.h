#ifndef __adc_H__
#define __adc_H__

#include <reg51.h>                              // special function register declarations
#include<absacc.h>
#include<intrins.h>

#define  DAC_1    XBYTE[0x2000]
#define  DAC_2    XBYTE[0x4000]

#define  ADC_0    XBYTE[0x6000]
#define  ADC_1    XBYTE[0x6001]
#define  ADC_2    XBYTE[0x6002]
#define  ADC_3    XBYTE[0x6003]
#define  ADC_4    XBYTE[0x6004]
#define  ADC_5    XBYTE[0x6005]
#define  ADC_6    XBYTE[0x6006]
#define  ADC_7    XBYTE[0x6007]

#define ADC_POWER 0x80
#define ADC_FLAG 0x10
#define ADC_START 0x08
#define ADC_SPEEDLL 0x00
#define ADC_SPEEDL 0x20
#define ADC_SPEEDH 0x40
#define ADC_SPEEDHH 0x60
//#define AUXR1 0xA2
#define IE 0xA8
#define IPH 0xB7

sfr ADC_CONTR=0xBC;
sfr ADC_RES=0xBD;
sfr ADC_RESL=0xBE;
sfr P1ASF=0x9D;

void adc_init();
//void adc_work();//中断函数可以不用声明

void delay(int delayTime);//延时为机器周期个数

#endif