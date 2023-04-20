#include<adc.h>

extern char ADC_RESULT;
extern char DAC_VALUE;
extern char workMode;
extern char WAVE_VALUE;
extern char OUTPUT_VALUE;

unsigned int channalSelect=0x0000;//DACͨ��ѡ��

void adc_init()
{
    P1ASF=0x08;     //ȷ��p13��a-d����,����
	//��AD��Դ���ٶ���������������һ��AD��ʼλ��AD���λ��ѡ��P1^3��ΪAD����˿�
    ADC_CONTR=ADC_INIT;
    delay(2);
}



void adc_start()		//adc���ڶ�ʱ���ж���ߣ�����Ƶ��Ϊ4000Hz��
{
	//����һ��ADת����Ȼ��ȴ�ת��������ADC�ж��ǵ����ȼ�
	ADC_CONTR=ADC_START;

	//���ʱ��պ�������DAC,��DA������
    switch(workMode){
        case 1:
		channalSelect=CH1;
        dac_work(channalSelect,DAC_VALUE);
		channalSelect=CH2;
        dac_work(channalSelect,WAVE_VALUE);
        break;
        case 2:
		channalSelect=CH1;
        dac_work(channalSelect,DAC_VALUE);
		channalSelect=CH2;
        dac_work(channalSelect,OUTPUT_VALUE);
        break;
        case 3:
		channalSelect=CH1;
        dac_work(channalSelect,DAC_VALUE);
		channalSelect=CH2;
        dac_work(channalSelect,0x00);
        break;
        default:break;
    }


}


void dac_work(int channalSelect,char value){
	XBYTE[channalSelect]=value;	
}

void adc_work() interrupt 5
{
	//ADC_STARTת���������Զ���0
	//ADC_FLAGת��������Ϊ1,��Ҫ�ֶ�����
	ADC_CONTR=ADC_INIT;

	//��AD����洢
	//����:ADCת��ֵ��1024����,DACת��ֵ��256����,Ҫ����ת��,��ȥ�����λ
	ADC_RESULT=ADC_RES;
}

void delay(int delayTime)
{
	unsigned int x;
	while(delayTime--){
	x=1000;
	while(x--);
	}
}