#include<main.h>
#include<adc.h>

unsigned char tmpA,tmpB,tmpC;
//用来输出峰峰值的临时变量

extern char isChange;//是准备调频/调幅，改变按钮5678的用途
extern char freBuffer;
extern char ampBuffer;

extern char waveMode;
extern char WAVE_VALUE;
extern int freq;
extern float vpp;
//led2-4作为频率和峰峰值的译码变量，在主循环里被赋值，在定时器中断1里赋给dspbuf
//初始显示0
//第三位要显示小数点，与0xFe
unsigned char ledbuffer[4]={0x11,0x11,0x11,0x11};//缓冲区

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
void init_timer1(void)
{
  TMOD &= 0x0F;
  TMOD |= 0x20;
  TL1 = 0x06;
  TH1 = 0x06;
  TR1 = 1;
}
void init_special_interrupts(void)   //  中断设置
  {
  //两个中断配置
  EA  = 1;		//总中断允许
  ET0 = 1;		//允许T0中断
  ET1 = 1;    //允许T1中断，驱动特征参数更新和它们的显示
  EADC=1;		//允许AD中断
  //中断优先级配置
  PT0 = 1;
  PT1 = 0;
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
   case 3:  a=0x08;	 break;
   default: a=0x10;  break;
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
  if(sel<=3){
  	a=dspbuf[sel];  
  }
  else{
  	switch(workMode)
   {
   case 1:  a=0xF7;  break;
   case 2:  a=0xFB;  break;
   case 3:  a=0xFD;  break;
   default: a=0xFE;  break;
   }
  }
  if(sel<=3)
    key_num=sel;		//取得按键所在行（可能的行）
  sel++;
  if(sel>4) sel=0;
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
  if(adcount==3||adcount==6||adcount==9){
	updateWaveBuffer(); 
  }
  if(adcount==10)                      // 1/4*10/2 = 5ms
    {
    dsptask();
    key_service();
    adcount=0;
    }
  EA=1;
  }
//---------------------------------------------------------------------------------------
void updateFeature() interrupt 3    //定时器1中断处理
{
  EA=0;
  clocktime++;
  //取特征得在adAddress改变之前进行
  ampMeasure();
  freMeasure();
  if(workMode==1){
  	//如果ADC——RESULT的值和上一次不同，那就更新,构造了一个循环阵列
  //方波呢？可以考虑将每次AD采样完成作为存入数据的标准
  	daAddress=adAddress;//同步
	if(adAddress<=0x0400){
	XBYTE[adAddress]=(ADC_RESULT-64)*2;
	ad_temp=ADC_RESULT;
	adAddress++;
	}
	else{
	adAddress=ADC_BASE_ADDRESS;
	XBYTE[adAddress]=(ADC_RESULT-64)*2;
	ad_temp=ADC_RESULT;
    adAddress++;
	}
	}
  if(workMode==2){
  		//通道2同样定时输出波形
		//通道2输出以前采的波形
		//工作模式2进行ch2输出变量OUTPUT_VALUE的赋值,调到工作模式2一定已经采完一轮了
  		//留有一个问题，跳出工作模式1后adAddress会在内存空间的某一处停下
  		//暂定为从adAddress开始循环输出
    		if(daAddress<=0x0400){
      			OUTPUT_VALUE=XBYTE[daAddress]/2;
      			daAddress=daAddress+1;
    		}
    		else{
      			daAddress=ADC_BASE_ADDRESS;
      			OUTPUT_VALUE=XBYTE[daAddress]/2;
      			daAddress++;
    	}
  }
  if(clocktime==500)
  {
    //0.25s更新一次信息
    //updateValue();
  }
  //先准备显示频率（1s-2s是频率），需要把频率对应的数码装进dspbuf数组中
  else if(clocktime==2000)
  {
    dspbuf[1]=ledbuffer[1];
    dspbuf[2]=ledbuffer[2];
    dspbuf[3]=ledbuffer[3];
  }
  //再显示峰峰值（下一个0s-1s是峰峰值)
  else if(clocktime==4000)
  {
    dspbuf[1]=0x11;
    dspbuf[2]=ledbuffer[2]&0xFE;
    dspbuf[3]=ledbuffer[3];
    clocktime=0;
  }
  EA=1;
}

