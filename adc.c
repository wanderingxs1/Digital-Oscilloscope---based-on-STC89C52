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
    P1ASF=0x08;     //ȷ��p13��a-d����,����
    ADC_CONTR=0x0CB;//��ipad�ʼ�
    //AUXR1=0x04;//AUXR1�޷�����ֵ����Ĭ��Ϊ0��ֻ��ʹ�ø�8λ��ADS_RES��ߣ���2λ��ADC_RESL�ĵ�2λ������λ������
    ADC_RES=0x00;
    ADC_RESL=0x00;

    //�����ж�
    EADC=1;//AD�ж�����
    IP=0x24;//AD������,�ⲿ�ж�1�θ�,��ʱ�����
    delay(2);
}

void adc_work() interrupt 5 using 1		//ʵʱ��ǿ���ɵ�һ������������Ǵ洢��ÿ4msһ�Ρ���ʱadcount>=16.
{
    //����ADC�жϿ���λ
    ADC_CONTR=ADC_CONTR&0xEF;
    //����P25-P27,����λ��Ӧ�ܽŷ���S0Ƭѡ�ź�,Ƭѡ�洢��
	testa=P2^5;
    testb=P2^6;
    testc=P2^7;
    P2=P2|0x20;
	P2=P2&0xBF;
	P2=P2&0x7F;
	//����adcֵ�Ĵ洢���ⲿRAM0x6000-0x7000),���ֻ�ܴ���ô��
	if(adcount>=16&&adAddress<0x7000)
	{
	   XBYTE[adAddress]=ADC_RES;
	   adAddress++;
	}

    //����ADCֵ��DAC��ת��

    //����:ADCת��ֵ��1024����,DACת��ֵ��256����,Ҫ����ת��,��ȥ�����λ
	ADC_RESULT=ADC_RES;
	ADC_RES=0x00;
	ADC_RESL=0x00;

    //����DAC��ֵ
    //ʹ��S1 DACоƬ
	P2=P2&0xEF;
	P2=P2|0x40;
	P2=P2&0x7F;
    //дѡͨ
    P3=P3&0xBF;
    //��P0�ڸ�ֵ
    P0=ADC_RESULT;
	delay(10);
    //��ɶ˿ڱ���
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