#include<main.h>
#include<adc.h>
//---------------------------------------------------------------------------------------
void init_timer0(void)              //  定时器0初始
  {
  TMOD &= 0XF0;
  TMOD |= 0X02;	   //8位重载定时器t0
  TL0 = 0X06;
  TH0 = 0X06;	   //一个计数周期0.25ms
  TR0 = 1;
  }
//---------------------------------------------------------------------------------------
void init_special_interrupts(void)   //  中断设置
  {
  //两个中断配置
  EA  = 1;		//总中断
  ET0 = 1;		//允许T0中断
  EADC=1;		//允许AD中断
  //中断优先级配置
  PT0 = 1;
  PADC=0;
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
   default: a=0x0E;
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
  adc_start();
  if(adcount==20)                      // 1/4*20 = 5ms
    {
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

  //工作模式1才会随时进行存储
  if(workMode==1){
	//如果ADC——RESULT的值和上一次不同，那就更新,构造了一个循环阵列
	if(ad_temp!=ADC_RESULT){
  daAddress=adAddress;//同步
	if(adAddress<=0x0FFF){
	XBYTE[adAddress]=ADC_RESULT;
	ad_temp=ADC_RESULT;
	adAddress++;
	}
	else{
	adAddress=ADC_BASE_ADDRESS;
	XBYTE[adAddress]=ADC_RESULT;
	ad_temp=ADC_RESULT;
    adAddress++;
	}
	}
  }

  //工作模式2进行ch2输出变量OUTPUT_VALUE的赋值,调到工作模式2一定已经采完一轮了
  //留有一个问题，跳出工作模式1后adAddress会在内存空间的某一处停下
  //暂定为从adAddress开始循环输出
  if(workMode==2){
    if(daAddress<=0x0FFF){
      OUTPUT_VALUE=XBYTE[daAddress];
      daAddress++;
    }
    else{
      daAddress=ADC_BASE_ADDRESS;
      OUTPUT_VALUE=XBYTE[daAddress];
      daAddress++;
    }
  }

  //按键处理
  if(key_sta&0x01)              // key_sta.0==1?，有按键按下
    {             
		 keyWork();
         key_sta=key_sta&0xfe;           // 置key_sta.0=0,复位
    } 
	}
  }
//----------------------------------End---------------------------------------------------
