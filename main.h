#include <reg51.h>                              // special function register declarations
#include<absacc.h>
#include<adc.h>
#include<key.h>
#include<workMode1-outputWave.h>

//Ƭ��RAM��СΪ8192(0x0000-0x1FFF)�ֽڣ�ʹ��4096(0x1000)�ֽ������ADת�����
#define ADC_BASE_ADDRESS 0x0000

void init_timer0();
void init_special_interrupts();

unsigned int         p_read=0x0000,p_write=0x0000,ad_temp=0;        //pread��Ҫ�������ݵĵ�ַ,pwrite��Ҫд�����ݵĵ�ַ,ad_temp�����һ�εĲ���ֵ
unsigned char        dspbuf[4]={0xfe,0xfe,0xfe,0xfe},sel=0;     //�����ʼ��ֻ��һ��С����
unsigned char		 lcdbuf[4]={0xF7,0xFB,0xFD,0xFE};//0��1��2��3ģʽ
unsigned int         clocktime=0,adcount=0;     //adcount���������жϴ�������,clocktimeû������

unsigned char		 ADC_RESULT=0;//��¼adת���Ľ�������������λ
unsigned char		 DAC_VALUE=0;//��¼Ҫ����DA������
unsigned char        OUTPUT_VALUE=0;//����ģʽ2��ÿ����ch2�����ֵ
unsigned int		 adAddress=ADC_BASE_ADDRESS;//ADC����Ĵ洢��ַ��λ
unsigned int		 daAddress=ADC_BASE_ADDRESS;//����ģʽ2��ch2�������ģʽ1�²ɳ��Ĳ��Σ������������ô�Ͳ������

unsigned int         sinAddress=SIN_BASE_ADDRESS;
unsigned int         triAddress=TRI_BASE_ADDRESS;
unsigned int         squAddress=SQU_BASE_ADDRESS;
unsigned int         teeAddress=TEE_BASE_ADDRESS;

unsigned  char 	key_sta=0,key_num;//��������״̬�Ͱ������(���Ա�ţ�
unsigned char workMode=1;//����ģʽ
//����ģʽ1��ch1��ȡ���룬�洢��� ch2����5678�������������������ң����ǣ�������������ֲ���
//����ģʽ2��ch1��ȡ���룬���洢ֱ����� ch2���֮ǰ�洢�õĲ���
//����ģʽ3��ch1��ȡ���룬�洢������������㡣ch2���ùܡ� ����Ҫ���������ȡƵ�ʺͷ�ֵ������ʾ��������ϣ����õ����жϣ�����ÿ��4��

sbit D_SER     = P1^0;
sbit D_SRCLK   = P1^1;
sbit D_RCLK    = P1^2;

extern char waveMode;
extern char WAVE_VALUE;