#include <reg51.h>                              // special function register declarations
#include<absacc.h>
#include<adc.h>
#include<key.h>

#define  DAC_1    XBYTE[0x2000]
#define  DAC_2    XBYTE[0x4000]

#define  ADC_0    XBYTE[0x6000]

#define ADC_BASE_ADDRESS 0x6000

#define  LEN        10

void init_timer0();
void init_special_interrupts();

//unsigned char        buff[LEN];             //���ݻ�����
//unsigned char        p_read=0,p_write=0,ad_temp;        //pread�ǵڶ��ٸ��������ֵ,pwrite�ǵڶ��ٸ�Ҫ�������ֵ,ad_tempû���õ�
unsigned char        dspbuf[4]={0xfe,0xfe,0xfe,0xfe},sel=0;     //�����ʼ��ֻ��һ��С����
unsigned char		 lcdbuf[4]={0xF7,0xFB,0xFD,0xFE};//0��1��2��3ģʽ
unsigned int         clocktime=0,adcount=0;     //adcount���������жϴ�������,clocktimeû������

unsigned char		 ADC_RESULT=0;//��¼adת���Ľ�������������λ
unsigned int		 adAddress=ADC_BASE_ADDRESS;//ADC����Ĵ洢��ַ��λ
unsigned int		 daAddress=ADC_BASE_ADDRESS;//����ģʽ2��ch2�������ģʽ1�²ɳ��Ĳ��Σ������������ô�Ͳ������

unsigned char workMode=0;//����ģʽ0,��λ
//����ģʽ1��ch1��ȡ���룬�洢��� ch2����5678�������������������ң����ǣ�������������ֲ���
//����ģʽ2��ch1��ȡ���룬���洢ֱ����� ch2���֮ǰ�洢�õĲ���
//����ģʽ3��ch1��ȡ���룬�洢������������㡣ch2���ùܡ� ����Ҫ���������ȡƵ�ʺͷ�ֵ������ʾ��������ϣ����õ����жϣ�����ÿ��4��

unsigned  char 	key_sta=0,key_num;//��������״̬�Ͱ������(���Ա�ţ�

sbit D_SER     = P1^0;
sbit D_SRCLK   = P1^1;
sbit D_RCLK    = P1^2;
sbit KEY1      = P3^4;
sbit KEY2      = P3^5;