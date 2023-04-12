#ifndef __adc_H__
#define __adc_H__

#include <reg51.h>                              // special function register declarations
#include<absacc.h>
#include<intrins.h>

sfr ADC_CONTR=0xBC;	 //ADC���ƼĴ���
sfr ADC_RES =0xBD;	//ADC����Ĵ����߰�λ
sfr ADC_RESL=0xBE;	//ADC����Ĵ�����2λ
sfr P1ASF=0x9D;		//P1����;�Ĵ���

//����Ҫʹ�ܵ�оƬ��ַ
#define CH1 0x2000;
#define CH2 0x4000;

//����ADC�ļ�������״̬
#define ADC_INIT 0x83	 //��Ҳ��ADC_FLAG��0ʱ���״̬
#define ADC_START 0x8B
#define ADC_FLAG 0x10		//��������ADC�жϱ�־λ


void adc_init();
void adc_start();
void dac_work(int channalSelect,char value);

void delay(int delayTime);//��λ��ʱΪ5ms��ֻ���ʼʹ����

#endif