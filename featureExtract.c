#include<featureExtract.h>

extern char ADC_RESULT;
extern int adAddress;

unsigned int freq=0;//待测信号频率,0-300Hz
float vpp=0.0;//绝对电压值
unsigned char amp=0;//这一时刻的采样数字值
unsigned char ampl=0;//前一次的采样数字值
//这两个值一定要初始置为中值！
unsigned char amp_up=128;//数据块内的最高电压值
unsigned char amp_low=128;//数据块内的最低电压值
int fre=0;//数据块内所有完整周期的采样点总个数
int fre_up=0;//一个周期的末地址
int fre_low=0;//一个周期的首地址
float fre_count=0;//已经经历的周期个数

void ampMeasure()
{
    amp=ADC_RESULT;
    if (amp>amp_up){amp_up=amp;}
    if (amp<amp_low){amp_low=amp;}
	//计数范围为一个数据块
    if(adAddress>0x0400){
        vpp=(amp_up*5.0-amp_low*5.0)/128;//之前为了示波器显示给幅值除2，这里乘回来
        amp_up=amp_low=128;
    }
}

void freMeasure()
{
    amp=ADC_RESULT;
	//作图判断时序
	//判断一个周期的开始，即前一次小于0，后一次大于0
    if (amp>128 && ampl<=128)
    {
    fre_up=adAddress; //第fre_count个周期的首地址
    if(fre_low!=0)	  //第0个周期之后才会计数
    {
    fre=fre+fabs(fre_low-fre_up);//后一项fabs()是一个周期的采样点个数,fre是数据块内所有完整周期的采样点总个数
    fre_count++;
    }
    fre_low=fre_up;	//更新为第(fre_count+1)	个周期的首地址
    }
	//数据块处理完
    if(adAddress>0x0400){
		//对输出回放波形存储的采样频率为4000/2=2000Hz（注意二分频）
        freq=floor(2000/(fre*1.0/fre_count));
        fre=0;
        fre_up=fre_low=0;
        fre_count=0;
        amp=ampl=129;
    }
    ampl=amp;

}