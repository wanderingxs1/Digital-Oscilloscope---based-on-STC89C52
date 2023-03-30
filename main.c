#include<main.h>
//---------------------------------------------------------------------------------------
void init_timer0(void)              //  定时器0初始
  {
  TMOD &= 0XF0;
  TMOD |= 0X02;
  TL0 = 0X06;
  TH0 = 0X06;
  TR0 = 1;
  ET0 = 1;
  }
//---------------------------------------------------------------------------------------
void init_special_interrupts(void)   //  中断设置
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
  key_num=sel;		//取得按键所在行（可能的行）
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

  //送入片选信号
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
   //选择要显示的工作模式
  switch(workMode)
   {
   case 0:  a=0xF7;  break;
   case 1:  a=0xFB;  break;
   case 2:  a=0xFD;  break;
   default: a=0xFE;
   }
   //把工作模式送出去
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
void timer_isr() interrupt 1           //定时器0中断处理
  {
  EA=0;
  adcount++;
  if(adcount==20)                      // 1/4*20 = 5ms
    {
    /*
    //读入ADC_0
    buff[p_write]=ADC_0;               // A/D读入,读入AD转换的1号单元数据
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
void fdisp(unsigned char n,unsigned char m)      //功能是将要写入的数值n对应的数码管编码装入dspbuf的第m个单元,没有用到
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
void main(void)                    // 主函数
  {
  init_timer0();                  //初始化定时器0
  init_special_interrupts();      //设置中断
  adc_init();						//设置ADC
  for(;;)
    {
	  if(key_sta&0x01)              // key_sta.0==1?，有按键按下
         {             
		 keyWork();
         key_sta=key_sta&0xfe;           // 置key_sta.0=0,复位
         } 
    /*
	if(p_read!=p_write)             //如果有读入数据，那么就写出去(只是写到了DA输出的1号单元)
   	   {
        DAC_1=buff[p_read];       // D/A输出
	    p_read++;
	    if(p_read==LEN)  p_read=0;
        }
    */
	}
  }
//----------------------------------End---------------------------------------------------
