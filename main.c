#include<main.h>
#include<adc.h>

unsigned char tmpA,tmpB,tmpC;
//����������ֵ����ʱ����

extern char isChange;//��׼����Ƶ/�������ı䰴ť5678����;
extern char freBuffer;
extern char ampBuffer;

extern char waveMode;
extern char WAVE_VALUE;
extern int freq;
extern float vpp;
//led2-4��ΪƵ�ʺͷ��ֵ���������������ѭ���ﱻ��ֵ���ڶ�ʱ���ж�1�︳��dspbuf
//��ʼ��ʾ0
//����λҪ��ʾС���㣬��0xFe
unsigned char ledbuffer[4]={0x11,0x11,0x11,0x11};//������

//---------------------------------------------------------------------------------------
void init_timer0(void)              //  ��ʱ��0��ʼ
  {
  TMOD &= 0XF0;
  TMOD |= 0X02;	   //8λ���ض�ʱ��t0
  TL0 = 0X06;
  TH0 = 0X06;	   //һ����������0.25ms
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
void init_special_interrupts(void)   //  �ж�����
  {
  //�����ж�����
  EA  = 1;		//���ж�����
  ET0 = 1;		//����T0�ж�
  ET1 = 1;    //����T1�жϣ����������������º����ǵ���ʾ
  EADC=1;		//����AD�ж�
  //�ж����ȼ�����
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
    key_num=sel;		//ȡ�ð��������У����ܵ��У�
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
void timer_isr() interrupt 1           //��ʱ��0�жϴ���
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
void updateFeature() interrupt 3    //��ʱ��1�жϴ���
{
  EA=0;
  clocktime++;
  //ȡ��������adAddress�ı�֮ǰ����
  ampMeasure();
  freMeasure();
  if(workMode==1){
  	//���ADC����RESULT��ֵ����һ�β�ͬ���Ǿ͸���,������һ��ѭ������
  //�����أ����Կ��ǽ�ÿ��AD���������Ϊ�������ݵı�׼
  	daAddress=adAddress;//ͬ��
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
  		//ͨ��2ͬ����ʱ�������
		//ͨ��2�����ǰ�ɵĲ���
		//����ģʽ2����ch2�������OUTPUT_VALUE�ĸ�ֵ,��������ģʽ2һ���Ѿ�����һ����
  		//����һ�����⣬��������ģʽ1��adAddress�����ڴ�ռ��ĳһ��ͣ��
  		//�ݶ�Ϊ��adAddress��ʼѭ�����
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
    //0.25s����һ����Ϣ
    //updateValue();
  }
  //��׼����ʾƵ�ʣ�1s-2s��Ƶ�ʣ�����Ҫ��Ƶ�ʶ�Ӧ������װ��dspbuf������
  else if(clocktime==2000)
  {
    dspbuf[1]=ledbuffer[1];
    dspbuf[2]=ledbuffer[2];
    dspbuf[3]=ledbuffer[3];
  }
  //����ʾ���ֵ����һ��0s-1s�Ƿ��ֵ)
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
//ͨ��2������ֲ�����WAVE_VALUE��ֵ
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
void fdisp(unsigned char n,unsigned char m)      //�����ǽ�Ҫд�����ֵn��Ӧ������ܱ���װ��ledbuf�ĵ�m����Ԫ
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
void main(void)                    // ������
  {
  CLK_DIV=CLK_DIV|0x01;		   //��Ƶ���ǳ���Ҫ����ΪDAC���ٶȲ��죬������������������ܲ�����
  //���ֲ���������������Ƶ���ȸ�һ���ܴ�ķ�Ƶ���ٵ���ԭ����
  init_timer0();                  //��ʼ����ʱ��0
  init_timer1();
  init_special_interrupts();      //�����ж�
  adc_init();						//����ADC
  waveInit();
  for(;;)
    {

	//����ģʽ1�Ĳ���ѡ��
	//����ģʽ2��������в���
  //���ǽ����������������Լ�ά����buffer
	switch(workMode){
	case 1:
	{
	//ͨ��1ʵʱ���
	DAC_VALUE=ADC_RESULT;
	}
	break;
	case 2:
	{
		//ͨ��1ʵʱ���
		DAC_VALUE=ADC_RESULT;
	}
	break;
  case 3:
  {
    //ͨ��1ʵʱ���
    DAC_VALUE=ADC_RESULT;
  }
  break;
	default:break;
	}

  //��������
  if(key_sta&0x01)              // key_sta.0==1?���а�������
    {             
		    keyWork();
        key_sta=key_sta&0xfe;           // ��key_sta.0=0,��λ
    }
  //������Ƶ�ʺ�vpp���룬����Ƶ��Ϊ0-999��vppΪ0.0-9.9
  if(clocktime<2000)   //ǰ1s��ʾ���ֵ��һֱ����Ƶ�ʣ������1s����д�롣��1s��ʾƵ�ʣ�һֱ������ֵ�������µ�ǰ1s����д��
  {
    unsigned char tmp1=freq/100;//�ֱ����̺�����
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
