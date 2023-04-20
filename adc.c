#include<adc.h>

extern char ADC_RESULT;
extern char DAC_VALUE;
extern char workMode;
extern char WAVE_VALUE;
extern char OUTPUT_VALUE;

unsigned int channalSelect=0x0000;//DAC通道选择

void adc_init()
{
    P1ASF=0x08;     //确定p13是a-d功能,输入
	//开AD电源，速度设置最慢，清零一次AD开始位和AD完成位，选择P1^3作为AD输入端口
    ADC_CONTR=ADC_INIT;
    delay(2);
}



void adc_start()		//adc放在定时器中断里边，采样频率为4000Hz。
{
	//启动一次AD转换，然后等待转换结束。ADC中断是低优先级
	ADC_CONTR=ADC_START;

	//这段时间刚好用来做DAC,把DA结果输出
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
	//ADC_START转换结束后自动置0
	//ADC_FLAG转换结束后为1,需要手动清零
	ADC_CONTR=ADC_INIT;

	//把AD结果存储
	//问题:ADC转换值是1024个阶,DAC转换值是256个阶,要进行转换,舍去最低两位
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