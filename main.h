#include <reg51.h>                              // special function register declarations
#include<absacc.h>
#include<adc.h>
#include<key.h>
#include<workMode1-outputWave.h>

//片外RAM大小为8192(0x0000-0x1FFF)字节，使用4096(0x1000)字节来存放AD转换结果
#define ADC_BASE_ADDRESS 0x0000

void init_timer0();
void init_special_interrupts();

unsigned int         p_read=0x0000,p_write=0x0000,ad_temp=0;        //pread是要读出数据的地址,pwrite是要写入数据的地址,ad_temp存放上一次的采样值
unsigned char        dspbuf[4]={0xfe,0xfe,0xfe,0xfe},sel=0;     //数码管始终只亮一个小数点
unsigned char		 lcdbuf[4]={0xF7,0xFB,0xFD,0xFE};//0，1，2，3模式
unsigned int         clocktime=0,adcount=0;     //adcount用来计数中断触发个数,clocktime没有作用

unsigned char		 ADC_RESULT=0;//记录ad转换的结果，舍弃最低两位
unsigned char		 DAC_VALUE=0;//记录要放入DA的数据
unsigned char        OUTPUT_VALUE=0;//工作模式2下每次在ch2输出的值
unsigned int		 adAddress=ADC_BASE_ADDRESS;//ADC结果的存储地址首位
unsigned int		 daAddress=ADC_BASE_ADDRESS;//工作模式2，ch2输出工作模式1下采出的波形，如果不够，那么就不输出了

unsigned int         sinAddress=SIN_BASE_ADDRESS;
unsigned int         triAddress=TRI_BASE_ADDRESS;
unsigned int         squAddress=SQU_BASE_ADDRESS;
unsigned int         teeAddress=TEE_BASE_ADDRESS;

unsigned  char 	key_sta=0,key_num;//按键按下状态和按键编号(绝对编号）
unsigned char workMode=1;//工作模式
//工作模式1：ch1读取输入，存储输出 ch2根据5678按键按下情况，输出正弦，三角，方波，锯齿四种波形
//工作模式2：ch1读取输入，不存储直接输出 ch2输出之前存储好的波形
//工作模式3：ch1读取输入，存储，输出不输出随便。ch2不用管。 但是要根据输入获取频率和幅值，并显示在数码管上，配置低速中断，至少每秒4次

sbit D_SER     = P1^0;
sbit D_SRCLK   = P1^1;
sbit D_RCLK    = P1^2;

extern char waveMode;
extern char WAVE_VALUE;