#include<main.h>
//---------------------------------------------------------------------------------------
void init_timer0(void)              //  ��ʱ��0��ʼ
  {
  TMOD &= 0XF0;
  TMOD |= 0X02;
  TL0 = 0X06;
  TH0 = 0X06;
  TR0 = 1;
  ET0 = 1;
  }
//---------------------------------------------------------------------------------------
void init_special_interrupts(void)   //  �ж�����
  {
  EX0 = 0;
  ET0 = 1;
  EX1 = 0;
  ET1 = 0;
  ES  = 1;
  EA  = 1;
  PT0 = 0;
  PX1 = 1;
  PS  = 0;
  }
//---------------------------------------------------------------------------------------
void dsptask()
  {
  unsigned char i;
  unsigned char a,b;

  switch(sel)
   {
   case 0:  a=0x01;  break;
   case 1:  a=0x02;  break;
   case 2:  a=0x04;  break;
   default: a=0x08;
   }
 for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7f;
    }
  a=dspbuf[sel];
  key_num=sel;		//ȡ�ð��������У����ܵ��У�
  sel++;
  if(sel>=4) sel=0;
  for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7fff;
    }
  D_RCLK=0;
  D_RCLK=1;
  D_RCLK=0;

  //����Ƭѡ�ź�
  a=0x10;
  for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7f;
    }
   //ѡ��Ҫ��ʾ�Ĺ���ģʽ
  switch(workMode)
   {
   case 0:  a=0xF7;  break;
   case 1:  a=0xFB;  break;
   case 2:  a=0xFD;  break;
   default: a=0xFE;
   }
   //�ѹ���ģʽ�ͳ�ȥ
  for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7fff;
    }
  D_RCLK=0;
  D_RCLK=1;
  D_RCLK=0;

  }
//---------------------------------------------------------------------------------------
void timer_isr() interrupt 1           //��ʱ��0�жϴ���
  {
  EA=0;
  adcount++;
  if(adcount==20)                      // 1/4*20 = 5ms
    {
    /*
    //����ADC_0
    buff[p_write]=ADC_0;               // A/D����,����ADת����1�ŵ�Ԫ����
    ADC_0=0;                           // restart A/D
    p_write++;
    if(p_write==LEN)  p_write=0;
    */
    dsptask();
	key_service();
    clocktime++;
    adcount=0;
    }
  EA=1;
  }
//---------------------------------------------------------------------------------------
void fdisp(unsigned char n,unsigned char m)      //�����ǽ�Ҫд�����ֵn��Ӧ������ܱ���װ��dspbuf�ĵ�m����Ԫ,û���õ�
  {
  char  c;
   switch(n)
    {
    case 0:  c=0x11;  break;
    case 1:  c=0x7d;  break;
    case 2:  c=0x23;  break;
    case 3:  c=0x29;  break;
    case 4:  c=0x4d;  break;
    case 5:  c=0x89;  break;
    case 6:  c=0x81;  break;
    case 7:  c=0x3d;  break;
    case 8:  c=0x01;  break;
    default: c=0x09; 
    }
   dspbuf[m]=c;
  }
//---------------------------------------------------------------------------------------
void main(void)                    // ������
  {
  init_timer0();                  //��ʼ����ʱ��0
  init_special_interrupts();      //�����ж�
  adc_init();						//����ADC
  for(;;)
    {
	  if(key_sta&0x01)              // key_sta.0==1?���а�������
         {             
		 keyWork();
         key_sta=key_sta&0xfe;           // ��key_sta.0=0,��λ
         } 
    /*
	if(p_read!=p_write)             //����ж������ݣ���ô��д��ȥ(ֻ��д����DA�����1�ŵ�Ԫ)
   	   {
        DAC_1=buff[p_read];       // D/A���
	    p_read++;
	    if(p_read==LEN)  p_read=0;
        }
    */
	}
  }
//----------------------------------End---------------------------------------------------