void updateWaveBuffer(){
	if(workMode==1){
//通道2输出四种波，给WAVE_VALUE赋值
		switch(waveMode){
			case 1:
      {
        if(sinAddress<=0x10FF){
				if(ampBuffer!=1){
      			WAVE_VALUE=XBYTE[sinAddress]*ampBuffer;}
				else{
				WAVE_VALUE=XBYTE[sinAddress];
				}
				sinAddress=sinAddress+1+freBuffer;
    		  }
    	  else{
      		sinAddress=SIN_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[sinAddress]*ampBuffer;
      		sinAddress=sinAddress+1+freBuffer;
    	}
      }
      break;
			case 2:
       {
        if(triAddress+freBuffer<=0x11F3){
      			WAVE_VALUE=XBYTE[triAddress+freBuffer]+ampBuffer;
      			triAddress++;
    		  }
    	  else{
      		triAddress=TRI_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[triAddress];
      		triAddress++;
    	}
      }
      break;
			case 3:
      {
        if(squAddress+freBuffer<=0x12F3){
      			WAVE_VALUE=XBYTE[squAddress+freBuffer]+ampBuffer;
      			squAddress++;
    		  }
    	  else{
      		squAddress=SQU_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[squAddress];
      		squAddress++;
    	}
      }
      break;
			case 4:
      {
        if(teeAddress+freBuffer<=0x1379){
      			WAVE_VALUE=XBYTE[teeAddress+freBuffer]+ampBuffer;
      			teeAddress++;
    		  }
    	  else{
      		teeAddress=TEE_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[teeAddress];
      		teeAddress++;
    	}
      }
      break;
			default:break;
		}
		}
}

//---------------------------------------------------------------------------------------
void fdisp(unsigned char n,unsigned char m)      //功能是将要写入的数值n对应的数码管编码装入ledbuf的第m个单元
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
   ledbuffer[m]=c;
  }
//---------------------------------------------------------------------------------------
void main(void)                    // 主函数
  {
  CLK_DIV=CLK_DIV|0x01;		   //分频，非常重要！因为DAC的速度不快，对它灌入过快数据它受不了了
  //发现不能运行立即调分频，先给一个很大的分频，再调回原来的
  init_timer0();                  //初始化定时器0
  init_timer1();
  init_special_interrupts();      //设置中断
  adc_init();						//设置ADC
  waveInit();
  for(;;)
    {

	//工作模式1的波形选择
	//工作模式2的输出已有波形
  //都是将所需数据送入它自己维护的buffer
	switch(workMode){
	case 1:
	{
	//通道1实时输出
	DAC_VALUE=ADC_RESULT;
	}
	break;
	case 2:
	{
		//通道1实时输出
		DAC_VALUE=ADC_RESULT;
	}
	break;
  case 3:
  {
    //通道1实时输出
    DAC_VALUE=ADC_RESULT;
  }
  break;
	default:break;
	}

  //按键处理
  if(key_sta&0x01)              // key_sta.0==1?，有按键按下
    {             
		    keyWork();
        key_sta=key_sta&0xfe;           // 置key_sta.0=0,复位
    }
  //对已有频率和vpp译码，假设频率为0-999，vpp为0.0-9.9
  if(clocktime<2000)   //前1s显示峰峰值，一直译码频率，到达后1s立即写入。后1s显示频率，一直译码峰峰值，到达新的前1s立即写入
  {
    unsigned char tmp1=freq/100;//分别是商和余数
    unsigned char tmp2=freq%100;
    fdisp(tmp1,1);
    tmp1=tmp2/10;
    tmp2=tmp2%10;
    fdisp(tmp1,2);
    fdisp(tmp2,3);
  }
  else{
    tmpA=floor(vpp);
    fdisp(tmpA,2);
    tmpB=floor(vpp*10);
    tmpC=tmpB%10;
    fdisp(tmpC,3);
  }
	}
  }
//----------------------------------End---------------------------------------------------
