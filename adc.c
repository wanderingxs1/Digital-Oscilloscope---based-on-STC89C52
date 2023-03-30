#include<adc.h>

extern int adc_work_count;
extern char ADC_RESULT;
extern int adAddress;
extern int adcount;

bit testa;
bit testb;
bit testc;


void adc_init()
{
    P1ASF=0x08;     //确定p13是a-d功能,输入
    ADC_CONTR=0x0CB;//见ipad笔记
    //AUXR1=0x04;//AUXR1无法被赋值，它默认为0，只能使得高8位在ADS_RES里边，低2位在ADC_RESL的低2位（这两位舍弃）
    ADC_RES=0x00;
    ADC_RESL=0x00;

    //配置中断
    EADC=1;//AD中断允许
    IP=0x24;//AD高优先,外部中断1次高,定时器最低
    delay(2);
}

void adc_work() interrupt 5 using 1		//实时性强，采到一个就输出。但是存储是每4ms一次。此时adcount>=16.
{
    //清零ADC中断控制位
    ADC_CONTR=ADC_CONTR&0xEF;
    //保护P25-P27,并置位相应管脚发出S0片选信号,片选存储器
	testa=P2^5;
    testb=P2^6;
    testc=P2^7;
    P2=P2|0x20;
	P2=P2&0xBF;
	P2=P2&0x7F;
	//进行adc值的存储（外部RAM0x6000-0x7000),最多只能存这么多
	if(adcount>=16&&adAddress<0x7000)
	{
	   XBYTE[adAddress]=ADC_RES;
	   adAddress++;
	}

    //进行ADC值向DAC的转换

    //问题:ADC转换值是1024个阶,DAC转换值是256个阶,要进行转换,舍去最低两位
	ADC_RESULT=ADC_RES;
	ADC_RES=0x00;
	ADC_RESL=0x00;

    //进行DAC赋值
    //使能S1 DAC芯片
	P2=P2&0xEF;
	P2=P2|0x40;
	P2=P2&0x7F;
    //写选通
    P3=P3&0xBF;
    //给P0口赋值
    P0=ADC_RESULT;
	delay(10);
    //完成端口保护
	if(testa)
		P2=P2|0x20;
	else
		P2=P2&0xEF;
	if(testb)
		P2=P2|0x40;
	else
		P2=P2&0xBF;
	if(testc)
		P2=P2|0x80;
	else
		P2=P2&0x7F;

    

}

void delay(int delayTime)
{
	int i;
    for(i=0;i<delayTime;i++)
    {

    }
}